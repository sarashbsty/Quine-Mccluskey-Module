#include <stdlib.h>
#include <math.h>
#include "quine.h"
#include "qmMinimizer.h"
#include "memory.h"
#include "int_array_dup.h"
#include "petrick.h"
#include "insert_entry.h"

qmData qmMinimizer(int *minterms, int minCount, int dontCareCount, int var){

	qmData data = {0};

	petrickData *pet = NULL;

	primeData *prime = NULL;

	Table *tables = NULL;
	Group *groups = NULL;

	char **essential = NULL , **result = NULL, **set = NULL;

	int **piChart = NULL, *uncoveredTerms = NULL, *newUncoveredTerms = NULL;

	int tablesCount = 0, error = 0 , uncoveredCount = 0, newUncoveredCount = 0;
	int setCount = 0 , primeCount = 0 , noEssentialPrimeCount = 0, essentialCount = 0 , resultCount = 0;

	int maxPossibleTerm = pow(2,var) - 1;

	if(var < 1){
		data.errorMsg = "Invalid Variable Given";
		goto FAIL;
	}

	int allTermsCount = minCount + dontCareCount;

	if(!minCount){
		data.errorMsg = "No minterms given";
		goto FAIL;
	}
	else if(allTermsCount > pow(2,var)){
		data.errorMsg = "No. of terms exceed maximum terms possible for given Variable";
		goto FAIL;
	}

	for(int i = 0; i < allTermsCount; i++){
		if(minterms[i] > maxPossibleTerm){
			data.errorMsg = "Found Invalid minterm for given Variable";
			goto FAIL;
		}
	}

	//Allocate memory for group tables pointer array
	tables = calloc(var+1 , sizeof(*tables));
	if(!tables){
		data.errorMsg = "tables Allocation Failed";
		goto FAIL;
	}

	groups = createGroups(minterms, minCount + dontCareCount, var); //memory safe
	if(!groups){
		data.errorMsg = "createGroupTable failed";
		goto FAIL;
	}

	//Table Reduction Process
	while(groups)
	{
		//Reduction : returns 'group' if no reduction happened.
		Group *newGroups = getReducedGroups(groups, var);          //memory safe
		if (!newGroups){
			data.errorMsg = "getReducedTable Failed";
			goto FAIL;
		}
		else if(newGroups == groups) newGroups = NULL;

		//store each group Table
		int idx = tablesCount;
		tables[idx].groups =  groups;
		groups = newGroups;

		tables[idx].count  =  var+1;
		tablesCount++;
	}

	//Gather prime-implicants  tables
	primeCount = getPrimeImplicants(&prime , tables , tablesCount);  //memory safe
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


	data.var                    =  var;
	data.tablesCount 	        =  tablesCount;
	data.tables	                =  tables;
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
		for(int i = 0; i < tablesCount; i++)
			Table_destroy(&tables[i]);

		free(tables);
		tables = NULL;
		tablesCount = 0;

		for(int i = 0; i < var+1; i++)
			if(groups) Group_destroy(&groups[i]);

		free(groups);

		free_2d_pointer((char**)piChart, primeCount);
		free_2d_pointer(essential, essentialCount);
		free_2d_pointer(set, setCount);

		free(uncoveredTerms);
		free(newUncoveredTerms);

		destroyPetrick(pet);
		free(pet);

		free_2d_pointer(result, resultCount);

		destroyPrimeData(prime, primeCount);

		data.tables	            =  NULL;
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
