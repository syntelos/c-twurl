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
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

static char* twurl_store = null;

char *store_read(char *file){
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
    return null;
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
 * Employ api/consumer secret and bearer token to fetch URL
 * to standard console output.
 */
bool_t twurl_app_get(char *url){
    if (null != url){

        char *username = store_read_username();
        if (null != username){
            char *password = store_read_password();
            if (null != password){
                char *bearer = store_read_bearer();
                if (null != bearer){

                    CURL *curl = curl_easy_init();
                    if (null != curl) {
                        curl_easy_setopt(curl, CURLOPT_URL, url);

                        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);

                        curl_easy_setopt(curl, CURLOPT_USERNAME, username);

                        curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

                        curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, bearer);

                        CURLcode re = curl_easy_perform(curl);

                        curl_easy_cleanup(curl);

                        free(username);

                        free(password);

                        free(bearer);

                        if (CURLE_OK == re){

                            return true;
                        }
                    }
                    else {
                        free(username);

                        free(password);

                        free(bearer);
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

                curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);

                curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, bearer);

                CURLcode re = curl_easy_perform(curl);

                curl_easy_cleanup(curl);

                free(bearer);

                if (CURLE_OK == re){

                    return true;
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

    return false;
}
/*
 * Read TWURL_STORE.
 */
bool_t twurl_init(){

    twurl_store = getenv("TWURL_STORE");

    if (null != twurl_store){

        if (0 == chdir(twurl_store)){

            return true;
        }
    }
    return false;

}