#pragma once
#include "quine.h"
#include "memory.h"

typedef struct{
	char **terms;
	int termsCount;
} combiStruct;

typedef struct{
	int error;

	char **process;
	int	processCount;

	char** SOP_terms;
	int SOP_count;

	combiStruct *combinations;
	int *cost;
	int minCostIdx;

} petrickData;

petrickData* petrick(primeData *prime , int primeCount , char **POS_terms, int POS_count, int var);

static void destroyCombiStruct(combiStruct *var , int count){
	if(!var) return;
	for(int i = 0; i < count; i++){
		if(var[i].terms) { free_2d_pointer(var[i].terms, var[i].termsCount); var[i].terms = NULL; }
		var[i].termsCount = 0;
	}
	free(var);
}

static void destroyPetrick(petrickData *var){
	if(!var) return;
	if(var->process)      { free_2d_pointer(var->process, var->processCount); var->process = NULL; }
	if(var->SOP_terms)    { free_2d_pointer(var->SOP_terms , var->SOP_count); var->SOP_terms = NULL; }

	destroyCombiStruct(var->combinations , var->SOP_count);

	if(var->cost)         { free(var->cost);  var->cost = NULL; }
	var->SOP_count = 0;
	var->process = 0;
}
