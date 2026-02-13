#include <stdlib.h>
#include <math.h>
#include "quine.h"
#include "qmMinimizer.h"
#include "memory.h"
#include "int_array_dup.h"
#include "petrick.h"

qmData qmMinimizer(int *minterms, int n_terms, int minCount, int var){

	qmData data = {0};

	//Allocate memory for group tables pointer array
	if(qmDataGroupAllocate(&data, var+1)){
		data.error = 1;
		data.errorMgs = "qmDataGroupAllocate Failed";
		return data;
	}

	quine *group = calloc(var+1, sizeof(quine));
	if(!group){
		data.error = 1;
		data.errorMgs = "Memory Allocation failed var: Group";
		return data;
	}

	if(createGroupTable(group, minterms, n_terms, var)){
		data.error = 1;
		data.errorMgs = "createGroupTable failed";
		return data;
	}

	quine prime = {0};

	//Table Reduction Process
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

		//store each group Table
		int idx = data.tableCount;
		data.groupTables[idx] = group;
		data.groupCount[idx] = var+1;
		data.tableCount++;

		group = newGroup;

	} while(canReduce);

	data.PI = prime;

	// prime_implicant_chart_table
	int **piChart = createPiChart(&prime, minterms, minCount, var);
	if(!piChart){
		data.error = 1;
		data.errorMgs = "Memory Allocation failed var: piChart";
		return data;
	}
	data.piChart = piChart;

	//store Essential Implicants
	data.essentialCount = getEssentialPi(&data.essentialPi, &prime);
	if(data.essentialCount == -1){
		data.error = 1;
		data.errorMgs = "getEssentialPi failed";
		return data;
	}

	//Get uncovered Minterms if exist
	int *uncoveredTerms = NULL;
	int uncoveredCount = getUncovered(&uncoveredTerms, &prime, piChart, minterms, minCount);
	if(uncoveredCount == -1){
		data.error = 1;
		data.errorMgs = "getUncovered failed";
		return data;
	}

	if(uncoveredCount > 0){

		//Store Uncovered minterms
		data.uncoveredTerms = intDupArr(uncoveredTerms, uncoveredCount);
		if(!data.uncoveredTerms){
			data.error = 1;
			data.errorMgs = "intDupArr failed";
			return data;
		}
		data.uncoveredCount = uncoveredCount;

		//Create a string array where each string is the indexes of all prime-implicants that cover ith uncovered term
		char **setArr = NULL;
		int setArrCount = getSetCoverage(&setArr, &prime, piChart, uncoveredTerms, uncoveredCount);
		if(setArrCount == -1){
			data.error = 1;
			data.errorMgs = "getSetCoverage failed";
			return data;
		}

		//Apply Column Reduction
		int newUncoveredCount = column_domination(setArr, &setArrCount, uncoveredTerms, uncoveredCount);
		if(newUncoveredCount <  uncoveredCount){
			uncoveredCount = newUncoveredCount;
			data.newUncoveredTerms = uncoveredTerms;
			data.newUncoveredCount = uncoveredCount;
		}
		else free(uncoveredTerms);

		//Apply Petrick Algorithm
		data.petrick = petrick(&prime, setArr, setArrCount, var);
		if(data.petrick.error){
			data.error = 1;
			data.errorMgs = "petrick failed";
			return data;
		}
		free_2d_pointer(setArr, setArrCount);
	}

	data.result = storeResult(&prime, var);
    return data;
}
