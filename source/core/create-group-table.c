#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include "quine.h"

static int count_1s(char *binary);
static char* ToBinary(int minterm , int var);

Group* createGroups(int *minterms, int allTermsCount, int var){

	Group *groups = calloc(var+1, sizeof(*groups));
	if(!groups) return NULL;

	char *bin = NULL;
	int* arr = NULL;

	for(int i = 0; i < allTermsCount; i++){

		// get Binary equivalents of the minterms
		bin = ToBinary(minterms[i] , var);
		if(!bin) goto FAIL;

		//create array of size 1
		arr = malloc(sizeof(*arr));
		if(!arr)
			goto FAIL;

		//find no.s of ones in binary
		int ones = count_1s(bin);

		int count = groups[ones].count;
		int cap = groups[ones].capacity;

		//allocating implicants
		if(count >= cap){
			cap = (!cap) ? 2 : cap * 2;
			Implicant *tmp = realloc(groups[ones].implicants , cap * sizeof(*tmp));
			if(!tmp) goto FAIL;
			groups[ones].implicants = tmp;
			groups[ones].capacity = cap;
		}

		Implicant *currentImplicant = &groups[ones].implicants[count];

		//inserting the binary , minterm , Count according to no.s of ones
		//Binary
		currentImplicant->binary = bin;
		bin = NULL;

		//minterms
		arr[0] = minterms[i];
		currentImplicant->minterms = arr;
		arr = NULL;

		//mintermCount
		currentImplicant->mintermCount = 1;

		//combined
		currentImplicant->isCombined = false;

		groups[ones].count++;

	}
	return groups;

	FAIL:
		free(bin);
		free(arr);
		for(int i = 0; i < var+1; i++)
			Group_destroy(&groups[i]);
		free(groups);
		return NULL;
}

static int count_1s(char *binary){
	int count = 0;
	for(int i = 0; binary[i] != '\0'; i++)
		if(binary[i] == '1') count++;
	return count;
}

static char* ToBinary(int minterm, int var){

	if (var == 0) return NULL;

	char* binary = malloc((var+1) * sizeof(*binary));
	if(!binary) return NULL;

	for(int j = 0; j < var; j++)
		binary[var-1-j] = ((minterm >> j) & 1) ? '1' : '0';
	binary[var] = '\0';

	return binary;
}
