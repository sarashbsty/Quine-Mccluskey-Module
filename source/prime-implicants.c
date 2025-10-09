#include "memory_tracker.h"

#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

void prime_implicants(quine *group , quine *prime , int var){

	for (int i = 0; i <= var; i++){
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){

			if(group[i].combined[j] == 1) continue; //skip Combined Binaries

			//allocating quine items (This will allocate combined variable , even if its not used)
			if(prime->capacity == 0){
				int flag = allocate(prime , 4);
				if(flag) { printf("\nERROR: quine items allocation failed | Low memory | prime-implicants\n"); exit(0); }
			}
			else if(prime->count >= prime->capacity){
				int new_cap = prime->capacity + 5;
				int flag = allocate(prime , new_cap);
				if(flag) { printf("\nERROR: quine items allocation failed | Low memory | prime-implicants\n"); exit(0); }
			}

			//allocating memory for binary_string
			char *new_binary = malloc((var+1) * sizeof(*new_binary));
			if(!new_binary) { printf("\nERROR: Binary string allocation Failed | Low Memory | prime-implicants\n"); exit(0); }

			int index = prime->count;

			//Inserting new_binary
			strcpy(new_binary , group[i].binary[j]);
			prime->binary[index] = new_binary;

			//moving minterms
			prime->minterms[index] = group[i].minterms[j];
			prime->mintermCount[index] = group[i].mintermCount[j];
			group[i].minterms[j] = NULL;

			//initialize
			prime->minimal[index] = 0;

			prime->count++;
        }
	}
}
