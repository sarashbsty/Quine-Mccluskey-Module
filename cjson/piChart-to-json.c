#include<stdlib.h>
#include "memory.h"
#include "quine.h"
#include "cJSON.h"
#include "qmdata_to_json.h"

cJSON *piChart_to_json(int **piChart , int row, int col)
{
	cJSON *outer = cJSON_CreateArray();

    for (int i = 0; i < row; i++) {
        cJSON *row = cJSON_CreateArray();

		int *arr = piChart[i];
        for (int j = 0; j < col; j++)
            cJSON_AddItemToArray(row, cJSON_CreateNumber(arr[j]));

        cJSON_AddItemToArray(outer, row);
    }

    return outer;
}

