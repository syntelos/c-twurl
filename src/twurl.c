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

#include "twurl.h"
#include "twurl_data.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define TWURL_UA "syntelos-twurl/0.2"

char* twurl_work = null;

char* twurl_store = null;

char *work_read(char *file){

    if (0 == chdir(twurl_work)){

        fd_t fd = open(file,O_RDONLY);
        if (-1 < fd){
            struct stat st;
            if (0 == fstat(fd,&st)){

                off_t loa = st.st_size;

                if (0 < loa){
                    /*
                     * Read file to buffer
                     */
                    char *buffer = calloc(loa,sizeof(char));
                    char *bp = buffer;
                    ssize_t rd;
                    size_t ct = loa;

                    while (0 < (rd = read(fd,bp,ct))){

                        if (1 > rd){

                            break;
                        }
                        else {
                            bp += rd;
                            ct -= rd;
                        }
                    }

                    if (0 == ct){
                        /*
                         * trim tail termination (CR|LF)
                         */
                        off_t idx;

                        char *tp = buffer+(loa-1);

                        for (idx = 0; idx < loa; idx++, tp--){

                            if (0 == *tp){

                                continue;
                            }
                            else if (0x20 > *tp){

                                *tp = 0;
                            }
                            else {
                                break;
                            }
                        }

                        /*
                         */
                        return buffer;
                    }
                    else {
                        free(buffer);
                    }
                }
            }

            close(fd);
        }
    }
    return null;
}
bool_t work_write(char *file, char *source, off_t size){

    if (0 == chdir(twurl_work)){

        fd_t fd = open(file,(O_WRONLY|O_CREAT|O_TRUNC),0644);
        if (-1 < fd){

            ssize_t wr;
            size_t rm = size;
            char *sp = source;

            while (0 < (wr = write(fd,sp,rm))){

                sp += wr;
                rm -= wr;
            }

            close(fd);

            return true;
        }
    }
    return false;
}
char *store_read(char *file){

    if (0 == chdir(twurl_store)){

        fd_t fd = open(file,O_RDONLY);
        if (-1 < fd){
            struct stat st;
            if (0 == fstat(fd,&st)){

                off_t loa = st.st_size;

                if (0 < loa){
                    /*
                     * Read file to buffer
                     */
                    char *buffer = calloc(loa,sizeof(char));
                    char *bp = buffer;
                    ssize_t rd;
                    size_t ct = loa;

                    while (0 < (rd = read(fd,bp,ct))){

                        if (1 > rd){

                            break;
                        }
                        else {
                            bp += rd;
                            ct -= rd;
                        }
                    }

                    if (0 == ct){
                        /*
                         * trim tail termination (CR|LF)
                         */
                        off_t idx;

                        char *tp = buffer+(loa-1);

                        for (idx = 0; idx < loa; idx++, tp--){

                            if (0 == *tp){

                                continue;
                            }
                            else if (0x20 > *tp){

                                *tp = 0;
                            }
                            else {
                                break;
                            }
                        }

                        /*
                         */
                        return buffer;
                    }
                    else {
                        free(buffer);
                    }
                }
            }

            close(fd);
        }
    }
    return null;
}
bool_t store_write(char *file, char *source, off_t size){

    if (0 == chdir(twurl_store)){

        fd_t fd = open(file,(O_WRONLY|O_CREAT|O_TRUNC),0644);
        if (-1 < fd){

            ssize_t wr;
            size_t rm = size;
            char *sp = source;

            while (0 < (wr = write(fd,sp,rm))){

                sp += wr;
                rm -= wr;
            }

            close(fd);

            return true;
        }
    }
    return false;
}
char *store_read_username(){
    return store_read("api_key.txt");
}
char *store_read_password(){
    return store_read("api_key_secret.txt");
}
char *store_read_bearer(){
    return store_read("bearer_token.txt");
}

/*
 * Internal application context.
 */
twurl_data_rec *data_rec = null;

/*
 * Employ api/consumer secret and bearer token to fetch URL
 * to standard console output.
 */
bool_t twurl_app_post(char *scope, char *url){
    if (null != url){

        char *username = store_read_username();
        if (null != username){
            char *password = store_read_password();
            if (null != password){

                CURL *curl = curl_easy_init();
                if (null != curl) {
                    curl_easy_setopt(curl, CURLOPT_URL, url);
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, TWURL_UA);

                    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

                    curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);

                    curl_easy_setopt(curl, CURLOPT_USERNAME, username);

                    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

                    curl_easy_setopt(curl, CURLOPT_POST, 1);
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scope);

                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &twurl_data_iob_reader);

                    twurl_data_iob *data_iob = calloc(1,sizeof(twurl_data_iob));

                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data_iob);

                    CURLcode re = curl_easy_perform(curl);

                    curl_easy_cleanup(curl);

                    free(username);

                    free(password);

                    if (CURLE_OK == re){

                        data_rec = twurl_data_rec_create(data_rec,data_iob);

                        twurl_data_iob_destroy(data_iob);

                        return true;
                    }
                    else {
                        twurl_data_iob_destroy(data_iob);
                    }
                }
                else {
                    free(username);

                    free(password);
                }
            }
            else {
                free(username);
            }
        }
    }
    return false;
}
/*
 * Employ api/consumer secret and bearer token to fetch URL
 * to standard console output.
 */
bool_t twurl_app_get(char *url){
    if (null != url){

        char *username = store_read_username();
        if (null != username){
            char *password = store_read_password();
            if (null != password){

                CURL *curl = curl_easy_init();
                if (null != curl) {
                    curl_easy_setopt(curl, CURLOPT_URL, url);
                    curl_easy_setopt(curl, CURLOPT_USERAGENT, TWURL_UA);

                    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

                    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

                    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);

                    curl_easy_setopt(curl, CURLOPT_USERNAME, username);

                    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &twurl_data_iob_reader);

                    twurl_data_iob *data_iob = calloc(1,sizeof(twurl_data_iob));

                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data_iob);

                    CURLcode re = curl_easy_perform(curl);

                    curl_easy_cleanup(curl);

                    free(username);

                    free(password);

                    if (CURLE_OK == re){

                        data_rec = twurl_data_rec_create(data_rec,data_iob);

                        twurl_data_iob_destroy(data_iob);

                        return true;
                    }
                    else {
                        twurl_data_iob_destroy(data_iob);
                    }
                }
                else {
                    free(username);

                    free(password);
                }
            }
            else {
                free(username);
            }
        }
    }
    return false;
}
/*
 * Employ bearer token to fetch URL to standard console
 * output.
 */
bool_t twurl_user_get(char *url){
    if (null != url){

        char *bearer = store_read_bearer();
        if (null != bearer){

            CURL *curl = curl_easy_init();
            if (null != curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_USERAGENT, TWURL_UA);

                curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

                curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

                curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);

                curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, bearer);

                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &twurl_data_iob_reader);

                twurl_data_iob *data_iob = calloc(1,sizeof(twurl_data_iob));

                curl_easy_setopt(curl, CURLOPT_WRITEDATA, data_iob);

                CURLcode re = curl_easy_perform(curl);

                curl_easy_cleanup(curl);

                free(bearer);

                if (CURLE_OK == re){

                    data_rec = twurl_data_rec_create(data_rec,data_iob);

                    twurl_data_iob_destroy(data_iob);

                    return true;
                }
                else {
                    twurl_data_iob_destroy(data_iob);
                }
            }
            else {
                free(bearer);
            }
        }
    }
    return false;
}
/*
 * Re/write 'bearer_token.txt'.
 */
bool_t twurl_update(){

    if (null != data_rec){

        twurl_data_rec_destroy(data_rec);

        data_rec = null;
    }

    if (twurl_app_post("grant_type=client_credentials","https://api.twitter.com/oauth2/token")){

        twurl_data_rec *record = twurl_data_rec_find(data_rec,"access_token");
        if (null != record){

            char *src = record->object;
            size_t len = strlen(src);

            if (store_write("bearer_token.txt",src,len)){

                twurl_data_rec_destroy(data_rec);

                data_rec = null;

                return true;
            }
            else {

                twurl_data_rec_destroy(data_rec);

                data_rec = null;
            }
        }
    }
    return false;
}
/*
 * Consolidate data.
 */
bool_t twurl_delete(int index){

    if (null != data_rec){
        off_t x = 0;

        twurl_data_rec *prev, *this;

        for (prev = null, this = data_rec; null != this; prev = this, x++, this = this->next){

            if (x == index){

                if (null == prev){

                    data_rec = this->next;
                }
                else {

                    prev->next = this->next;
                }

                twurl_data_rec_destroy(this);

                break;
            }
        }

        return true;
    }
    else {
        return false;
    }
}
/*
 * Output data.
 */
bool_t twurl_print(){

    if (null != data_rec){
        twurl_data_rec *dp;

        for (dp = data_rec; null != dp; dp = dp->next){

            fprintf(stdout,"%s %s\n",dp->identity,dp->object);
        }

        return true;
    }
    else {
        return false;
    }
}
/*
 * Write data.
 */
bool_t twurl_write(char *file){
    if (0 == chdir(twurl_work)){

        fd_t fd = open(file,(O_WRONLY|O_CREAT|O_TRUNC));
        if (-1 < fd){
            FILE *io = fdopen(fd,"w");
            if (null != io){

                twurl_data_rec *dp;

                for (dp = data_rec; null != dp; dp = dp->next){

                    fprintf(io,"%s %s\n",dp->identity,dp->object);
                }

                fclose(io);
            }
            close(fd);

            return true;
        }
    }
    return false;
}
/*
 * Output data value of named record.
 */
bool_t twurl_valueof(char *name){

    twurl_data_rec *record = twurl_data_rec_find(data_rec,name);
    if (null != record){

        fprintf(stdout,"%s\n",record->object);

        return true;
    }
    else {

        return false;
    }
}
/*
 * Read TWURL_STORE.
 */
bool_t twurl_init(){

    twurl_work = getcwd(null,0);

    twurl_store = getenv("TWURL_STORE");

    if (null != twurl_store){

        if (0 == chdir(twurl_store)){

            return true;
        }
    }
    return false;

}
