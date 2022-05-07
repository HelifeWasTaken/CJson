/**
 * parser/cjson_parse_value.c
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

#include "cjson/cjson_parser.h"

struct cjson_parsers {
    bool (*parser)(struct cjson*, struct cjson_parser_info*);
};

static const struct cjson_parsers cjson_PARSERS[] = { { cjson_parse_object }, { cjson_parse_array }, { cjson_parse_string },
    { cjson_parse_number }, { cjson_parse_boolean }, { cjson_parse_null } };

static const size_t cjson_PARSERS_COUNT = sizeof(cjson_PARSERS) / sizeof(*cjson_PARSERS);

bool cjson_parse_value(struct cjson* cjson, struct cjson_parser_info* parser)
{
    cjson_skip_withespaces(parser);
    if (parser->buf[parser->index] == '\0')
        cjson_PARSER_ERROR(parser, "Unexpectedly reached the end of cjson."
                                  " cjson might be empty or unterminated");
    for (size_t i = 0; i < cjson_PARSERS_COUNT; i++) {
        if (cjson_PARSERS[i].parser(cjson, parser) == true)
            return true;
    }
    cjson_PARSER_ERROR(parser, "Unexpected characters or format");
}
