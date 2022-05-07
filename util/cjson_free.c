/**
 * util/cjson_free.c
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

void cjson_free(struct cjson* cjson)
{
    switch (cjson->t) {
    case cjson_STR:
        cjson_system_free(cjson->v.s);
        break;
    case cjson_OBJECT:
        cjson_free_object((struct cjson_object**)&cjson->v.o);
        break;
    case cjson_ARRAY:
        cjson_free_array((struct cjson_array**)&cjson->v.a);
        break;
    case cjson_NUM:
    case cjson_BOOL:
    case cjson_NULL:
        break;
    }
}
