#pragma once
#include "quine.h"

typedef struct qmData qmData;

cJSON *qmData_to_json(const qmData *data);
cJSON *groupData_to_json(groupData **tables , int *groupSize, int tablesCount);
cJSON *primeData_to_json(const primeData *prime , int primeCount);
cJSON *piChart_to_json(int **piChart , int row, int col);
