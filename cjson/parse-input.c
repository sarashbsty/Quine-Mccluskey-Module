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

	if (*min_count > 0)
	{
		*minterms = malloc(sizeof(int) * (*min_count));
		if (!*minterms) { cJSON_Delete(root); return 3; }

		for (int i = 0; i < *min_count; i++)
		{
			cJSON *item = cJSON_GetArrayItem(mins, i);
			if (!cJSON_IsNumber(item)) { cJSON_Delete(root); return 4; }
			(*minterms)[i] = item->valueint;
		}
	}
	else *minterms = NULL;

    // dontCares optional
    cJSON *dc = cJSON_GetObjectItem(root, "dontCares");
    if (dc && cJSON_IsArray(dc))
	{
		*dc_count = cJSON_GetArraySize(dc);
		if (*dc_count > 0)
		{
			*dontCares = malloc(sizeof(int) * (*dc_count));
			if (!*dontCares) { cJSON_Delete(root); return 3; }

			for (int i = 0; i < *dc_count; i++)
			{
				cJSON *item = cJSON_GetArrayItem(dc, i);
				if (!cJSON_IsNumber(item)){ cJSON_Delete(root); return 4; }
				(*dontCares)[i] = item->valueint;
			}
		}
		else *dontCares = NULL;
    }
	else
	{
		*dc_count = 0;
		*dontCares = NULL;
	}

    cJSON_Delete(root);
    return 0;
}
