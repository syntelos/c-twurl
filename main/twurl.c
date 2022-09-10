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
#include <stdio.h>
#include <string.h>

typedef enum {
    main_op_unknown  = 0,
    main_op_app_get  = 2,
    main_op_app_post = 3,
    main_op_user_get = 4,
    main_op_update   = 5,
    main_op_delete   = 6,
    main_op_print    = 7,
    main_op_write    = 8

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
                else if (0 == strcmp("post",arg)){

                    return main_op_app_post;
                }
            }
            break;
        case 'd':
            if (0 == strcmp("delete",arg)){

                return main_op_delete;
            }
            break;
        case 'p':
            if (0 == strcmp("print",arg)){

                return main_op_print;
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
        case 'w':
            if (0 == strcmp("write",arg)){

                return main_op_write;
            }
            break;
        default:
            break;
        }
    }
    return main_op_unknown;
}

int main_app_get(int argx, int argc, char **argv){

    argx += 2;

    if (argx < argc){

        main_state = twurl_app_get(argv[argx]);
    }
    else {
        main_state = false;
    }

    return 3;
}

int main_app_post(int argx, int argc, char **argv){

    argx += 2;

    if ((argx+1) < argc){

        main_state = twurl_app_post(argv[argx],argv[argx+1]);
    }
    else {
        main_state = false;
    }

    return 4;
}

int main_user_get(int argx, int argc, char **argv){

    argx += 2;

    if (argx < argc){

        main_state = twurl_user_get(argv[argx]);
    }
    else {
        main_state = false;
    }

    return 3;
}

int main_update(int argx, int argc, char **argv){

    main_state = twurl_update();

    return 1;
}

int main_delete(int argx, int argc, char **argv){

    argx += 1;

    if (argx < argc){

        char *arg = argv[argx];

        int iarg = atoi(arg);

        main_state = twurl_delete(iarg);
    }
    return 2;
}

int main_print(int argx, int argc, char **argv){

    main_state = twurl_print();

    return 1;
}

int main_write(int argx, int argc, char **argv){

    argx += 1;

    if (argx < argc){

        char *arg = argv[argx];

        main_state = twurl_write(arg);
    }
    return 2;
}

void main_usage(int argc, char **argv){
    char *pn = argv[0];

    fprintf(stderr,"Synopsis\n\n\t%s app get <url>\n\t%s app post <scope> <url>\n\t%s user get <url>\n\t%s app update\n\t%s delete <index>\n\t%s print\n\t%s write <file>\n\n",pn,pn,pn,pn,pn,pn,pn);

    fprintf(stderr,"Description\n\n\tFetch URL to internal data table.  Update bearer\n\tcredentials.  Produce data to console or file.\n\n\tThe user access employs the bearer token.  The app\n\taccess employs the API keys as well as the bearer\n\ttoken.\n\n");
}

int main(int argc, char **argv){

    if (1 < argc){

        if (twurl_init()){
            int argx;

            for (argx = 1; main_state && argx < argc; ){

                switch (main_operator(argx,argc,argv)){

                case main_op_app_get:
                    argx += main_app_get(argx,argc,argv);
                    break;

                case main_op_app_post:
                    argx += main_app_post(argx,argc,argv);
                    break;

                case main_op_user_get:
                    argx += main_user_get(argx,argc,argv);
                    break;

                case main_op_update:
                    argx += main_update(argx,argc,argv);
                    break;

                case main_op_unknown:
                    main_usage(argc,argv);
                    return 1;

                default:
                    fprintf(stderr,"%s error recognizing input '%s'.\n",argv[0],argv[argx]);
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
            fprintf(stderr,"%s error initializing TWURL_STORE (%s).\n",argv[0],getenv("TWURL_STORE"));
            return 1;
        }
    }
    else {
        main_usage(argc,argv);
        return 1;
    }
}
