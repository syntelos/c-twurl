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
#ifndef syntelos_twurl_h_
#define syntelos_twurl_h_

#include "syntelos.h"

/*
 * Read data.  Return true on success.
 */
bool_t twurl_app_get(char*);
/*
 * Read data.  Return true on success.
 */
bool_t twurl_app_post(char*,char*);
/*
 * Read data.  Return true on success.
 */
bool_t twurl_user_get(char*);
/*
 * Fetch new bearer token to TWURL_STORE 'bearer_token.txt'.
 */
bool_t twurl_update();
/*
 * Consolidate data.
 */
bool_t twurl_delete(int index);
/*
 * Output data.
 */
bool_t twurl_print();
/*
 * Write data.
 */
bool_t twurl_write(char *file);
/*
 * Output data.
 */
bool_t twurl_valueof(char *name);
/*
 * Requires environment variable TWURL_STORE to define a
 * directory containing files "api_key.txt",
 * "api_key_secret.txt", and "bearer_token.txt".
 */
bool_t twurl_init();

#endif
