#include<stdlib.h>
#include "memory.h"
#include "quine.h"
#include "cJSON.h"
#include "qmdata_to_json.h"

cJSON *groupData_to_json(groupData **tables , int *groupSize, int tablesCount)
{
	if(!tables || !tablesCount) return NULL;

	cJSON *tableArr = cJSON_CreateArray();
	if(!tableArr) goto FAIL;

	for(int i = 0; i < tablesCount; i++)
	{
		cJSON *perTableObj = cJSON_CreateObject();
		if(!perTableObj) goto FAIL;

		cJSON *groupsArr = cJSON_AddArrayToObject(perTableObj, "groups");
		if(!groupsArr) goto FAIL;

		groupData *group = tables[i];
		int groupCount = groupSize[i];

		for(int x = 0; x < groupCount; x++)
		{
			if(!group[x].count) continue;

			cJSON *perGroupObj = cJSON_CreateObject();
			if (!perGroupObj) goto FAIL;

			cJSON *rows = cJSON_AddArrayToObject(perGroupObj, "rows");
			if(!rows) goto FAIL;

			for (int i = 0; i < group[x].count; i++)
			{
				cJSON *row = cJSON_CreateObject();
				if(!row) goto FAIL;

				cJSON_AddStringToObject(row, "binary", group[x].binary[i]);  // binary string

				cJSON *minterms_arr = cJSON_AddArrayToObject(row, "minterms");  // minterms array
				if(!minterms_arr) goto FAIL;

				cJSON_AddBoolToObject(row, "combined", group[x].isCombined[i]); // combined flag

				for (int j = 0; j < group[x].mintermCount[i]; j++)
					cJSON_AddItemToArray(minterms_arr, cJSON_CreateNumber(group[x].minterms[i][j])); //add minterms to mintermArr

				cJSON_AddItemToArray(rows, row);
			}

			cJSON_AddItemToArray(groupsArr, perGroupObj);
		}

		cJSON_AddItemToArray(tableArr, perTableObj);
	}

	return tableArr;

	FAIL:
		cJSON_Delete(tableArr);
		return NULL;
}
