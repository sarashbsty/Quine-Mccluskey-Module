#pragma once
#include "memory_tracker.h"
#include "memory.h"

typedef struct quine {
    int count;
	int capacity;
    char **binary;
    int **minterms;
    int *mintermCount;
    int *combined;
} quine;

typedef struct{
	int capacity;
    char *binary;
	char *expression;
    int *minterms;
    int mintermCount;
	int isEssential;
	int cost;
} primeData;

quine* createGroupTable(int *minterms, int n_terms, int var);
quine* getReducedTable(quine *group , int var);
int getPrimeImplicants(primeData **primePtr , int *primeCountPtr, int *primeCapPtr, quine *group, int groupSize);
int** createPiChart(primeData *prime ,int primeCount, int *minterms , int min_count, int var);
int getEssentialPi(char ***returnPtr, int **table, primeData *prime, int primeCount , int *minterms, int minCount);
int getUncovered(int **returnPtr, int **piChart, int start, int primeCount, int *minterms, int minCount);
int getSetCoverage(char*** returnPtr, int primeCount, int **table ,int *uncoveredTerms ,int uncoveredCount);
int column_domination(char** setArr, int* setArrCount,int *uncovered_terms ,int uncovered_count);

void displayGroups(quine *group , int var);
void displayPi(primeData *prime , int primeCount);
void displayPiChart(primeData *prime , int primeCount, int** table , int *minterms , int min_count);

static inline void clear_quine(quine *var){
	if(!var) return;
	if(var->binary){ free_2d_pointer(var->binary , var->count); var->binary = NULL; }
	if(var->minterms){ free_2d_pointer((char**)var->minterms , var->count); var->minterms = NULL; }
	if(var->mintermCount){ free(var->mintermCount); var->mintermCount = NULL; }
	if(var->combined) { free(var->combined); var->combined = NULL; }
	var->capacity = 0;
	var->count = 0;
}

static void destroyPrimeData(primeData *var, int primeCount){
	if(!var) return;
	for(int i = 0; i < primeCount; i++){
		if(var[i].binary){ free(var[i].binary); var[i].binary = NULL; }
		if(var[i].expression){ free(var[i].expression); var[i].expression = NULL; }
		if(var[i].minterms){ free(var[i].minterms); var[i].minterms = NULL; }
	}
	free(var);
	return;
}

static int allocate(quine *var , int size){
	//Memory Allocation for quine items

	char **binaryTmp =  NULL;
	int **mintermsTmp = NULL, *mintermCountTmp = NULL, *combinedTmp = NULL;

	binaryTmp = realloc(var->binary , size * sizeof(*binaryTmp));
	if(!binaryTmp) goto FAIL;

	mintermsTmp = realloc(var->minterms , size * sizeof(*mintermsTmp));
	if(!mintermsTmp) goto FAIL;

	mintermCountTmp = realloc(var->mintermCount , size * sizeof(*mintermCountTmp));
	if(!mintermCountTmp) goto FAIL;

	combinedTmp = realloc(var->combined , size * sizeof(*combinedTmp));
	if(!combinedTmp) goto FAIL;

	var->binary = binaryTmp;
	var->minterms = mintermsTmp;
	var->mintermCount = mintermCountTmp;
	var->combined = combinedTmp;
	var->capacity = size;
	return 0;

	FAIL:
		free(binaryTmp);
		free(mintermsTmp);
		free(mintermCountTmp);
		free(combinedTmp);
		return 1;
}
