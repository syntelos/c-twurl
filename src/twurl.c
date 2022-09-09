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
#include <stdlib.h>
#include <string.h>

static char* twurl_store = null;
/*
 * Employ api/consumer secret and bearer token to fetch URL
 * to standard console output.
 */
bool_t twurl_app_get(char *url){

    return false;
}
/*
 * Employ bearer token to fetch URL to standard console
 * output.
 */
bool_t twurl_user_get(char *url){

    return false;
}
/*
 * Re/write 'bearer_token.txt'.
 */
bool_t twurl_update(){

    return false;
}
/*
 * Read TWURL_STORE.
 */
bool_t twurl_init(){

    twurl_store = getenv("TWURL_STORE");

    if (null != twurl_store){

        return true;
    }
    else {
        return false;
    }
}
