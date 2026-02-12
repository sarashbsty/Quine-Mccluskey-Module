#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture

static int count_1s(char *binary);
static char* ToBinary(int minterm , int var);

quine* createGroupTable(int *minterms, int n_terms, int var){

	quine *group = calloc(var+1 , sizeof(*group));
	if(!group) return NULL;

	for(int i = 0; i < n_terms; i++){

		// get Binary equivalents of the minterms
		char* bin = ToBinary(minterms[i] , var);
		if(!bin) return NULL;

		//find no.s of ones in binary
		int ones = count_1s(bin);
		int index = group[ones].count;

		//allocating quine items
		if(group[ones].capacity == 0){
			if(allocate(&group[ones] , 4))
				return NULL;
		}
		else if(group[ones].count >= group[ones].capacity){
			int new_cap = (group[ones].capacity) * 2;
			if(allocate(&group[ones] , new_cap))
				return NULL;
		}

		//inserting the binary , minterm , Count according to no.s of ones
		//Binary
		group[ones].binary[index] = bin;

		//minterms
		int *arr = malloc(sizeof(*arr));
		if(!arr) return NULL;

		arr[0] = minterms[i];
		group[ones].minterms[index] = arr;

		//mintermCount
		group[ones].mintermCount[index] = 1;

		//combined
		group[ones].combined[index] = 0;

		//expression NULL initialize
		group[ones].expression[index] = NULL;

		group[ones].count++;
	}
	return group;
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
