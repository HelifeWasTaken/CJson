/*
** EPITECH PROJECT, 2021
** cjsonobjectResize
** File description:
** resize
*/

#include <string.h>

#include "cjson/cjson_util.h"

static size_t cjson_MAX_SIZE_BEFORE_RESIZE_object = 3;

static bool cjson_object_resize_internal(struct cjson_object_bucket* bucket, struct cjson_object** n_map)
{
    struct cjson_tuple t = { 0 };

    for (; bucket; bucket = bucket->next) {
        t = MAKE_cjson_TUPLE(cjson_strndup(bucket->t.key, strlen(bucket->t.key)), bucket->t.v);
        if (t.key == NULL)
            return false;
        if (cjson_insert_object(n_map, &t) == false)
            return false;
    }
    return true;
}

bool cjson_object_resize(struct cjson_object** map)
{
    struct cjson_object* n_map = cjson_create_object();
    size_t n_size = (*map)->size + (*map)->size / 2;

    if (n_map == NULL)
        return false;
    n_map->buckets = cjson_alloc(sizeof(struct cjson_object_bucket*) * n_size);
    if (n_map->buckets == NULL)
        return false;
    n_map->size = n_size;
    for (size_t i = 0; i != (*map)->size; i++)
        if (cjson_object_resize_internal((*map)->buckets[i], &n_map) == false)
            return false;
    cjson_free_object(map);
    *map = n_map;
    return cjson_resize_if_needed_object(map);
}

bool cjson_resize_if_needed_object(struct cjson_object** map)
{
    size_t size = 0;

    for (size_t i = 0; i != (*map)->size; i++) {
        size = 0;
        for (struct cjson_object_bucket* buck = (*map)->buckets[i]; buck; buck = buck->next)
            size++;
        if (size >= cjson_MAX_SIZE_BEFORE_RESIZE_object)
            break;
    }
    if (size >= cjson_MAX_SIZE_BEFORE_RESIZE_object)
        return cjson_object_resize(map);
    return true;
}
