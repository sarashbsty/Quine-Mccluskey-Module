#include <stdlib.h>
#include <math.h>
#include "quine.h"
#include "qmMinimizer.h"
#include "memory.h"
#include "int_array_dup.h"
#include "petrick.h"

qmData qmMinimizer(int *minterms, int n_terms, int minCount, int var){

	qmData data = {0};

	#define FAIL(msg)\
        do { \
            data.error = 1;\
            data.errorMsg = msg;\
            return data;\
        } while (0)


	//Allocate memory for group tables pointer array
	int error = qmDataGroupAllocate(&data, var+1);
	if(error) FAIL("qmDataGroupAllocate Failed");

	quine *group = createGroupTable(minterms, n_terms, var);
	if(!group) FAIL("createGroupTable failed");

	quine *newGroup = NULL;

	quine prime = {0};

	//Table Reduction Process
	while(group != newGroup){

		//Reduction
		newGroup = getReducedTable(group, var); //returns 'group' if no reduction happened.

		if(newGroup == group) newGroup = NULL;
		else if (!newGroup){
			int idx = data.tableCount;
			data.groupTables[idx] = group; group = NULL;
			data.groupCount[idx] = var+1;
			data.tableCount++;
			FAIL("Memory Allocation failed var: newGroup");
		}

		//get prime-implicants afer each reduction
		error = getPrimeImplicants(group, &prime, var);
		if(error) FAIL("getPrimeImplicants failed");

		//store each group Table
		int idx = data.tableCount;
		data.groupTables[idx] = group; group = NULL;
		data.groupCount[idx] = var+1;
		data.tableCount++;

		group = newGroup; newGroup = NULL;
	}

	data.PI = prime;

	// prime_implicant_chart_table
	int **piChart = createPiChart(&prime, minterms, minCount, var);
	if(!piChart) FAIL("Memory Allocation failed var: piChart");

	data.piChart = piChart;

	//store Essential Implicants
	data.essentialCount = getEssentialPi(&data.essentialPi, &prime);
	if(data.essentialCount == -1) FAIL("getEssentialPi failed");

	//Get uncovered Minterms if exist
	int *uncoveredTerms = NULL;
	int uncoveredCount = getUncovered(&uncoveredTerms, &prime, piChart, minterms, minCount);
	if(uncoveredCount == -1) FAIL("getUncovered failed");

	if(uncoveredCount > 0){

		//Store Uncovered minterms
		data.uncoveredTerms = intDupArr(uncoveredTerms, uncoveredCount);
		if(!data.uncoveredTerms) FAIL("intDupArr failed");
		data.uncoveredCount = uncoveredCount;

		//Create a string array where each string is the indexes of all prime-implicants that cover ith uncovered term
		char **setArr = NULL;
		int setArrCount = getSetCoverage(&setArr, &prime, piChart, uncoveredTerms, uncoveredCount);
		if(setArrCount == -1) FAIL("getSetCoverage failed");

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
		if(data.petrick.error) FAIL("petrick failed");

		free_2d_pointer(setArr, setArrCount);
	}

	data.result = storeResult(&prime, var);
    return data;
}
