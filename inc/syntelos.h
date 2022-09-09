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
#ifndef _syntelos_syntelos_h
#define _syntelos_syntelos_h
/*
 * As "syntelos/types", but avoiding "types". 
 */
#include <unistd.h>
#include <stdint.h>
/*
 * Boolean
 */
typedef uint8_t bool_t;

#define true 1
#define false 0
/*
 * Pointer
 */
#define null 0
/*
 * File descriptor
 */
typedef int fd_t;
/*
 * MMAP null
 */
#define mnul ((void*)-1)
/*
 * Arithmetic
 */
#define min(a,b) (a < b)?(a):(b)
#define max(a,b) (a > b)?(a):(b)

#endif
