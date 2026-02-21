#include "memory_tracker.h"

#include <stdio.h>
#include <stdlib.h>
#include "qmMinimizer.h"
#include "cJSON.h"
#include "qmdata_to_json.h"
#include "parse_input.h"
#include "qm_run.h"

char *qm_run(const char *input)
{
    char *errorMsg = NULL;
	int var = 0, *minterms = NULL, minCount = 0 , *dontCare = NULL, dontCareCount = 0;

	cJSON *qmData_json = NULL;
	qmData data = {0};

	if(!input){
		errorMsg =  "Failed to read input";
		goto FAIL;
	}

	int error = parse_input_json(input, &var, &minterms, &minCount, &dontCare, &dontCareCount);
	if(error == 1){
		errorMsg = "Parsing input failed due to invalid json";
		goto FAIL;
	}
	else if(error == 2){
		errorMsg = "Some input variable not Found";
		goto FAIL;
	}
	else if(error == 3){
		errorMsg = "memory Allocation fail";
		goto FAIL;
	}
	else if(error == 4){
		errorMsg = "Invalid Datatypes Found";
		goto FAIL;
	}

	int *tmp = realloc(minterms, (minCount + dontCareCount) * sizeof(*tmp));
	if(!tmp) goto FAIL;
	minterms = tmp;

	//inserting dontcare elements at the end of minterms
	for(int i = 0; i < dontCareCount; i++)
		minterms[minCount + i] = dontCare[i];

	//call minimizer
	data = qmMinimizer(minterms, minCount, dontCareCount, var);
	if(data.error){
		errorMsg = data.errorMsg;
		goto FAIL;
	}

	//convert to json
	qmData_json = qmData_to_json(&data);
	if(!qmData_json){
		errorMsg = "cJSON Output Failed";
		goto FAIL;
	}

	char *out = cJSON_Print(qmData_json);

	cJSON_Delete(qmData_json);
	destroyQmData(&data);
	free(minterms);
	free(dontCare);

	return out;

	FAIL:
		cJSON_Delete(qmData_json);
		destroyQmData(&data);

		free(minterms);
		free(dontCare);

		cJSON *error_json = cJSON_CreateObject();
		cJSON_AddBoolToObject(error_json , "error" , 1);
		cJSON_AddStringToObject(error_json , "errorMsg" , errorMsg);

		out = cJSON_Print(error_json);
		cJSON_Delete(error_json);

		return out;
}
