#pragma once
#include<stdlib.h>
#include "quine.h"

typedef struct{
	int error; // handles error boolean
	char *errorMgs;

	int *groupCount;
	quine** groupTables;
	int tableCount;
	int tableCapacity;

	int **piChart;

	int *uncoveredTerms;
	int uncoveredCount;

	char *result;
} qmData;

qmData qmMinimizer(int *minterms, int n_terms, int min_count, int var);

static void destroyQmGroupTables(qmData *var){
	for(int i = 0; i < var->tableCount; i++){
		for(int k = 0; k < var->groupCount[i]; k++)
			clear_quine(&var->groupTables[i][k]);
		free(var->groupTables[i]);
		var->groupCount[i] = 0;
	}
	free(var->groupTables); var->groupTables = NULL;
	free(var->groupCount); var->groupCount = NULL;
	var->tableCapacity = 0;
	var->tableCount = 0;
}

static int qmDataGroupAllocate(qmData *var, int size){

	int *temp1 = realloc(var->groupCount, size * sizeof(*temp1));
	if(!temp1) return 1;

	quine **temp2 = realloc(var->groupTables, size * sizeof(*temp2));
	if(!temp2){
		free(temp1);
		return 1;
	}

	var->groupCount = temp1;
	var->groupTables = temp2;
	var->tableCapacity = size;

	return 0;
}
