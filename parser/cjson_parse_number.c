/**
 * parser/cjson_parse_number.c
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
#include <ctype.h>

#include "cjson/cjson_parser.h"

static bool cjson_strtoll(char const** ptr, int64_t* value)
{
    if (isdigit(**ptr) == false)
        return (false);
    while (isdigit(**ptr)) {
        *value = *value * 10 + (**ptr - '0');
        (*ptr)++;
    }
    return (true);
}

static bool cjson_parse_exponent(char const** ptr, struct cjson* cjson)
{
    bool negative = false;
    int64_t value = 0;

    (*ptr)++;
    if (**ptr != '+' && **ptr != '-')
        return (false);
    if (**ptr == '-')
        negative = true;
    (*ptr)++;
    if (cjson_strtoll(ptr, &value) == false)
        return (false);
    for (int64_t i = 0; i < value; i++) {
        if (negative == true) {
            cjson->v.f /= 10;
        } else {
            cjson->v.f *= 10;
        }
    }
    return (true);
}

static bool cjson_parse_float_completely(struct cjson* cjson, char const** ptr)
{
    int64_t number = 0;
    int64_t decimal = 0;
    double divided = 0;

    if (cjson_strtoll(ptr, &number) == false)
        return (false);
    if (**ptr != '.')
        return (false);
    (*ptr)++;
    if (cjson_strtoll(ptr, &decimal) == false)
        return (false);
    divided = decimal;
    while (divided >= 10)
        divided /= 10;
    divided /= 10;
    cjson->v.f = number + divided;
    return (true);
}

static bool cjson_parse_integer(struct cjson* cjson, char const** buffer)
{
    char const* ptr = *buffer;
    bool minus = false;
    int64_t v = 0;

    if (*ptr == '-') {
        ptr++;
        minus = true;
    }
    if (cjson_strtoll(&ptr, &v) == false)
        return (false);
    cjson->v.f = v;
    if (*ptr == 'e' || *ptr == 'E')
        if (cjson_parse_exponent(&ptr, cjson) == false)
            return (false);
    if (minus == true)
        cjson->v.f = -cjson->v.f;
    *buffer = ptr;
    cjson->t = cjson_NUM;
    return (true);
}

static bool cjson_parse_number_internal(struct cjson* cjson, char const** buffer)
{
    char const* ptr = *buffer;
    bool minus = false;

    if (*ptr == '-') {
        ptr++;
        minus = true;
    }
    if (cjson_parse_float_completely(cjson, &ptr) == false)
        return (cjson_parse_integer(cjson, buffer));
    if (*ptr == 'e' || *ptr == 'E') {
        if (cjson_parse_exponent(&ptr, cjson) == false)
            return (false);
    }
    if (minus == true)
        cjson->v.f = -cjson->v.f;
    *buffer = ptr;
    cjson->t = cjson_NUM;
    return (true);
}

bool cjson_parse_number(struct cjson* cjson, struct cjson_parser_info* parser)
{
    char* ptr = NULL;

    cjson_skip_withespaces(parser);
    ptr = parser->buf + parser->index;
    if (cjson_parse_number_internal(cjson, (const char**)&ptr) == false)
        return false;
    parser->index += (ptr - parser->buf);
    parser->col += (ptr - parser->buf);
    return true;
}
