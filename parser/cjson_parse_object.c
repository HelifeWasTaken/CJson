/**
 * parser/cjson_parse_object.c
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
#include <cjson/cjson_parser.h>
#include <stdio.h>

static bool cjson_parse_object_read_data(struct cjson_object** map, struct cjson_parser_info* parser)
{
    struct cjson key = { 0 };
    struct cjson value = { 0 };

    cjson_skip_withespaces(parser);
    if (cjson_parse_string(&key, parser) == false)
        return false;
    cjson_skip_withespaces(parser);
    if (parser->buf[parser->index] != ':')
        cjson_PARSER_ERROR(parser, "Parser could not find \":\" character after a string"
                                  " in an object to terminate the key");
    parser->index++;
    parser->col++;
    cjson_skip_withespaces(parser);
    if (cjson_parse_value(&value, parser) == false)
        return false;
    if (cjson_insert_object(map, &MAKE_cjson_TUPLE(key.v.s, value)) == false)
        cjson_PARSER_ERROR(parser, "Certainly allocation error at cjson insert object");
    return true;
}

static bool cjson_parse_object_loop(struct cjson* cjson, struct cjson_parser_info* parser, struct cjson_object** map)
{
    while (parser->buf[parser->index]) {
        cjson_skip_withespaces(parser);
        if (parser->buf[parser->index] == '}') {
            cjson->t = cjson_OBJECT;
            cjson->v.o = *map;
            parser->index++;
            parser->col++;
            return true;
        } else if (parser->buf[parser->index] != ',') {
            cjson_PARSER_ERROR(parser, "Expected ',' after value [object]");
        }
        parser->index++;
        parser->col++;
        cjson_skip_withespaces(parser);
        if (cjson_parse_object_read_data(map, parser) == false)
            return false;
    }
    cjson_PARSER_ERROR(parser, "Expected '}' at the end of the object");
}

bool cjson_parse_object(struct cjson* cjson, struct cjson_parser_info* parser)
{
    struct cjson_object* map = NULL;

    cjson_skip_withespaces(parser);
    if (parser->buf[parser->index] != '{')
        return false;
    parser->index++;
    parser->col++;
    cjson_skip_withespaces(parser);
    map = cjson_create_object();
    if (map == NULL)
        cjson_PARSER_ERROR(parser, "Allocation failed in cjson_parse_object");
    if (parser->buf[parser->index] == '}') {
        parser->index++;
        parser->col++;
        cjson->v.o = map;
        cjson->t = cjson_OBJECT;
        return true;
    }
    return cjson_parse_object_read_data(&map, parser) == true ? cjson_parse_object_loop(cjson, parser, &map) : false;
}
