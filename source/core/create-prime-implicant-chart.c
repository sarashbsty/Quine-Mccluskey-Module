#include<math.h>
#include<stdlib.h>
#include "quine.h"
#include "memory.h"

int** createPiChart(primeData *prime ,int primeCount, int *minterms , int minCount, int var){

	int **table = malloc(primeCount * sizeof(*table));
	if(!table) return NULL;

	//create an array of size pow(2,var) and Assign 1 to respective column
	for(int i = 0; i < primeCount; i++)
	{
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
	return table;
}
