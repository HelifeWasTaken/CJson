/*
** EPITECH PROJECT, 2021
** cjsonobjectBucket
** File description:
** objectBucket
*/

#include <string.h>

#include "cjson/cjson_util.h"

struct cjson_object_bucket* cjson_create_object_bucket(const struct cjson_tuple* tuple)
{
    struct cjson_object_bucket* new = cjson_alloc(sizeof(struct cjson_object_bucket));

    if (new != NULL)
        new->t = *tuple;
    return new;
}

bool cjson_insert_object_bucket(struct cjson_object_bucket** root, const struct cjson_tuple* tuple)
{
    struct cjson_object_bucket* new = cjson_create_object_bucket(tuple);
    struct cjson_object_bucket* buck = *root;

    if (new == NULL)
        return false;
    if (*root == NULL)
        *root = new;
    for (; buck; buck = buck->next) {
        if (strcmp(buck->t.key, tuple->key) == 0) {
            cjson_system_free(tuple->key);
            cjson_free(&buck->t.v);
            buck->t.v = tuple->v;
            return true;
        }
    }
    new->next = *root;
    *root = new;
    return true;
}
