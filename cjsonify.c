/**
 * cjsonify.c
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
#include <stdlib.h>
#include <stdio.h>

#include "cjson/cjson_parser.h"

struct cjson_buffer {
    char* buf;
    size_t used;
    size_t allocated;
};

static const size_t BASE_cjson_BUFFER_SIZE = 8192;

struct cjson_stringify_escapes_equivalences {
    const char code;
    const char escape;
};

static const struct cjson_stringify_escapes_equivalences cjson_TOKENS_ESCAPES_EQUIVALENCES[] = {
    { '\"', '\"' }, { 'b', '\b' }, { 'f', '\f' }, { 'n', '\n' }, { 'r', '\r' }, { 't', '\t' }
};

static const size_t SIZE_cjson_ESCAPES_EQUIVALENCES_TOKENS_ESC = sizeof(cjson_TOKENS_ESCAPES_EQUIVALENCES) / sizeof(*cjson_TOKENS_ESCAPES_EQUIVALENCES);
static bool cjson_buffer_stringify_value(struct cjson_buffer* buf, struct cjson* value);

static struct cjson_buffer* make_cjson_buffer(void)
{
    struct cjson_buffer* buffer = malloc(sizeof(struct cjson_buffer));

    if (buffer == NULL)
        return NULL;
    buffer->buf = malloc(sizeof(char) * BASE_cjson_BUFFER_SIZE);
    if (buffer->buf == NULL) {
        free(buffer);
        return NULL;
    }
    buffer->used = 0;
    buffer->allocated = BASE_cjson_BUFFER_SIZE;
    return buffer;
}

static bool append_cjson_buffer(struct cjson_buffer* buf, char const* bytes, size_t len)
{
    if (buf->used + len >= buf->allocated) {
        buf->allocated = buf->allocated + ((len + buf->used) / BASE_cjson_BUFFER_SIZE + 1) * BASE_cjson_BUFFER_SIZE;
        buf->buf = realloc(buf->buf, buf->allocated);
        if (buf->buf == NULL) {
            return false;
        }
    }
    memcpy(buf->buf + buf->used, bytes, len);
    buf->used += len;
    return true;
}

static void destroy_cjson_buffer(struct cjson_buffer* buf)
{
    if (buf == NULL)
        return;
    free(buf->buf);
    free(buf);
}

static bool cjson_buffer_stringify_null(struct cjson_buffer* buf, struct cjson* cjson)
{
    (void)cjson;
    return append_cjson_buffer(buf, "null", 4);
}

static bool cjson_buffer_stringify_boolean(struct cjson_buffer* buf, struct cjson* cjson)
{
    if (cjson->v.b) {
        return append_cjson_buffer(buf, "true", 4);
    } else {
        return append_cjson_buffer(buf, "false", 5);
    }
}

static bool cjson_buffer_stringify_number(struct cjson_buffer* buf, struct cjson* cjson)
{
    char tmp[64];
    const int len = snprintf(tmp, 64, "%f", cjson->v.f);
    return len < 0 ? false : append_cjson_buffer(buf, tmp, len);
}

static bool cjson_buffer_stringify_string(struct cjson_buffer* buf, struct cjson* cjson)
{
    char tmp[2] = { 0 };
    size_t i = 0;
    size_t offset = 0;

    if (append_cjson_buffer(buf, "\"", 1) == false)
        return false;
    while (cjson->v.s[i]) {
        offset += strcspn(cjson->v.s + i, "\"\b\f\n\r\t");
        if (offset != i) {
            if (append_cjson_buffer(buf, cjson->v.s + i, offset - i) == false)
                return false;
            i = offset;
            continue;
        } else if (cjson->v.s[i] == '\0')
            break;
        for (size_t j = 0; j < SIZE_cjson_ESCAPES_EQUIVALENCES_TOKENS_ESC; j++) {
            if (cjson->v.s[i] == cjson_TOKENS_ESCAPES_EQUIVALENCES[j].code) {
                tmp[0] = cjson_TOKENS_ESCAPES_EQUIVALENCES[j].escape;
                if (append_cjson_buffer(buf, tmp, 1) == false)
                    return false;
                break;
            }
        }
        i += 1;
        offset = i;
    }
    return append_cjson_buffer(buf, "\"", 1);
}

static bool cjson_buffer_stringify_array(struct cjson_buffer* buf, struct cjson* cjson)
{
    struct cjson_array* array = cjson->v.a;

    if (append_cjson_buffer(buf, "[", 1) == false)
        return false;
    for (size_t i = 0; i < array->size; i++) {
        if (i != 0 && i + 1 != array->size && append_cjson_buffer(buf, ",", 1) == false)
            return false;
        if (cjson_buffer_stringify_value(buf, &array->values[i]) == false)
            return false;
    }
    return append_cjson_buffer(buf, "]", 1);
}

static bool cjson_buffer_stringify_object(struct cjson_buffer* buf, struct cjson* cjson)
{
    struct cjson_object* object = cjson->v.o;
    bool once = false;

    if (append_cjson_buffer(buf, "{", 1) == false)
        return false;
    for (size_t i = 0; i < object->size; i++) {
        for (struct cjson_object_bucket* bucket = object->buckets[i]; bucket; bucket = bucket->next) {
            if (once && append_cjson_buffer(buf, ",", 1) == false)
                return false;
            once = true;
            if (append_cjson_buffer(buf, "\"", 1) == false ||
                append_cjson_buffer(buf, bucket->t.key, strlen(bucket->t.key) == false ||
                append_cjson_buffer(buf, "\":", 2) == false ||
                cjson_buffer_stringify_value(buf, &bucket->t.v) == false)
                return false;
        }
    }
    return append_cjson_buffer(buf, "}", 1);
}

static bool cjson_buffer_stringify_value(struct cjson_buffer* buf, struct cjson* value)
{
    static bool (*stringify_func[])(struct cjson_buffer*, struct cjson*) = {
        cjson_buffer_stringify_number, cjson_buffer_stringify_string, cjson_buffer_stringify_array,
        cjson_buffer_stringify_object, cjson_buffer_stringify_boolean, cjson_buffer_stringify_null
    };
    return stringify_func[value->t](buf, value);
}

char* stringify(struct cjson* cjson)
{
    struct cjson_buffer* buffer = make_cjson_buffer();
    char *result = NULL;

    if (buffer == NULL)
        return NULL;
    if (cjson_buffer_stringify_value(buffer, cjson) == false || append_cjson_buffer(buf, "\0", 1) == false) {
        destroy_cjson_buffer(buffer);
        return NULL;
    }
    result = strdup(buffer->buf);
    destroy_cjson_buffer(buffer);
    return result;
}

bool cjsonify(struct cjson* cjson, char* str)
{
    return cjson_parse_value(cjson, &(struct cjson_parser_info) { str, 0, 0, 0 });
}

bool cjson_parse(struct cjson *cjson, char const *filename)
{
    char tmp[BUFSIZ] = {0};
    FILE *fp = NULL;
    struct cjson_buffer *buf = NULL;
    bool res = true;

    if ((fp = fopen(filename, "r")) == NULL) {
        return false;
    } else if ((buf = make_cjson_buffer()) == NULL) {
        fclose(fp);
        return false;
    }
    while (fgets(tmp, BUFSIZ, fp) != NULL) {
        res = append_cjson_buffer(buf, tmp, strlen(tmp));
        if (res == false)
            break;
    }
    if (res)
        res = cjsonify(cjson, buf->buf);
    destroy_cjson_buffer(buf);
    fclose(fp);
    return res;
}
