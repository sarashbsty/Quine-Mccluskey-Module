#include<string.h>
#include<stdlib.h>
#include "quine.h" // groupData struture

static int count_1s(char *binary);
static char* ToBinary(int minterm , int var);

groupData* createGroupTable(int *minterms, int n_terms, int var){

	groupData *group = calloc(var+1, sizeof(*group));
	if(!group) return NULL;

	char *bin = NULL;
	int* arr = NULL;

	for(int i = 0; i < n_terms; i++){

		// get Binary equivalents of the minterms
		bin = ToBinary(minterms[i] , var);
		if(!bin) goto FAIL;

		//create array of size 1
		arr = malloc(sizeof(*arr));
		if(!arr) goto FAIL;

		//find no.s of ones in binary
		int ones = count_1s(bin);
		int index = group[ones].count;

		//allocating groupData items
		if(group[ones].capacity == 0){
			if(allocate(&group[ones] , 4))
				goto FAIL;
		}

		else if(group[ones].count >= group[ones].capacity){
			int new_cap = (group[ones].capacity) * 2;
			if(allocate(&group[ones] , new_cap))
				goto FAIL;
		}

		//inserting the binary , minterm , Count according to no.s of ones
		//Binary
		group[ones].binary[index] = bin;
		bin = NULL;

		//minterms
		arr[0] = minterms[i];
		group[ones].minterms[index] = arr;
		arr = NULL;

		//mintermCount
		group[ones].mintermCount[index] = 1;

		//combined
		group[ones].combined[index] = 0;

		group[ones].count++;
	}
	return group;

	FAIL:
		free(bin);
		free(arr);
		for(int i = 0; i < var+1; i++)
			clear_quine(&group[i]);
		free(group);
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
