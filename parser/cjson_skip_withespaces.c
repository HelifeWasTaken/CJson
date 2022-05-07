/**
 * parser/cjson_skip_withespaces.c
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
#include "cjson/cjson_parser.h"

void cjson_skip_withespaces(struct cjson_parser_info* parser)
{
    for (; parser->buf[parser->index]; parser->index++) {
        parser->col++;
        switch (parser->buf[parser->index]) {
        case '\n':
            parser->col = 0;
            parser->line++;
        case '\t':
        case ' ':
            continue;
        default:
            return;
        }
    }
}
