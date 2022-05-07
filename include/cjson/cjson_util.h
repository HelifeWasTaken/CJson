/**
 * include/cjson/cjson_util.h
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
#ifndef cjson_UTIL_H
#define cjson_UTIL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void* cjson_alloc(size_t size);
void* cjson_realloc(void* ptr, size_t nsize);
char* cjson_strndup(char const* s, size_t size);
void cjson_system_free(void* ptr);

union cjson_value {
    char* s;
    double f;
    void* a;
    void* o;
    bool b;
    void* n;
};

enum cjson_type {
    cjson_NUM,
    cjson_STR,
    cjson_ARRAY,
    cjson_OBJECT,
    cjson_BOOL,
    cjson_NULL
};

struct cjson {
    union cjson_value v;
    enum cjson_type t;
};

struct cjson_tuple {
    char* key;
    struct cjson v;
};

#define MAKE_cjson_TUPLE(kv, vv) \
    (struct cjson_tuple)         \
    {                           \
        .key = kv, .v = vv      \
    }

struct cjson_object_bucket {
    struct cjson_tuple t;
    struct cjson_object_bucket* next;
};

struct cjson_object_bucket* cjson_create_object_bucket(const struct cjson_tuple* tuple);
bool cjson_insert_object_bucket(struct cjson_object_bucket** root, const struct cjson_tuple* tuple);

struct cjson_object {
    struct cjson_object_bucket** buckets;
    size_t size;
};

size_t cjson_hash(char const* s);
struct cjson_object* cjson_create_object(void);
bool cjson_insert_object(struct cjson_object** map, const struct cjson_tuple* tuple);

bool cjson_resize_if_needed_object(struct cjson_object** map);
bool cjson_object_resize(struct cjson_object** map);

void cjson_free_object(struct cjson_object** map);

struct cjson_array {
    size_t size;
    size_t a_size;
    struct cjson* values;
};

struct cjson_array* cjson_create_array(void);
bool cjson_append_array(struct cjson_array** array, const struct cjson* v);
void cjson_free_array(struct cjson_array** array);

void cjson_free(struct cjson* cjson);

#endif
