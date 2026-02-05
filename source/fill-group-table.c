#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

void fill_group_table(quine *group , int *minterms, int n_terms, int var){

	// initialize
	for(int i = 0; i <= var; i++)
		init_quine(&group[i]);

	for(int j = 0; j < n_terms; j++){

		// get Binary equivalents of the minterms
		char* bin = ToBinary(minterms[j] , var);
		if(!bin){ printf("\nERROR: Binary creation failed | Low memory | fill-group-table\n"); exit(0); }

		//find no.s of ones in binary
		int ones = count_1s(bin);
		int index = group[ones].count;

		//allocating quine items
		if(group[ones].capacity == 0){
			int flag = allocate(&group[ones] , 4);
			if(flag) { printf("\nERROR: group items allocation failed | Low memory | fill-group-table\n"); exit(0); }
		}
		else if(group[ones].count >= group[ones].capacity){
			int new_cap = (group[ones].capacity) * 2;
			int flag = allocate(&group[ones] , new_cap);
			if(flag) { printf("\nERROR: group items allocation failed | Low memory | fill-group-table\n"); exit(0); }
		}

		//inserting the binary , minterm , Count according to no.s of ones
		//Binary
		group[ones].binary[index] = bin;

		//minterms
		int *arr = malloc(sizeof(*arr));
		if(!arr) { printf("\nERROR: mintern array creation failed | Low memory | fill-group-table\n"); exit(0); }
		arr[0] = minterms[j];
		group[ones].minterms[index] = arr;

		//mintermCount
		group[ones].mintermCount[index] = 1;

		//combined
		group[ones].combined[index] = 0;

		//expression NULL initialize
		group[ones].expression[index] = NULL;

		group[ones].count++;
	}
}
