/**
 * util/array/cjson_append_array.c
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
#include "cjson/cjson_util.h"

static size_t cjson_BASE_ARRAY_SIZE = 5;

static bool cjson_append_array_first_alloc(struct cjson_array** array, const struct cjson* v)
{
    (*array)->values = cjson_alloc(sizeof(struct cjson) * cjson_BASE_ARRAY_SIZE);
    if ((*array)->values == NULL)
        return false;
    (*array)->a_size = cjson_BASE_ARRAY_SIZE;
    (*array)->size = 1;
    (*array)->values[0] = *v;
    return true;
}

static bool cjson_append_array_second_alloc(struct cjson_array** array, const struct cjson* v)
{
    size_t new_size = (*array)->a_size + (*array)->a_size / 2;

    if ((*array)->size == (*array)->a_size) {
        (*array)->values = cjson_realloc((*array)->values, sizeof(struct cjson) * new_size);
        if ((*array)->values == NULL)
            return false;
        (*array)->a_size = new_size;
    }
    (*array)->values[(*array)->size++] = *v;
    return true;
}

bool cjson_append_array(struct cjson_array** array, const struct cjson* v)
{
    return (*array)->size == 0 ? cjson_append_array_first_alloc(array, v) : cjson_append_array_second_alloc(array, v);
}
