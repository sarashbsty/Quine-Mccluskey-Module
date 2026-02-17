#include<math.h>
#include<stdlib.h>
#include "quine.h"
#include "memory.h"

int** createPiChart(primeData *prime ,int primeCount, int *minterms , int minCount, int var){

	int **table = malloc(primeCount * sizeof(*table));
	if(!table) return NULL;

	//create an array of size pow(2,var) and Assign 1 to respective column
	for(int i = 0; i < primeCount; i++){

		int *arr = calloc(pow(2,var) , sizeof(*arr));
		if(!arr){
			free_2d_pointer((char**)table, i);
			return NULL;
		}

		for(int j = 0; j < prime[i].mintermCount; j++){
			int idx = prime[i].minterms[j];
			arr[idx] = 1;
		}

		table[i] = arr;
	}

	//Finding the essential implicant by finding column(minterm) with only one '1' and the prime implicant that covers it
	for(int j = 0; j < minCount; j++){
		int index ,count = 0;
		for(int i = 0; i < primeCount && count <= 1; i++){
			if(table[i][minterms[j]] != 0){
				count++;
				index = i;
			}
		}
		if(count == 1){
			//marking for visual guide
			table[index][minterms[j]] = 2;

			//marked for minimal cover
			prime[index].minimal = 1;
		}
	}

	return table;
}
