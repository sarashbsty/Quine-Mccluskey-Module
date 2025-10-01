#include "memory_tracker.h"

#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

void prime_implicants(quine *group , quine *prime , int var){

	for (int i = 0; i <= var; i++){
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){

			if(group[i].combined[j] == 1) continue; //skip Combined Binaries
			int index = prime->count;

			//Memory Allocation for quine items
			//binary
			char **temp1 = realloc(prime->binary , (index+1) * sizeof(*temp1));
			if(!temp1){ printf("\nERROR: quine binary allocation failed | Low memory | prime-implicants\n"); exit(0);}
			prime->binary = temp1;

			//minterms
			int **temp2 = realloc(prime->minterms , (index+1) * sizeof(*temp2));
			if(!temp2){ printf("\nERROR: quine minterms allocation failed | Low memory | prime-implicants\n"); exit(0);}
			prime->minterms = temp2;

			//mintermCount
			int *temp3 = realloc(prime->mintermCount , (index+1) * sizeof(*temp3));
			if(!temp3){ printf("\nERROR: quine minterm allocation failed | Low memory | prime-implicants\n"); exit(0);}
			prime->mintermCount = temp3;

			//allocating memory for binary_string
			char *new_binary = malloc((var+1) * sizeof(*new_binary));
			if(!new_binary) { printf("\nERROR: Binary string allocation Failed | Low Memory | prime-implicants\n"); exit(0); }

			//Inserting new_binary
			strcpy(new_binary , group[i].binary[j]);
			prime->binary[index] = new_binary;

			//moving minterms
			prime->minterms[index] = group[i].minterms[j];
			group[i].minterms[j] = NULL;

			prime->mintermCount[index] = group[i].mintermCount[j];

			prime->count++;
        }
	}
}
