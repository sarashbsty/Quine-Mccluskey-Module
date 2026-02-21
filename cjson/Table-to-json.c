#include<stdlib.h>
#include "memory.h"
#include "quine.h"
#include "cJSON.h"
#include "qmdata_to_json.h"

cJSON *Table_to_json(Table *tables , int tablesCount)
{
	if(!tables || !tablesCount) return NULL;

	cJSON *tableArr = cJSON_CreateArray();
	if(!tableArr) goto FAIL;

	for(int i = 0; i < tablesCount; i++)
	{
		cJSON *perTableObj = cJSON_CreateObject();
		if(!perTableObj) goto FAIL;

		cJSON *groupsJson = cJSON_AddArrayToObject(perTableObj, "groups");
		if(!groupsJson) goto FAIL;

		Group *groups = tables[i].groups;
		int groupCount = tables[i].count;

		for(int x = 0; x < groupCount; x++)
		{
			if(!groups[x].count) continue;

			cJSON *perGroupObj = cJSON_CreateObject();
			if (!perGroupObj) goto FAIL;

			cJSON *implicants = cJSON_AddArrayToObject(perGroupObj, "implicants");
			if(!implicants) goto FAIL;

			for (int i = 0; i < groups[x].count; i++)
			{
				cJSON *imp = cJSON_CreateObject();
				if(!imp) goto FAIL;

				cJSON_AddStringToObject(imp, "binary", groups[x].implicants[i].binary);  // binary string

				cJSON *minterms_arr = cJSON_AddArrayToObject(imp, "minterms");  // minterms array
				if(!minterms_arr) goto FAIL;

				cJSON_AddBoolToObject(imp, "combined", groups[x].implicants[i].isCombined); // combined flag

				for (int j = 0; j < groups[x].implicants[i].mintermCount; j++)
					cJSON_AddItemToArray(minterms_arr, cJSON_CreateNumber(groups[x].implicants[i].minterms[j])); //add minterms to mintermArr

				cJSON_AddItemToArray(implicants, imp);
			}

			cJSON_AddItemToArray(groupsJson, perGroupObj);
		}

		cJSON_AddItemToArray(tableArr, perTableObj);
	}

	return tableArr;

	FAIL:
		cJSON_Delete(tableArr);
		return NULL;
}
