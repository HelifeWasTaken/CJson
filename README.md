# CJson

Simple and easy to use Json library in C

```c
// Loading a json from a file
int main(void)
{
    struct cjson json = {0};

    if (json_parse(&json, "filname.json") == false) {
        return 255;
    }
    destroy_cjson(&json);
    return 0;
}

// Loading a json from a string
int main(void)
{
    struct cjson json = {0};
    const char *json_string = "{\"key\":\"value\"}";

    if (jsonify(&json, json_string) == false) {
        return 255;
    }
    destroy_cjson(&json);
    return 0;
}

// Stringify a json
int main(void)
{
    struct cjson json = {0};
    const char *json_string = "{\"key\":\"value\"}";
    char *result = NULL;

    if (jsonify(&json, json_string) == false) {
        return 255;
    }
    result = json_stringify(&json);
    puts(result ? "Failed to stringify json" : result);
    free(result);
    destroy_cjson(&json);
}
```