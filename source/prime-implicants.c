#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

void prime_implicants(quine group[] , quine *prime , int variables){

	for (int i = 0; i <= variables; i++){
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){

			if(group[i].combined[j] == 1) continue; //skip Combined Binaries

			int index = prime->count;
			strcpy(prime->binary[index] , group[i].binary[j]);

			//copying midterms
			for(int x = 0; x < group[i].mintermCount[j]; x++)
				prime->minterms[index][x] = group[i].minterms[j][x];
			prime->mintermCount[index] = group[i].mintermCount[j];

			prime->count++;
        }
	}
}
