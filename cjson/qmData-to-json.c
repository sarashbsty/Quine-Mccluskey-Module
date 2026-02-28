#include "qmMinimizer.h"
#include<stdlib.h>
#include<math.h>
#include "memory.h"
#include "quine.h"
#include "cJSON.h"
#include "qmdata_to_json.h"

cJSON *qmData_to_json(const qmData *data){
    cJSON *root = cJSON_CreateObject();
    if (!root) goto FAIL;

	//error
	cJSON_AddBoolToObject(root, "error" , data->error);

	//errorMsg
	cJSON_AddStringToObject(root , "errorMsg" , data->errorMsg);

	//variable
	cJSON_AddNumberToObject(root, "variable", data->var);

	//minterms
	cJSON *minterms = cJSON_CreateArray();
	if(!minterms) goto FAIL;
	else cJSON_AddItemToObject(root, "minterms" , minterms);

	for(int i = 0; i < data->minCount; i++){
		cJSON *num =  cJSON_CreateNumber(data->minterms[i]);
		if(!num) goto FAIL;
		else cJSON_AddItemToArray(minterms , num);
	}


	//dontCares
	cJSON *dontCares = cJSON_CreateArray();
	if(!dontCares) goto FAIL;
	else cJSON_AddItemToObject(root, "dontCares" , dontCares);

	for(int i = 0; i < data->dontCareCount; i++){
		cJSON *num =  cJSON_CreateNumber(data->dontCares[i]);
		if(!num) goto FAIL;
		else cJSON_AddItemToArray(dontCares , num);
	}


	//Tables
	cJSON *tables = Table_to_json(data->tables , data->tablesCount);
	if(!tables) goto FAIL;
	else cJSON_AddItemToObject(root, "tables" , tables);


	//prime implicants
	cJSON *prime = primeData_to_json(data->prime , data->primeCount);
	if(!prime) goto FAIL;
	else cJSON_AddItemToObject(root, "primeImplicants" , prime);


	//prime implicants chart
	cJSON *piChart = piChart_to_json(data->piChart, data->primeCount, pow(2,data->var));
	if(!piChart) goto FAIL;
	else cJSON_AddItemToObject(root, "piChart" , piChart);


	//essential implicants
	cJSON *essentialPi = cJSON_CreateArray();
	if(!essentialPi) goto FAIL;
	else cJSON_AddItemToObject(root, "essentialPi" , essentialPi);

	for(int i = 0; i < data->essentialCount; i++){
		cJSON *str = cJSON_CreateString(data->essential[i]);
		if(!str) goto FAIL;
		else cJSON_AddItemToArray(essentialPi, str);
	}


	//uncovered minterms
	cJSON *uncoveredTerms = cJSON_CreateArray();
	if(!uncoveredTerms) goto FAIL;
	else cJSON_AddItemToObject(root, "uncoveredTerms" , uncoveredTerms);

	for(int i = 0; i < data->uncoveredCount; i++){
		cJSON *num =  cJSON_CreateNumber(data->uncoveredTerms[i]);
		if(!num) goto FAIL;
		else cJSON_AddItemToArray(uncoveredTerms , num);
	}


	//reduced uncovered minterms
	cJSON *newUncoveredTerms = cJSON_CreateArray();
	if(!newUncoveredTerms) goto FAIL;
	else cJSON_AddItemToObject(root, "newUncoveredTerms" , newUncoveredTerms);

	for(int i = 0; i < data->newUncoveredCount; i++){
		cJSON *num =  cJSON_CreateNumber(data->newUncoveredTerms[i]);
		if(!num) goto FAIL;
		else cJSON_AddItemToArray(newUncoveredTerms , num);
	}

	//set representation
	cJSON *set = cJSON_CreateArray();
	if(!set) goto FAIL;
	else cJSON_AddItemToObject(root, "set" , set);

	for(int i = 0; i < data->setCount; i++){
		cJSON *str = cJSON_CreateString(data->set[i]);
		if(!str) goto FAIL;
		else cJSON_AddItemToArray(set, str);
	}


	//petrick distribution and absoption steps
	cJSON *process = cJSON_CreateArray();
	if(!process) goto FAIL;
	else cJSON_AddItemToObject(root, "process" , process);


	//petrick final SOP terms
	cJSON *SOP_terms = cJSON_CreateArray();
	if(!SOP_terms) goto FAIL;
	else cJSON_AddItemToObject(root, "sopTerms" , SOP_terms);


	//combinations and cost
	cJSON *combinations = cJSON_CreateArray();
	if(!combinations) goto FAIL;
	else cJSON_AddItemToObject(root, "combinations" , combinations);

	if(data->petrick)
	{
		// adding process strings
		for(int i = 0; i < data->petrick->processCount; i++){
			cJSON *str = cJSON_CreateString(data->petrick->process[i]);
			if(!str) goto FAIL;
			else cJSON_AddItemToArray(process, str);
		}

		//adding SOP terms
		for(int i = 0; i < data->petrick->SOP_count; i++){
			cJSON *str = cJSON_CreateString(data->petrick->SOP_terms[i]);
			if(!str) goto FAIL;
			else cJSON_AddItemToArray(SOP_terms, str);
		}

		//adding combination and cost
		for(int i = 0; i < data->petrick->combiCount; i++){
			cJSON *perCombinations = cJSON_CreateArray();
			if(!perCombinations) goto FAIL;
			else cJSON_AddItemToArray(combinations, perCombinations);

			for(int j = 0; j < data->petrick->combinations[i].termsCount; j++){
				cJSON *str = cJSON_CreateString(data->petrick->combinations[i].terms[j]);
				if(!str) goto FAIL;
				else cJSON_AddItemToArray(perCombinations, str);
			}

			cJSON *num = cJSON_CreateNumber(data->petrick->combinations[i].cost);
			if(!num) goto FAIL;
			else cJSON_AddItemToArray(perCombinations, num);
		}

		//index of the combination with minimum cost in combinations(array)
		cJSON *n = cJSON_CreateNumber(data->petrick->minCostIdx);
		if(!n) goto FAIL;
		else cJSON_AddItemToObject(root, "minCostIdx" , n);
	}

	//final result
	cJSON *result = cJSON_CreateArray();
	if(!result) goto FAIL;
	else cJSON_AddItemToObject(root, "result" , result);

	for(int i = 0; i < data->resultCount; i++){
		cJSON *str =  cJSON_CreateString(data->result[i]);
		if(!str) goto FAIL;
		else cJSON_AddItemToArray(result, str);
	}

	return root;

	FAIL:
		cJSON_Delete(root);
		return NULL;
}

