/*
** EPITECH PROJECT, 2021
** FreecjsonObject
** File description:
** Free
*/

#include "cjson/cjson_util.h"

static void cjson_free_buck(struct cjson_object_bucket** root)
{
    if (*root == NULL)
        return;
    cjson_free_buck(&(*root)->next);
    cjson_system_free((*root)->t.key);
    cjson_free(&(*root)->t.v);
}

void cjson_free_object(struct cjson_object** map)
{
    if (*map == NULL)
        return;
    for (size_t i = 0; i != (*map)->size; i++) {
        cjson_free_buck(&(*map)->buckets[i]);
    }
    cjson_system_free((*map)->buckets);
    cjson_system_free(*map);
}
