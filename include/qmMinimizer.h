#pragma once
#include<stdlib.h>
#include "quine.h"
#include "petrick.h"
#include <string.h>

typedef struct qmData{
	int error; // handles error boolean
	char *errorMsg;

	int *groupSize;
	quine **groupTables;
	int tableCount;
	int tableCapacity;

	primeData *prime;
	int primeCount;
	int noEssentialPrimeCount;

	int **piChart;

	char **essential;
	int essentialCount;

	int *uncoveredTerms;
	int uncoveredCount;

	int *newUncoveredTerms;
	int newUncoveredCount;

	char **set;
	int setCount;

	petrickData *petrick;

	char **result;
	int resultCount;

} qmData;

qmData qmMinimizer(int *minterms, int n_terms, int min_count, int var);

static void destroyQmGroupTables(qmData *var)
{
	if(!var->groupTables) return;
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

	free_2d_pointer((char**)var->piChart , var->primeCount);
	free_2d_pointer(var->set , var->setCount);

	destroyPrimeData(var->prime, var->primeCount);

	free_2d_pointer(var->essential , var->essentialCount);

	free(var->uncoveredTerms);

	free(var->newUncoveredTerms);

	destroyPetrick(var->petrick);
	free(var->petrick);

	free_2d_pointer(var->result , var->resultCount);
}
