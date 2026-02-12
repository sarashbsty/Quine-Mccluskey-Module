#include <stdlib.h>
#include <math.h>
#include "quine.h"
#include "qmMinimizer.h"
#include "memory.h"

qmData qmMinimizer(int *minterms, int n_terms, int min_count, int var){

	qmData data = {0};

	//Allocate memory for group tables pointer array
	if(qmDataGroupAllocate(&data, var+1)){
		data.error = 1;
		data.errorMgs = "qmDataGroupAllocate Failed";
		return data;
	}

	//create initial Table
	quine *group = createGroupTable(minterms, n_terms, var);
	if(!group){
		data.error = 1;
		data.errorMgs = "createGroupTable failed";
		return data;
	}

	quine prime = {0};

	int canReduce = 0;
	do{
		quine *newGroup = calloc(var+1, sizeof(quine));
		if(!newGroup){
			data.error = 1;
			data.errorMgs = "Memory Allocation failed var: newGroup";
			return data;
		}

		//Reduction
		canReduce = reduce_table(group, newGroup, var);
		if(canReduce == -1){
			data.error = 1;
			data.errorMgs = "Reduce_table failed";
			return data;
		}
		else if(canReduce == 0) { free(newGroup); newGroup == NULL; }

		//get prime-implicants afer each reduction
		if(getPrimeImplicants(group, &prime, var)){
			data.error = 1;
			data.errorMgs = "getPrimeImplicants failed\n";
			return data;
		}

		quine *temp = group;
		group = newGroup;

		int idx = data.tableCount;
		data.groupTables[idx] = temp;
		data.groupCount[idx] = var+1;
		data.tableCount++;

	} while(canReduce);

	// prime_implicant_chart_table
	int **piChart = createPiChart(&prime, minterms, min_count, var);
	if(!piChart){
		data.error = 1;
		data.errorMgs = "Memory Allocation failed var: piChart";
		return data;
	}

	int *uncoveredTerms = malloc(min_count * sizeof(int));
	if(!uncoveredTerms){
		data.error = 1;
		data.errorMgs = "Memory Allocation failed var: uncoveredTerms";
		return data;
	}

	//Gather Uncovered Minterms if exist
	int uncoveredCount = 0;
	for(int i = 0; i < min_count; i++){
		int exist = 0;
		for(int j = 0; j < prime.count; j++){
			if(prime.minimal[j] == 0) continue;
			if(piChart[j][minterms[i]] != 0){
			 	exist = 1; break;
			}
		}
		if(!exist) uncoveredTerms[uncoveredCount++] = minterms[i];
	}

	if(uncoveredCount > 0)
	{

		char **setArr = malloc(uncoveredCount * sizeof(*setArr));
		if(!setArr){
			data.error = 1;
			data.errorMgs = "Memory Allocation failed var: setArr";
			return data;
		}

		int setArrCount = getSetCoverage(setArr, &prime, piChart, uncoveredTerms, uncoveredCount);
		if(setArrCount == -1){
			data.error = 1;
			data.errorMgs = "getSetCoverage failed";
			return data;
		}

		uncoveredCount = column_domination(setArr, &setArrCount, uncoveredTerms, uncoveredCount);

		if(petrick(&prime,setArr,setArrCount,var)){
			data.error = 1;
			data.errorMgs = "petrick failed";
			return data;
		}

		free_2d_pointer(setArr,setArrCount);
	}

	data.result = storeResult(&prime, var);

	free(uncoveredTerms);
	free_2d_pointer((char**)piChart , prime.count);
	clear_quine(&prime);
	free(minterms);
    return data;
}
