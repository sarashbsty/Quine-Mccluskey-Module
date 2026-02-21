#include<stdlib.h>
#include "memory.h"
#include "quine.h"
#include "cJSON.h"
#include "qmdata_to_json.h"

cJSON *primeData_to_json(const primeData *prime, int primeCount)
{
	if(!prime || !primeCount) return NULL;

	cJSON *primeArr = cJSON_CreateArray();
	if(!primeArr) goto FAIL;

	for(int i = 0; i < primeCount; i++)
	{
		cJSON *row = cJSON_CreateObject();
		if(!row) goto FAIL;

		// binary string
		cJSON_AddStringToObject(row, "binary", prime[i].binary);

		// expression string
		cJSON_AddStringToObject(row, "expression", prime[i].expression);

		// minterms array
		cJSON *minterms_arr = cJSON_AddArrayToObject(row, "minterms");
		if(!minterms_arr) goto FAIL;

		//add minterms to mintermArr
		for (int j = 0; j < prime[i].mintermCount; j++)
			cJSON_AddItemToArray(minterms_arr, cJSON_CreateNumber(prime[i].minterms[j]));

		//isEssential
		cJSON_AddBoolToObject(row , "isEssential" , prime[i].isEssential);

		//cost
		cJSON_AddNumberToObject(row , "cost" , prime[i].cost);

		cJSON_AddItemToArray(primeArr, row);
	}

	return primeArr;

	FAIL:
		cJSON_Delete(primeArr);
		return NULL;
}
