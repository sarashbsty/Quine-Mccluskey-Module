#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

void fill_group_table(quine group[] , int minterms[], int n_terms, int variables){

	char** Binary = ToBinary(minterms, n_terms, variables);
	if(Binary == NULL) { printf("\nERROR: Binary creation failed | Low memory | fill-group-table\n"); exit(0); }

	for(int i = 0; i <= variables; i++){
		group[i].count = 0; // initialize

		for(int j = 0; j < n_terms; j++){
			if(count_1s(Binary[j]) == i){
				int index = group[i].count;
				strcpy(group[i].binary[index] , Binary[j]);
				group[i].minterms[index][0] = minterms[j];
				group[i].mintermCount[index] = 1;
				group[i].count++;
			}
		}
	}

	free(Binary[0]); // free big block
	free(Binary);    // free array of pointer

}
