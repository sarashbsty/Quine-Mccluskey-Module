#pragma once
#include<stdlib.h>
#include "quine.h"

typedef struct qmData{
	int error; // handles error boolean
	char *errorMgs;

	int *groupCount;
	quine** groupTables;
	int tableCount;
	int tableCapacity;

	quine PI;
	int **piChart;

	char **essentialPi;
	int essentialCount;

	int *uncoveredTerms;
	int uncoveredCount;

	int *newUncoveredTerms;
	int newUncoveredCount;

	char **petrickLog;
	int logCount;

	char** SOP_terms;
	int SOP_count;

	char** combinations;
	int *cost;

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

static void destroyQmData(qmData *var){
	destroyQmGroupTables(var);
	free_2d_pointer((char**)var->piChart , var->PI.count);
	free_2d_pointer(var->essentialPi , var->essentialCount);
	free_2d_pointer(var->petrickLog, var->logCount);
	free_2d_pointer(var->SOP_terms, var->SOP_count);
	free_2d_pointer(var->combinations, var->SOP_count);
	clear_quine(&var->PI);
	free(var->uncoveredTerms);
	free(var->newUncoveredTerms);
	free(var->result);
	free(var->cost);
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
