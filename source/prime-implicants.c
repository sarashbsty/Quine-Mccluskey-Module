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

			//allocating quine items (This will allocate combined variable , even if its not used)
			int flag = allocate(prime , index+1);
			if(flag) { printf("\nERROR: quine items allocation failed | Low memory | prime-implicants\n"); exit(0); }

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
