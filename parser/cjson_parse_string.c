/**
 * parser/cjson_parse_string.c
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

struct cjson_parser_escapes_equivalences {
    const char code;
    const char escape;
};

static const struct cjson_parser_escapes_equivalences cjson_TOKENS_ESCAPES_EQUIVALENCES[] = {
    { '\"', '\"' }, { 'b', '\b' }, { 'f', '\f' }, { 'n', '\n' }, { 'r', '\r' }, { 't', '\t' }
};

static const size_t SIZE_cjson_ESCAPES_EQUIVALENCES_TOKENS_ESC = sizeof(cjson_TOKENS_ESCAPES_EQUIVALENCES) / sizeof(*cjson_TOKENS_ESCAPES_EQUIVALENCES);

static char get_escape_code(char const c)
{
    for (size_t i = 0; i < SIZE_cjson_ESCAPES_EQUIVALENCES_TOKENS_ESC; i++)
        if (cjson_TOKENS_ESCAPES_EQUIVALENCES[i].code == c)
            return cjson_TOKENS_ESCAPES_EQUIVALENCES[i].escape;
    return c;
}

static void cjson_transform_string_escapes(struct cjson* cjson, struct cjson_parser_info* parser)
{
    size_t index = 0;
    size_t offset = 0;

    for (; cjson->v.s[index + offset]; index++) {
        if (cjson->v.s[index + offset] == '\\' && cjson->v.s[index + offset + 1] != 'u') {
            cjson->v.s[index] = get_escape_code(cjson->v.s[index + offset]);
            offset++;
            parser->col += 2;
        } else {
            cjson->v.s[index] = cjson->v.s[index + offset];
            parser->col++;
        }
        if (cjson->v.s[index] == '\n') {
            parser->col = 0;
            parser->line++;
        }
    }
    cjson->v.s[index] = '\0';
}

// Last false should never be reached
// it is just there so gcc does not comaplain
static bool cjson_parse_string_internal(char** ptr)
{
    char* tmp = *ptr;

    (*ptr)++;
    while (1) {
        tmp = strchr(tmp, '\"');
        if (tmp == NULL)
            return false;
        if (*(tmp - 1) == '\\')
            continue;
        *ptr = tmp;
        return true;
    }
    return false;
}

bool cjson_parse_string(struct cjson* cjson, struct cjson_parser_info* parser)
{
    char* ptr = NULL;
    char* original_ptr = NULL;
    char* res = NULL;

    cjson_skip_withespaces(parser);
    ptr = parser->buf + parser->index;
    original_ptr = ptr;
    if (*ptr != '\"')
        return false;
    if (cjson_parse_string_internal(&ptr) == false)
        cjson_PARSER_ERROR(parser, "Unmatched \" character or invalid string");
    res = cjson_strndup(original_ptr, ptr - original_ptr);
    if (res == NULL)
        cjson_PARSER_ERROR(parser, "Allocation failed in cjson_parse_string");
    parser->index += ptr - original_ptr;
    cjson->v.s = res;
    cjson->t = cjson_STR;
    cjson_transform_string_escapes(cjson, parser);
    return true;
}
