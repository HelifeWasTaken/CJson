/**
 * parser/cjson_parse_bool.c
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
#include <string.h>

#include "cjson/cjson_parser.h"

bool cjson_parse_boolean(struct cjson* cjson, struct cjson_parser_info* parser)
{
    cjson_skip_withespaces(parser);
    if (strncmp(parser->buf + parser->index, "true", 4) != 0) {
        if (strncmp(parser->buf + parser->index, "false", 5) != 0) {
            return false;
        }
        parser->index += 5;
        parser->col += 5;
        cjson->v.b = false;
        cjson->t = cjson_BOOL;
    }
    parser->index += 4;
    parser->col += 4;
    cjson->v.b = true;
    cjson->t = cjson_BOOL;
    return true;
}
