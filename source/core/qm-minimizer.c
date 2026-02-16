#include <stdlib.h>
#include <math.h>
#include "quine.h"
#include "qmMinimizer.h"
#include "memory.h"
#include "int_array_dup.h"
#include "petrick.h"

qmData qmMinimizer(int *minterms, int n_terms, int minCount, int var){

	qmData data = {0};

	petrickData *petrickFiles = NULL;

	quine **groupTables = NULL , *group = NULL , *newGroup = NULL , *prime = NULL;

	char *essentialPi = NULL , *result = NULL, **set = NULL;

	int *groupSize = NULL, **piChart = NULL, *uncoveredTerms = NULL, *newUncoveredTerms = NULL;

	int groupTablesCount = 0, error = 0 , uncoveredCount = 0, newUncoveredCount = 0 , setCount = 0;

	//Allocate memory for group tables pointer array
	groupTables = calloc(var+1 , sizeof(*groupTables));
	if(!groupTables){
		data.errorMsg = "groupTables Allocation Failed";
		goto FAIL;
	}

	groupSize = calloc(var+1 , sizeof(*groupSize));
	if(!groupSize){
		data.errorMsg = "groupSize Allocation Failed";
		goto FAIL;
	}

	group = createGroupTable(minterms, n_terms, var); //memory safe
	if(!group){
		data.errorMsg = "createGroupTable failed";
		goto FAIL;
	}

	prime = calloc(1,sizeof(*prime));
	if(!prime){
		data.errorMsg = "var :Prime allocation Failed";
		goto FAIL;
	}

	//Table Reduction Process
	while(group != newGroup){

		//Reduction : returns 'group' if no reduction happened.
		newGroup = getReducedTable(group, var);          //memory safe
		if (!newGroup){
			data.errorMsg = "getReducedTable Failed";
			goto FAIL;
		}
		else if(newGroup == group) newGroup = NULL;

		//get prime-implicants afer each reduction
		error = getPrimeImplicants(group, prime, var);  //memory safe
		if(error){
			data.errorMsg = "getPrimeImplicants Failed";
			goto FAIL;
		}

		//store each group Table
		int idx = groupTablesCount;
		groupTables[idx] = group;
		group = NULL;

		groupSize[idx] = var+1;

		groupTablesCount++;

		group = newGroup;
		newGroup = NULL;
	}

	// prime_implicant_chart_table
	piChart = createPiChart(prime, minterms, minCount, var);  //memory safe
	if(!piChart){
		data.errorMsg = "createPiChart Failed";
		goto FAIL;
	}

	//store Essential Implicants
	error = getEssentialPi(&essentialPi, prime);   //memory safe
	if(error){
		data.errorMsg = "getEssentialPi Failed";
		goto FAIL;
	}

	//Get uncovered Minterms if exist
	uncoveredCount = getUncovered(&uncoveredTerms, prime, piChart, minterms, minCount);  //memory safe
	if(uncoveredCount == -1){
		data.errorMsg = "getUncovered Failed";
		goto FAIL;
	}

	if(uncoveredCount > 0){

		//create a copy of Uncovered minterms
		newUncoveredTerms = intDupArr(uncoveredTerms, uncoveredCount); //memory safe
		if(!newUncoveredTerms){
			data.errorMsg = "intDupArr Failed";
			goto FAIL;
		}
		newUncoveredCount = uncoveredCount;

		//Create a string array where each string is the indexes of all prime-implicants that cover ith uncovered term
		setCount = getSetCoverage(&set, prime, piChart, newUncoveredTerms, newUncoveredCount); // memory safe
		if(setCount == -1){
			data.errorMsg = "getSetCoverage Failed";
			goto FAIL;
		}

		//Apply Column Reduction
		newUncoveredCount = column_domination(set, &setCount, newUncoveredTerms, newUncoveredCount); //memory safe
		if(newUncoveredCount == uncoveredCount){
			free(newUncoveredTerms);
			newUncoveredTerms = NULL;
			newUncoveredCount = 0;
		}

		//Apply Petrick Algorithm
		petrickFiles = petrick(prime, set, setCount, var);  //memory safe
		if(petrickFiles->error){
			data.errorMsg = "petrick Failed";
			goto FAIL;
		}
	}

	petrickData *P = petrickFiles;

	if(P) result = getResult(essentialPi, P->combinations[P->minCostIdx]);
	else result = getResult(essentialPi, NULL);
	if(!result){
		data.errorMsg = "storeResult Failed";
		goto FAIL;
	}

	P = NULL;

	data.tableCount 	    =  groupTablesCount;
	data.groupTables	    =  groupTables;
	data.groupSize   	    =  groupSize;
	data.PI 			    =  prime;
	data.piChart		    =  piChart;
	data.essentialPi 	    =  essentialPi;
	data.uncoveredTerms     =  uncoveredTerms;
	data.uncoveredCount     =  uncoveredCount;
	data.newUncoveredTerms  =  newUncoveredTerms;
	data.newUncoveredCount  =  newUncoveredCount;
	data.set                =  set;
	data.setCount           =  setCount;
	data.petrick            =  petrickFiles;
	data.result             =  result;

	groupTablesCount   =  0;
	groupTables        =  NULL;
	groupSize          =  NULL;
	prime              =  NULL;
	piChart            =  NULL;
	essentialPi        =  NULL;
	uncoveredTerms     =  NULL;
	newUncoveredTerms  =  NULL;
	set                =  NULL;
	petrickFiles       =  NULL;
	result             =  NULL;

    return data;

	FAIL:
		//clear group tables
		for(int i = 0; i < groupTablesCount; i++){
			quine *table = groupTables[i];
			for(int j = 0; j < groupSize[i]; j++)
				clear_quine(&table[j]);
			free(table);
			groupSize[i] = 0;
		}

		free(groupTables); groupTables = NULL;
		free(groupSize); groupSize = NULL;
		groupTablesCount = 0;

		for(int i = 0; i < var+1; i++){
			if(group) clear_quine(&group[i]);
			if(newGroup) clear_quine(&newGroup[i]);
		}
		free(group);
		free(newGroup);

		free_2d_pointer(set, setCount);

		if(prime) free_2d_pointer((char**)piChart , prime->count);
		clear_quine(prime);
		free(prime);

		free(essentialPi);
		free(uncoveredTerms);
		free(newUncoveredTerms);

		destroyPetrick(petrickFiles);
		free(petrickFiles);

		free(result);

		if(!data.errorMsg) data.errorMsg = "qmMinimizer Failed";
		data.error = 1;
		return data;

}
