#pragma once
#include<stdlib.h>
#include "quine.h"
#include "petrick.h"

typedef struct qmData{
	int error; // handles error boolean
	char *errorMsg;

	int *groupSize;
	quine** groupTables;
	int tableCount;
	int tableCapacity;

	quine PI;
	int **piChart;

	char *essentialPi;

	int *uncoveredTerms;
	int uncoveredCount;

	int *newUncoveredTerms;
	int newUncoveredCount;

	petrickData petrick;

	char *result;

} qmData;

qmData qmMinimizer(int *minterms, int n_terms, int min_count, int var);

static void destroyQmGroupTables(qmData *var){
	for(int i = 0; i < var->tableCount; i++){
		for(int k = 0; k < var->groupSize[i]; k++)
			clear_quine(&var->groupTables[i][k]);
		free(var->groupTables[i]);
		var->groupSize[i] = 0;
	}
	free(var->groupTables); var->groupTables = NULL;
	free(var->groupSize); var->groupSize = NULL;
	var->tableCount = 0;
}

static void destroyQmData(qmData *var)
{
	destroyQmGroupTables(var);

	free_2d_pointer((char**)var->piChart , var->PI.count);

	clear_quine(&var->PI);

	free(var->essentialPi);

	free(var->uncoveredTerms);

	free(var->newUncoveredTerms);

	destroyPetrick(&var->petrick);

	free(var->result);
}

static int qmDataGroupAllocate(qmData *var, int size){

	int *temp1 = realloc(var->groupSize, size * sizeof(*temp1));
	if(!temp1) return 1;

	quine **temp2 = realloc(var->groupTables, size * sizeof(*temp2));
	if(!temp2){
		free(temp1);
		return 1;
	}

	var->groupSize = temp1;
	var->groupTables = temp2;
	var->tableCapacity = size;

	return 0;
}
