#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

void prime_implicants(quine group[] , quine *prime , int variables){
	
	for (int i = 0; i <= variables; i++){
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){
			
			if(group[i].combined[j] == 0){
				
				// to check whether the implicant is already in prime or not
				int check = is_exist(prime->binary , group[i].binary[j] ,  prime->count);
				if(check == 1) continue;
				
				int index = prime->count;
				strcpy(prime->binary[index] , group[i].binary[j]);
				
				int x;  //copying midterms
				for(x = 0; x < group[i].mintermCount[j]; x++)
					prime->minterms[index][x] = group[i].minterms[j][x];
				
				prime->mintermCount[index] = x;
				prime->count++;
			}
        }
	}	
}