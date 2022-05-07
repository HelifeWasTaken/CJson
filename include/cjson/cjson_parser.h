/**
 * include/cjson/cjson_parser.h
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
#ifndef cjson_PARSER_H
#define cjson_PARSER_H

#include "cjson_util.h"

struct cjson_parser_info {
    char* buf;
    size_t index;
    size_t line;
    size_t col;
};

bool cjson_parse_null(struct cjson* cjson, struct cjson_parser_info* parser);
bool cjson_parse_boolean(struct cjson* cjson, struct cjson_parser_info* parser);

bool cjson_parse_number(struct cjson* cjson, struct cjson_parser_info* parser);

bool cjson_parse_string(struct cjson* cjson, struct cjson_parser_info* parser);

bool cjson_parse_array(struct cjson* cjson, struct cjson_parser_info* parser);

bool cjson_parse_object(struct cjson* cjson, struct cjson_parser_info* parser);

bool cjson_parse_value(struct cjson* cjson, struct cjson_parser_info* parser);

void cjson_skip_withespaces(struct cjson_parser_info* parser);

#define cjson_PARSER_ERROR(pinfo, error)                                                          \
    {                                                                                            \
        fprintf(stderr, "Error at: [%ld:%ld] -> {%s}.\n", (pinfo)->line, (pinfo)->col, (error)); \
        return false;                                                                            \
    }

bool cjsonify(struct cjson* cjson, char* str);

char *stringify(struct cjson *cjson);

#endif
