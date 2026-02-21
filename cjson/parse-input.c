#include "memory_tracker.h"
#include<stdlib.h>
#include "cJSON.h"

int parse_input_json(const char *json_text, int *variables, int **minterms, int *min_count, int **dontCares, int *dc_count)
{
    cJSON *root = cJSON_Parse(json_text);
    if (!root) return 1;

    cJSON *vars = cJSON_GetObjectItem(root, "var");
    cJSON *mins = cJSON_GetObjectItem(root, "minterms");

    if (!cJSON_IsNumber(vars) || !cJSON_IsArray(mins)) {
        cJSON_Delete(root);
        return 2;
    }

    *variables = vars->valueint;

    *min_count = cJSON_GetArraySize(mins);
    *minterms = malloc(sizeof(int) * (*min_count));

    for (int i = 0; i < *min_count; i++) {
        (*minterms)[i] =
            cJSON_GetArrayItem(mins, i)->valueint;
    }

    // dontCares optional
    cJSON *dc = cJSON_GetObjectItem(root, "dontCares");
    if (dc && cJSON_IsArray(dc)) {
        *dc_count = cJSON_GetArraySize(dc);
        *dontCares = malloc(sizeof(int) * (*dc_count));
        for (int i = 0; i < *dc_count; i++) {
            (*dontCares)[i] =
                cJSON_GetArrayItem(dc, i)->valueint;
        }
    } else {
        *dc_count = 0;
        *dontCares = NULL;
    }

    cJSON_Delete(root);
    return 0;
}
