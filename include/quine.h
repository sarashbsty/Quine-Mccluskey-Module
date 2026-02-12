#pragma once
#include "memory_tracker.h"
#include "memory.h"

typedef struct quine {
    int count;
	int capacity;
    char **binary;
	char **expression;
    int **minterms;
    int *mintermCount;
    int *combined;
	int *minimal;
	int *cost;
} quine;

int allocate(quine *var , int size);
quine* createGroupTable(int *minterms , int n_terms, int var);
int reduce_table(quine *group , quine *newGroup , int var);
int getPrimeImplicants(quine *group , quine *prime , int var);
int** createPiChart(quine *prime , int minterms[] , int min_count, int var);
int checkCoverage(quine *prime , int *uncovered_terms, int minterms[] , int min_count);
int set_minimizer(quine *prime , int *uncovered_terms , int *uncovered_count);
int getSetCoverage(char** setArr, quine *prime,int** table ,int *uncovered_terms ,int uncovered_count);
int column_domination(char** setArr, int* setArrCount,int *uncovered_terms ,int uncovered_count);
int petrick(quine *prime , char** POS_terms, int POS_count, int var);
char* storeResult(const quine *prime, int var);

void displayGroups(quine *group , int var);
void displayPi(const quine *prime);
void displayPiChart(const quine *prime , int** table , int *minterms , int min_count);

static inline void clear_quine(quine *var){
	if(!var) return;
	if(var->binary){ free_2d_pointer(var->binary , var->count); var->binary = NULL; }
	if(var->expression){ free_2d_pointer(var->expression, var->count); var->expression = NULL; }
	if(var->minterms){ free_2d_pointer((char**)var->minterms , var->count); var->minterms = NULL; }
	if(var->mintermCount){ free(var->mintermCount); var->mintermCount = NULL; }
	if(var->combined) { free(var->combined); var->combined = NULL; }
	if(var->minimal) { free(var->minimal); var->minimal = NULL; }
	if(var->cost) { free(var->cost); var->cost = NULL; }
	var->capacity = 0;
	var->count = 0;
}

/* no more needed
static inline void init_quine(quine *var){
	if(!var) return;
	var->count = 0;
	var->capacity = 0;
	var->binary = NULL;
	var->minterms = NULL;
	var->mintermCount = NULL;
	var->combined = NULL;
	var->minimal = NULL;
	var->expression = NULL;
	var->cost = NULL;
}
*/

