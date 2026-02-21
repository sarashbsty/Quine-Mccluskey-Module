#pragma once
#include<stdlib.h>
#include "quine.h"
#include "petrick.h"
#include <string.h>

typedef struct qmData{
	int error; // handles error boolean
	char *errorMsg;

	int var;

	Table *tables;
	int tablesCount;

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

qmData qmMinimizer(int *minterms, int minCount, int dontCareCount, int var);

static void destroyQmData(qmData *var)
{
	if(var->tables){
		for(int i = 0; i < var->tablesCount; i++)
			Table_destroy(&var->tables[i]);
	}
	free(var->tables);

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
