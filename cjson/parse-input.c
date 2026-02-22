#include "memory_tracker.h"
#include<stdlib.h>
#include "cJSON.h"

int parse_input_json(const char *json_text, int *varPtr, int **mintermsPtr, int *min_countPtr, int **dontCaresPtr, int *dc_countPtr)
{
	int error = 0, *minterms = NULL, *dontCares = NULL;
	int var, min_count, dc_count;

    cJSON *root = cJSON_Parse(json_text);
    if (!root){ error = 1; goto FAIL; }

    cJSON *vars = cJSON_GetObjectItem(root, "var");
    cJSON *mins = cJSON_GetObjectItem(root, "minterms");

    if (!cJSON_IsNumber(vars) || !cJSON_IsArray(mins))
	{ error = 2; goto FAIL; }

    var = vars->valueint;
	min_count = cJSON_GetArraySize(mins);

	if (min_count > 0)
	{
		minterms = malloc(sizeof(int) * min_count);
		if (!minterms) { error = 3; goto FAIL; }

		for (int i = 0; i < min_count; i++)
		{
			cJSON *item = cJSON_GetArrayItem(mins, i);
			if (!cJSON_IsNumber(item)) { error = 4; goto FAIL; }
			minterms[i] = item->valueint;
		}
	}
	else minterms = NULL;

    // dontCares optional
    cJSON *dc = cJSON_GetObjectItem(root, "dontCares");
    if (dc && cJSON_IsArray(dc))
	{
		dc_count = cJSON_GetArraySize(dc);
		if (dc_count > 0)
		{
			dontCares = malloc(sizeof(int) * dc_count);
			if (!dontCares) { error = 3; goto FAIL; }

			for (int i = 0; i < dc_count; i++)
			{
				cJSON *item = cJSON_GetArrayItem(dc, i);
				if (!cJSON_IsNumber(item)){ error = 4; goto FAIL; }
				dontCares[i] = item->valueint;
			}
		}
		else dontCares = NULL;
    }
	else
	{
		dc_count = 0;
		dontCares = NULL;
	}

    cJSON_Delete(root);

	*varPtr = var;
	*mintermsPtr = minterms;
	*dontCaresPtr = dontCares;
	*min_countPtr = min_count;
	*dc_countPtr = dc_count;

    return 0;

	FAIL:
		cJSON_Delete(root);
		free(minterms);
		free(dontCares);

		if(!error) error = 1;

		*mintermsPtr = NULL;
		*dontCaresPtr = NULL;
		return error;
}
