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
#include <stdio.h>
#include <string.h>

typedef enum {
    main_op_unknown  = 0,
    main_op_update   = 1,
    main_op_app_get  = 2,
    main_op_user_get = 3

} main_op;
/*
 * Operators turn this bit down (to false) on error.
 */
bool_t main_state = true;

main_op main_operator(int argx, int argc, char **argv){
    if (-1 < argx && argx < argc && null != argv){
        char *arg = argv[argx];

        switch(*arg){
        case 'a':
            if (0 == strcmp("app",arg)){
                argx += 1;
                arg = argv[argx];

                if (0 == strcmp("get",arg)){

                    return main_op_app_get;
                }
            }
            break;
        case 'u':
            if (0 == strcmp("update",arg)){

                return main_op_update;
            }
            else if (0 == strcmp("user",arg)){
                argx += 1;
                arg = argv[argx];

                if (0 == strcmp("get",arg)){

                    return main_op_user_get;
                }
            }
            break;
        default:
            break;
        }
    }
    return main_op_unknown;
}

int main_update(int argx, int argc, char **argv){

    main_state = twurl_update();

    return 1;
}

int main_app_get(int argx, int argc, char **argv){

    main_state = twurl_app_get(argv[argx+3]);

    return 3;
}

int main_user_get(int argx, int argc, char **argv){

    main_state = twurl_user_get(argv[argx+3]);

    return 3;
}

void main_usage(int argc, char **argv){
    char *pn = argv[0];

    fprintf(stderr,"Synopsis\n\n\t%s app get <url>\n\t%s user get <url>\n\t%s app update\n\nDescription\n\n\tFetch URL to stdout.  Update bearer credentials.\n\n",pn,pn,pn);
}

int main(int argc, char **argv){

    if (1 < argc){
        int argx;

        for (argx = 1; main_state && argx < argc; ){

            switch (main_operator(argx,argc,argv)){

            case main_op_update:
                argx += main_update(argx,argc,argv);
                break;

            case main_op_app_get:
                argx += main_app_get(argx,argc,argv);
                break;

            case main_op_user_get:
                argx += main_user_get(argx,argc,argv);
                break;

            case main_op_unknown:
                main_usage(argc,argv);
                return 1;

            default:
                fprintf(stderr,"%s error recognizing input '%s'.\n",argv[0],argv[1]);
                return 1;
            }
        }

        if (main_state){

            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        main_usage(argc,argv);
        return 1;
    }
}
