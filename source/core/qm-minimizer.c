#include <stdlib.h>
#include <math.h>
#include "quine.h"
#include "qmMinimizer.h"
#include "memory.h"
#include "int_array_dup.h"
#include "petrick.h"

//memory safe
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

	quine *group = createGroupTable(minterms, n_terms, var); //memory safe
	if(!group) FAIL("createGroupTable failed");

	quine *newGroup = NULL;

	quine prime = {0};

	//Table Reduction Process
	while(group != newGroup){

		//Reduction : returns 'group' if no reduction happened.
		newGroup = getReducedTable(group, var); //memory safe

		if(newGroup == group) newGroup = NULL;
		else if (!newGroup){
			for(int i = 0; i < var+1; i++) clear_quine(&group[i]);
			free(group);
			FAIL("Memory Allocation failed var: newGroup");
		}

		//get prime-implicants afer each reduction
		error = getPrimeImplicants(group, &prime, var);  //memory safe
		if(error){
			for(int i = 0; i < var+1; i++){
				clear_quine(&group[i]);
				clear_quine(&newGroup[i]);
			}
			free(group);
			free(newGroup);
			clear_quine(&prime);
			FAIL("getPrimeImplicants failed");
		}

		//store each group Table
		int idx = data.tableCount;
		data.groupTables[idx] = group; group = NULL;
		data.groupCount[idx] = var+1;
		data.tableCount++;

		group = newGroup; newGroup = NULL;
	}

	data.PI = prime;

	// prime_implicant_chart_table
	int **piChart = createPiChart(&prime, minterms, minCount, var);  //memory safe
	if(!piChart) FAIL("Memory Allocation failed var: piChart");

	data.piChart = piChart;

	//store Essential Implicants
	data.essentialCount = getEssentialPi(&data.essentialPi, &prime);   //memory safe
	if(data.essentialCount == -1) FAIL("getEssentialPi failed");

	//Get uncovered Minterms if exist
	int *uncoveredTerms = NULL;
	int uncoveredCount = getUncovered(&uncoveredTerms, &prime, piChart, minterms, minCount);
	if(uncoveredCount == -1) FAIL("getUncovered failed");

	if(uncoveredCount > 0){

		//Store Uncovered minterms
		data.uncoveredTerms = intDupArr(uncoveredTerms, uncoveredCount);
		if(!data.uncoveredTerms){ free(uncoveredTerms); FAIL("intDupArr failed"); }
		data.uncoveredCount = uncoveredCount;

		//Create a string array where each string is the indexes of all prime-implicants that cover ith uncovered term
		char **setArr = NULL;
		int setArrCount = getSetCoverage(&setArr, &prime, piChart, uncoveredTerms, uncoveredCount);
		if(setArrCount == -1){ free(uncoveredTerms); FAIL("getSetCoverage failed"); }

		//Apply Column Reduction
		int newUncoveredCount = column_domination(setArr, &setArrCount, uncoveredTerms, uncoveredCount);
		if(newUncoveredCount <  uncoveredCount){
			uncoveredCount = newUncoveredCount;
			data.newUncoveredTerms = uncoveredTerms;
			uncoveredTerms = NULL;
			data.newUncoveredCount = uncoveredCount;
		}
		else free(uncoveredTerms);

		//Apply Petrick Algorithm
		data.petrick = petrick(&prime, setArr, setArrCount, var);
		if(data.petrick.error) FAIL("petrick failed");

		free_2d_pointer(setArr, setArrCount);
	}

	char *result = storeResult(&prime, var);
	if(!result) FAIL("storeResult Failed");
	data.result = result;
	result = NULL;

    return data;
}
