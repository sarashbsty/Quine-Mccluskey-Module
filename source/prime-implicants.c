#include "memory_tracker.h"

#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

void prime_implicants(quine *group , quine *prime , int var){

	for (int i = 0; i <= var; i++){
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){

			if(group[i].combined[j] == 1) continue; //skip Combined Binaries

			//allocating quine items
			if(prime->capacity == 0){
				int flag = allocate(prime , 4);
				if(flag) { printf("\nERROR: quine items allocation failed | Low memory | prime-implicants\n"); exit(0); }
			}
			else if(prime->count >= prime->capacity){
				int new_cap = prime->capacity + 5;
				int flag = allocate(prime , new_cap);
				if(flag) { printf("\nERROR: quine items allocation failed | Low memory | prime-implicants\n"); exit(0); }
			}

			//get expression string from binary
			char *expres = Expression(group[i].binary[j]);
			if(!expres) { printf("\nERROR: expression string allocation Failed | Low Memory | prime-implicants\n"); exit(0); }

			//Inserting quine items
			int index = prime->count;
			prime->binary[index] = group[i].binary[j];
			prime->expression[index] = expres;
			prime->minterms[index] = group[i].minterms[j];
			prime->mintermCount[index] = group[i].mintermCount[j];
			prime->minimal[index] = 0;
			prime->count++;

			//deattach quine items from group
			group[i].binary[j] = NULL;
			group[i].minterms[j] = NULL;
        }
	}
}
