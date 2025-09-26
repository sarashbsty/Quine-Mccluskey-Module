#include "memory_tracker.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

void fill_group_table(quine *group , int *minterms, int n_terms, int variables){

	char** Binary = ToBinary(minterms, n_terms, variables);
	if(Binary == NULL) { printf("\nERROR: Binary creation failed | Low memory | fill-group-table\n"); exit(0); }

	// initialize
	for(int i = 0; i <= variables; i++)
		group[i].count = 0;

	for(int j = 0; j < n_terms; j++){

		//find no.s of ones in binary
		int ones = count_1s(Binary[j]);
		int index = group[ones].count;

		//inserting the binary and minterm according to no.s of ones
		strcpy(group[ones].binary[index] , Binary[j]);
		group[ones].minterms[index][0] = minterms[j];
		group[ones].mintermCount[index] = 1;
		group[ones].count++;
	}

	free_2d_pointer(Binary , n_terms);
}
