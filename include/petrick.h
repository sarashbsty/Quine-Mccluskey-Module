#pragma once
#include "quine.h"
#include "memory.h"

typedef struct{
	int error;

	char **process;
	int	processCount;

	char** SOP_terms;
	int SOP_count;

	char** combinations;
	int *cost;
	int minCostIdx;

} petrickData;

petrickData petrick(quine *prime , char **POS_terms, int POS_count, int var);

static void destroyPetrick(petrickData *var){
	free_2d_pointer(var->process, var->processCount);
	free_2d_pointer(var->SOP_terms , var->SOP_count);
	free_2d_pointer(var->combinations, var->SOP_count);
	free(var->cost);
}
