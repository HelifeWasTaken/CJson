/**
 * util/cjson_allocator.c
 * Copyright (c) 2022 Mattis Dalleau <mattis.dalleau@epitech.eu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson/cjson_util.h"

void* cjson_alloc(size_t size)
{
    void* ptr = calloc(size, 1);

    if (ptr == NULL)
        fprintf(stderr, "cjsonAllocator got NULL for a query of %lx size", size);
    return ptr;
}

void* cjson_realloc(void* ptr, size_t nsize)
{
    void* nptr = realloc(ptr, nsize);

    if (nptr == NULL)
        fprintf(stderr, "cjsonRealloc got NULL for a query of %lx size", nsize);
    return nptr;
}

void cjson_system_free(void* ptr)
{
    if (ptr)
        free(ptr);
}

char* cjson_strndup(char const* s, size_t size)
{
    char* n = strndup(s, size);

    if (n == NULL)
        fprintf(stderr, "cjsonStrndup got NULL for a query of %lx size", size);
    return n;
}
