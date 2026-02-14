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

petrickData* petrick(quine *prime , char **POS_terms, int POS_count, int var);

static void destroyPetrick(petrickData *var){
	if(!var) return;
	if(var->process)      { free_2d_pointer(var->process, var->processCount); var->process = NULL; }
	if(var->SOP_terms)    { free_2d_pointer(var->SOP_terms , var->SOP_count); var->SOP_terms = NULL; }
	if(var->combinations) { free_2d_pointer(var->combinations, var->SOP_count); var->combinations = NULL;}
	if(var->cost)         { free(var->cost);  var->cost = NULL; }
	var->SOP_count = 0;
	var->process = 0;
}
