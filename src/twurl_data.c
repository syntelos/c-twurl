/*
 * Syntelos TWURL
 * Copyright (C) 2022, John Pritchard, Syntelos
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * (LGPL and GPL) as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL
 * and GPL for more details.
 *
 * You should have received a copy of the LGPL and GPL along with this
 * program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "twurl_data.h"
#include "json.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

/*
 * libcurl write_callback
 */
size_t twurl_data_iob_reader(char *source, size_t size, size_t many, void *data_iob){
    if (null != source && 0 < size && 0 < many && null != data_iob){
        twurl_data_iob *cx = data_iob;
        size_t input = (size*many);
        {
            size_t nsize = (cx->limit+input);

            if (nsize > cx->size){

                if (0 == cx->size){

                    cx->content = malloc(nsize);
                }
                else {
                    cx->content = realloc(cx->content,nsize);
                }

                if (null != cx->content){
                    cx->size = nsize;

                    char *target = (cx->content+cx->limit);

                    memset(target,0,input);
                }
            }

            char *target = (cx->content+cx->limit);

            memcpy(target,source,input);

            cx->limit += input;
        }
        return input;
    }
    else {
        return 0;
    }
}

void twurl_data_iob_destroy(twurl_data_iob *cx){

    if (null != cx){

        if (null != cx->content){

            if (0 != cx->size){
                memset(cx->content,0,cx->size);
            }
            free(cx->content);
        }

        memset(cx,0,sizeof(twurl_data_iob));
        free(cx);
    }
}

twurl_data_rec_create_mode _data_rec_create_mode = twurl_data_rec_create_mode_table;

twurl_data_rec* _data_rec_create(twurl_data_rec *record, char *name, off_t name_size, char *value, off_t value_size){
    if (null != name && 0 < name_size && null != value && 0 < value_size){

        if (null == record){

            record = calloc(1,sizeof(twurl_data_rec));

            if (null != record){
                off_t nz = min(twurl_identity_size,name_size);

                memset(record->identity,0,twurl_identity_size);
                memcpy(record->identity,name,nz);

                off_t vz = min(twurl_object_size,value_size);

                memset(record->object,0,twurl_object_size);
                memcpy(record->object,value,vz);
            }
        }
        else if (twurl_data_rec_create_mode_table == _data_rec_create_mode){
            /*
             * In table mode each identity is a unique
             * identifier in the data record set.
             */
            twurl_data_rec *tgt = record;
            while (null != tgt){

                if (0 == strcmp(name,tgt->identity)){
                    off_t cz = min(twurl_object_size,value_size);

                    memset(tgt->object,0,twurl_object_size);
                    memcpy(tgt->object,value,cz);

                    break;
                }
                else if (null != tgt->next){

                    tgt = tgt->next;
                }
                else {
                    tgt->next = calloc(1,sizeof(twurl_data_rec));
                    tgt = tgt->next;
                    if (null != tgt){
                        off_t nz = min(twurl_identity_size,name_size);

                        memset(tgt->identity,0,twurl_identity_size);
                        memcpy(tgt->identity,name,nz);

                        off_t vz = min(twurl_object_size,value_size);

                        memset(tgt->object,0,twurl_object_size);
                        memcpy(tgt->object,value,vz);
                    }
                    break;
                }
            }

        }
        else {
            /*
             * In list mode the unique identity of each
             * identifier in the data record set defines a
             * serial list element.
             */
            twurl_data_rec *tgt = record;
            while (null != tgt){

                if (null != tgt->next){

                    tgt = tgt->next;
                }
                else {
                    tgt->next = calloc(1,sizeof(twurl_data_rec));
                    tgt = tgt->next;
                    if (null != tgt){
                        off_t nz = min(twurl_identity_size,name_size);

                        memset(tgt->identity,0,twurl_identity_size);
                        memcpy(tgt->identity,name,nz);

                        off_t vz = min(twurl_object_size,value_size);

                        memset(tgt->object,0,twurl_object_size);
                        memcpy(tgt->object,value,vz);
                    }
                    break;
                }
            }

        }
    }
    return record;
}

twurl_data_rec* twurl_data_rec_create(twurl_data_rec *record, twurl_data_iob *cx){
    if (null != cx && 0 < cx->limit){

        json_value *doc = json_parse(cx->content,cx->limit);
        if (null != doc){
            /*
             * The "condensation" process flattens the JSON
             * object into an equivalent of an RDBMS table.  The
             * JSON object name and value are handled as
             * identity and object of association.
             *
             * The observable of interest is the opaque
             * object of association, i.e.  { "name":
             * "value"} where the right hand size of the
             * object association is not a complex like a
             * list or object.
             *
             * This term is an object identity which is
             * unique by its identifier.
             */
            if (json_object == doc->type){
                off_t length = doc->u.object.length;

                json_object_entry *begin = doc->u.object.values;
                json_object_entry *end = doc->u.object.values+length;
                json_object_entry *lp = begin;

                for (; lp < end; lp++){

                    char *n = lp->name;
                    off_t nz = lp->name_length;
                    json_value *v = lp->value;
                    if (json_object == v->type){
                        off_t ol = v->u.object.length;

                        json_object_entry *ob = v->u.object.values;
                        json_object_entry *oz = v->u.object.values+ol;
                        json_object_entry *op = ob;

                        for (; op < oz; op++){

                            char *von = op->name;
                            off_t vonz = op->name_length;
                            json_value *vov = op->value;

                            if (json_string == vov->type){
                                off_t ll = vov->u.string.length;
                                char* ss = vov->u.string.ptr;

                                record = _data_rec_create(record,von,vonz,ss,ll);
                            }
                        }
                    }
                    else if (json_string == v->type){
                        off_t ll = v->u.string.length;
                        char* ss = v->u.string.ptr;

                        record = _data_rec_create(record,n,nz,ss,ll);
                    }
                }
            }
            else if (json_array == doc->type){
                off_t length = doc->u.array.length;

                json_value **begin = doc->u.array.values;
                json_value **end = doc->u.array.values+length;
                json_value **lp = begin;

                for (; lp < end; lp++){
                    json_value *ap = *lp;
                    if (json_object == ap->type){
                        off_t ol = ap->u.object.length;

                        json_object_entry *ob = ap->u.object.values;
                        json_object_entry *oz = ap->u.object.values+ol;
                        json_object_entry *op = ob;

                        for (; op < oz; op++){

                            char *n = op->name;
                            off_t nz = op->name_length;
                            json_value *v = op->value;

                            if (json_string == v->type){
                                off_t ll = v->u.string.length;
                                char* ss = v->u.string.ptr;

                                record = _data_rec_create(record,n,nz,ss,ll);
                            }
                        }
                    }
                }
            }
            json_value_free(doc);
        }
    }
    return record;
}

int twurl_data_rec_indexof(twurl_data_rec *record, char *name){
    if (null != record && null != name){
        twurl_data_rec *rp;
        int idx;

        for (idx = 0, rp = record; null != rp; idx++, rp = rp->next){

            if (0 == strcmp(rp->identity,name)){

                return idx;
            }
        }
    }
    return -1;
}

twurl_data_rec* twurl_data_rec_find(twurl_data_rec *record, char *name){
    if (null != record && null != name){
        twurl_data_rec *rp;

        for (rp = record; null != rp; rp = rp->next){

            if (0 == strcmp(rp->identity,name)){

                return rp;
            }
        }
    }
    return null;
}

void twurl_data_rec_destroy(twurl_data_rec *record){

    while (null != record){

        twurl_data_rec *n = record->next;
        {
            memset(record,0,sizeof(twurl_data_rec));
            free(record);
        }
        record = n;
    }
}

twurl_data_rec_create_mode twurl_data_rec_create_mode_get(){

    return  _data_rec_create_mode;
}

bool_t twurl_data_rec_create_mode_set(twurl_data_rec_create_mode m){

    switch (m){

    case twurl_data_rec_create_mode_table:
    case twurl_data_rec_create_mode_series:

        _data_rec_create_mode = m;

        return true;

    default:
        return false;
    }
}
