/**
 * parser/cjson_parse_array.c
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
#include <string.h>

#include "cjson/cjson_parser.h"

static bool cjson_read_array(struct cjson* cjson, struct cjson_parser_info* parser, struct cjson_array** arr,
    struct cjson* tmp)
{
    while (parser->buf[parser->index]) {
        memset(tmp, 0, sizeof(struct cjson));
        cjson_skip_withespaces(parser);
        if (parser->buf[parser->index] == ']') {
            parser->col++;
            parser->index++;
            cjson->t = cjson_ARRAY;
            cjson->v.a = *arr;
            return true;
        } else if (parser->buf[parser->index] != ',') {
            cjson_PARSER_ERROR(parser, "Expected ',' after value [array]");
        }
        parser->col++;
        parser->index++;
        if (cjson_parse_value(tmp, parser) == false)
            return false;
        if (cjson_append_array(arr, tmp) == false)
            cjson_PARSER_ERROR(parser, "Mem alloc error in cjson_append_array")
    }
    cjson_PARSER_ERROR(parser, "Expected ']' to end the array");
}

static bool cjson_parse_array_internal(struct cjson* cjson, struct cjson_parser_info* parser, struct cjson_array** arr)
{
    struct cjson tmp = { 0 };

    if (cjson_parse_value(&tmp, parser) == false)
        return false;
    if (cjson_append_array(arr, &tmp) == false)
        cjson_PARSER_ERROR(parser, "Certainly Allocation error in cjson_append_array");
    return cjson_read_array(cjson, parser, arr, &tmp);
}

bool cjson_parse_array(struct cjson* cjson, struct cjson_parser_info* parser)
{
    struct cjson_array* arr = NULL;

    cjson_skip_withespaces(parser);
    if (parser->buf[parser->index] != '[')
        return false;
    parser->index++;
    parser->col++;
    cjson_skip_withespaces(parser);
    arr = cjson_create_array();
    if (arr == NULL)
        cjson_PARSER_ERROR(parser, "Allocation failed in cjson_parse_array");
    if (parser->buf[parser->index] == ']') {
        parser->index++;
        parser->col++;
        cjson->v.a = arr;
        cjson->t = cjson_ARRAY;
        return true;
    }
    return cjson_parse_array_internal(cjson, parser, &arr);
}
