#pragma once
#include "memory.h"
#include "stdbool.h"

typedef struct {
    char *binary;
    int *minterms;
    int mintermCount;
    bool isCombined;
} Implicant;

static void Implicant_destroy(Implicant *var){
	if(!var) return;

	free(var->binary);
	free(var->minterms);

	var->binary = NULL;
	var->minterms = NULL;
	var->mintermCount = 0;
	var->isCombined = false;
}

typedef struct {
    Implicant *implicants;
    int count;
    int capacity;
} Group;

static void Group_destroy(Group *var){
	if(!var) return;
	if(var->implicants){
		for(int i = 0; i < var->count; i++)
			Implicant_destroy(&var->implicants[i]);
		free(var->implicants);
		var->implicants = NULL;
	}
	var->count = 0;
	var->capacity = 0;
}

typedef struct {
    Group *groups;
    int count;
    int capacity;
} Table;

static void Table_destroy(Table *var){
	if(!var) return;
	if(var->groups){
		for(int i = 0; i < var->count; i++)
			Group_destroy(&var->groups[i]);
		free(var->groups);
		var->groups = NULL;
	}
	var->count = 0;
	var->capacity = 0;
}

typedef struct{
	int capacity;
    char *binary;
	char *expression;
    int *minterms;
    int mintermCount;
	bool isEssential;
	int cost;
} primeData;

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

Group* createGroups(int *minterms, int allTermsCount, int var);
Group* getReducedGroups(Group *groups , int var);
int getPrimeImplicants(primeData **primePtr ,Table *tables, int tablesCount);
int** createPiChart(primeData *prime ,int primeCount, int *minterms , int min_count, int var);
int getEssentialPi(char ***returnPtr, int **table, primeData *prime, int primeCount , int *minterms, int minCount);
int getUncovered(int **returnPtr, int **piChart, int start, int primeCount, int *minterms, int minCount);
int getSetCoverage(char*** returnPtr, int primeCount, int **table ,int *uncoveredTerms ,int uncoveredCount);
int column_domination(char** setArr, int* setArrCount,int *uncovered_terms ,int uncovered_count);

void displayTable(Table *table, int var);
void displayPi(primeData *prime , int primeCount);
void displayPiChart(primeData *prime , int primeCount, int** table , int *minterms , int min_count);
