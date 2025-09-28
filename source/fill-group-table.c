#include "memory_tracker.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

void fill_group_table(quine *group , int *minterms, int n_terms, int var){

	// get Binary equivalents of the minterms
	char** Binary = ToBinary(minterms, n_terms, var);
	if(Binary == NULL) { printf("\nERROR: Binary creation failed | Low memory | fill-group-table\n"); exit(0); }

	// initialize
	for(int i = 0; i <= var; i++){
		group[i].count = 0;
		group[i].binary = NULL;
	}

	for(int j = 0; j < n_terms; j++){

		//find no.s of ones in binary
		int ones = count_1s(Binary[j]);
		int index = group[ones].count;

		//Memory Allocation for group items
		//binary
		char **temp1 = realloc(group[ones].binary , (index+1)*sizeof(*temp1));
		if(!temp1){ printf("\nERROR: group binary array allocation failed | Low memory | fill-group-table\n"); exit(0); }
		group[ones].binary = temp1;

		//inserting the binary , minterm , Count according to no.s of ones
		//Binary
		char *str = malloc(var+1);
		if(!str) { printf("\nERROR: group binary allocation failed | Low memory | fill-group-table\n"); exit(0); }
		strcpy(str , Binary[j]);
		group[ones].binary[index] = str;

		//minterms
		group[ones].minterms[index][0] = minterms[j];

		//mintermCount
		group[ones].mintermCount[index] = 1;

		group[ones].count++;
	}

	free_2d_pointer(Binary , n_terms);
}
