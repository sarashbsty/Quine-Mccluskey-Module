#include "memory_tracker.h"

#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

void prime_implicants(quine group[] , quine *prime , int var){

	for (int i = 0; i <= var; i++){
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){

			if(group[i].combined[j] == 1) continue; //skip Combined Binaries
			int index = prime->count;

			//allocating memory for binary_array
			char **temp = realloc(prime->binary , (index+1)*sizeof(*temp));
			if(!temp){ printf("\nERROR: reduced binary array allocation failed | Low memory | prime-implicant\n"); exit(0); }
			prime->binary = temp;

			//allocating memory for binary_string
			char *new_binary = malloc((var+1) * sizeof(*new_binary));
			if(!new_binary) { printf("\nERROR: Binary string allocation Failed | Low Memory | prime-implicants\n"); exit(0); }
			strcpy(new_binary , group[i].binary[j]);

			//Inserting new_binary
			prime->binary[index] = new_binary;

			//copying midterms
			for(int x = 0; x < group[i].mintermCount[j]; x++)
				prime->minterms[index][x] = group[i].minterms[j][x];
			prime->mintermCount[index] = group[i].mintermCount[j];

			prime->count++;
        }
	}
}
