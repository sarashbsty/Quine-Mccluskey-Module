#include <stdlib.h>
#include <math.h>
#include "quine.h"
#include "qmMinimizer.h"
#include "memory.h"
#include "int_array_dup.h"
#include "petrick.h"
#include "insert_entry.h"

qmData qmMinimizer(int *minterms, int n_terms, int minCount, int var){

	qmData data = {0};

	petrickData *pet = NULL;

	primeData *prime = NULL;

	groupData **groupTables = NULL , *group = NULL , *newGroup = NULL;

	char **essential = NULL , **result = NULL, **set = NULL;

	int *groupSize = NULL, **piChart = NULL, *uncoveredTerms = NULL, *newUncoveredTerms = NULL;

	int groupTablesCount = 0, error = 0 , uncoveredCount = 0, newUncoveredCount = 0;
	int setCount = 0 , primeCount = 0 , noEssentialPrimeCount = 0, essentialCount = 0 , resultCount = 0;

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

	//Table Reduction Process
	while(group != newGroup)
	{
		//Reduction : returns 'group' if no reduction happened.
		newGroup = getReducedTable(group, var);          //memory safe
		if (!newGroup){
			data.errorMsg = "getReducedTable Failed";
			goto FAIL;
		}
		else if(newGroup == group) newGroup = NULL;

		//store each group Table
		int idx = groupTablesCount;
		groupTables[idx] = group;
		group = NULL;

		groupSize[idx] = var+1;
		groupTablesCount++;

		group = newGroup;
		newGroup = NULL;
	}

	//Gather prime-implicants  groupTables
	primeCount = getPrimeImplicants(&prime , groupTables, groupSize , groupTablesCount);  //memory safe
	if(primeCount == -1){
		data.errorMsg = "getPrimeImplicants Failed";
		goto FAIL;
	}

	// prime_implicant_chart_table
	piChart = createPiChart(prime, primeCount, minterms, minCount, var);  //memory safe
	if(!piChart){
		data.errorMsg = "createPiChart Failed";
		goto FAIL;
	}

	//store Essential Implicants
	essentialCount = getEssentialPi(&essential, piChart, prime, primeCount, minterms, minCount);   //memory safe
	if(essentialCount == -1){
		data.errorMsg = "getEssentialPi Failed";
		goto FAIL;
	}
	noEssentialPrimeCount = primeCount - essentialCount;

	//Get uncovered Minterms if exist
	uncoveredCount = getUncovered(&uncoveredTerms, piChart, noEssentialPrimeCount, primeCount, minterms, minCount);  //memory safe
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
		setCount = getSetCoverage(&set, noEssentialPrimeCount, piChart, newUncoveredTerms, newUncoveredCount); // memory safe
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

		//Apply petrick Algorithm
		pet = petrick(prime, noEssentialPrimeCount, set, setCount, var);  //memory safe
		if(pet->error){
			data.errorMsg = "petrick Failed";
			goto FAIL;
		}
	}

	//Result
	int resultCap = 0;
	for(int i = 0; i < essentialCount; i++){
		int error = insertEntry(&result , &resultCount, &resultCap , essential[i]);
		if(error){ data.errorMsg = "insertEntry Failed"; goto FAIL; }
	}

	if(pet){
		int x = pet->minCostIdx;
		for(int i = 0; i < pet->combinations[x].termsCount; i++){
			char *entry = pet->combinations[x].terms[i];
			int error = insertEntry(&result , &resultCount, &resultCap , entry);
			if(error){ data.errorMsg = "insertEntry Failed"; goto FAIL; }
		}
	}

	data.tableCount 	        =  groupTablesCount;
	data.groupTables	        =  groupTables;
	data.groupSize   	        =  groupSize;
	data.prime 			        =  prime;
	data.primeCount             =  primeCount;
	data.noEssentialPrimeCount  =  noEssentialPrimeCount;
	data.piChart		        =  piChart;
	data.essential 	 	        =  essential;
	data.essentialCount         =  essentialCount;
	data.uncoveredTerms         =  uncoveredTerms;
	data.uncoveredCount         =  uncoveredCount;
	data.newUncoveredTerms      =  newUncoveredTerms;
	data.newUncoveredCount      =  newUncoveredCount;
	data.set                    =  set;
	data.setCount               =  setCount;
	data.petrick                =  pet;
	data.result                 =  result;
	data.resultCount            =  resultCount;

    return data;

	FAIL:
		//clear group tables
		for(int i = 0; i < groupTablesCount; i++){
			groupData *table = groupTables[i];
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

		free_2d_pointer((char**)piChart, primeCount);
		free_2d_pointer(essential, essentialCount);
		free_2d_pointer(set, setCount);

		free(uncoveredTerms);
		free(newUncoveredTerms);

		destroyPetrick(pet);
		free(pet);

		free_2d_pointer(result, resultCount);

		destroyPrimeData(prime, primeCount);

		data.groupTables	    =  NULL;
		data.groupSize   	    =  NULL;
		data.prime 			    =  NULL;
		data.piChart		    =  NULL;
		data.essential 	        =  NULL;
		data.uncoveredTerms     =  NULL;
		data.newUncoveredTerms  =  NULL;
		data.set                =  NULL;
		data.petrick            =  NULL;
		data.result             =  NULL;

		if(!data.errorMsg) data.errorMsg = "qmMinimizer Failed";
		data.error = 1;
		return data;

}
