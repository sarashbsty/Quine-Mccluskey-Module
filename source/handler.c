#ifdef _WIN32
#include <windows.h>
#endif

#include "memory_tracker.h"

#include <stdio.h>
#include <stdlib.h>
#include "qmMinimizer.h"
#include "cJSON.h"
#include "qmdata_to_json.h"
#include "parse_input.h"

static char *read_stdin();

int main()
{
	#ifdef _WIN32
	system("cls");
    SetConsoleOutputCP(CP_UTF8);
    #endif

	char *input = NULL , *errorMsg = NULL, *out;
	int var = 0, *minterms = NULL, minCount = 0 , *dontCare = NULL, dontCareCount = 0;

	cJSON *qmData_json = NULL;
	qmData data = {0};

	input = read_stdin();
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

	out = cJSON_Print(qmData_json);
	puts(out);

	free(out);
	cJSON_Delete(qmData_json);
	destroyQmData(&data);
	free(input);
	free(minterms);
	free(dontCare);

	return 0;

	FAIL:
		cJSON_Delete(qmData_json);
		destroyQmData(&data);

		free(input);
		free(minterms);
		free(dontCare);

		cJSON *error_json = cJSON_CreateObject();
		cJSON_AddNumberToObject(error_json , "error" , 1);
		cJSON_AddStringToObject(error_json , "errorMsg" , errorMsg);

		out = cJSON_Print(error_json);
		puts(out);

		free(out);
		cJSON_Delete(error_json);

		return 1;
}

static char *read_stdin() {
    size_t cap = 4096;
    size_t len = 0;
    char *buf = malloc(cap);
    if (!buf) return NULL;

    int c;
    while ((c = getchar()) != EOF) {
        buf[len++] = (char)c;

        if (len + 1 >= cap) {
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp) {
                free(buf);
                return NULL;
            }
            buf = tmp;
        }
    }

    buf[len] = '\0';
    return buf;
}

