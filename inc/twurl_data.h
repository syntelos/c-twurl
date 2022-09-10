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
#ifndef syntelos_twurl_data_h_
#define syntelos_twurl_data_h_

#include "twurl.h"
/*
 * 256 (1/4 KB)
 */
#define twurl_identity_size 0x100
/*
 * 4096 (4 KB)
 */
#define twurl_object_size 0x1000
/*
 * Object content in textual format.
 */
typedef char twurl_identity[twurl_identity_size];
/*
 * Object content in textual format.
 */
typedef char twurl_object[twurl_object_size];
/*
 * Object association to identity.
 */
typedef struct _twurl_data_rec {

    twurl_identity identity;

    twurl_object object;

    struct _twurl_data_rec *next;

} twurl_data_rec;
/*
 * Input buffer for libcurl write callback.
 */
typedef struct _twurl_data_iob {
    /*
     * Allocated dimension (extent).  Zero means null.
     */
    size_t size;
    /*
     * Employed dimension (end).  Zero means null.
     */
    off_t limit;
    /*
     * Material element.
     */
    char *content;

} twurl_data_iob;
/*
 * libcurl write_callback
 */
size_t twurl_data_iob_reader(char*, size_t, size_t, void*);

void twurl_data_iob_destroy(twurl_data_iob*);
/*
 * The "condensation" process flattens the JSON object into
 * an equivalent of an RDBMS table.  The JSON object name
 * and value are handled as identity and object of
 * association.
 */
twurl_data_rec* twurl_data_rec_create(twurl_data_rec*,twurl_data_iob*);

int twurl_data_rec_indexof(twurl_data_rec*,char*);

twurl_data_rec* twurl_data_rec_find(twurl_data_rec*,char*);

void twurl_data_rec_destroy(twurl_data_rec*);

#endif
