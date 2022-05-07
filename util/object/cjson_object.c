/*
** EPITECH PROJECT, 2021
** cjsonobject
** File description:
** object
*/

#include "cjson/cjson_util.h"

static size_t cjson_object_BASIC_SIZE = 10;

size_t cjson_hash(char const* s)
{
    size_t res = 0;

    for (; *s; s++)
        res = *s + (*s ^ res) + (*s << 3) + res / 2 - (res >> 1);
    return res;
}

struct cjson_object* cjson_create_object(void)
{
    return cjson_alloc(sizeof(struct cjson_object));
}

bool cjson_insert_object(struct cjson_object** map, const struct cjson_tuple* tuple)
{
    size_t hash = 0;

    if (*map == NULL) {
        *map = cjson_create_object();
        if (*map == NULL)
            return false;
    }
    if ((*map)->size == 0) {
        (*map)->buckets = cjson_alloc(sizeof(struct cjson_object_bucket*) * cjson_object_BASIC_SIZE);
        (*map)->size = cjson_object_BASIC_SIZE;
    }
    hash = cjson_hash(tuple->key) % (*map)->size;
    if (cjson_insert_object_bucket(&(*map)->buckets[hash], tuple) == false)
        return false;
    return cjson_resize_if_needed_object(map);
}
