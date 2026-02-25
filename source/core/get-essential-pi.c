#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include "quine.h"
#include "insert_entry.h"

int getEssentialPi(char ***returnPtr, int **table, primeData *prime, int primeCount , int *minterms, int minCount){

	char **essential = NULL;
	int essentialCount = 0, cap = 0;

	for(int j = 0; j < minCount; j++){
		int index ,count = 0;
		for(int i = 0; i < primeCount && count <= 1; i++){
			if(table[i][minterms[j]] != 0){
				count++;
				index = i;
			}
		}
		if(count == 1)
		{
			//marking for visual guide
			table[index][minterms[j]] = 2;

			if(!prime[index].isEssential){
				int error = insertEntry(&essential , &essentialCount, &cap , prime[index].expression);
				if(error) goto FAIL;
				prime[index].isEssential = true;
			}
		}
	}

	//put essential Primes below the list/table
	int i = 0 , j = primeCount - 1;
	while(i < j)
	{
		while(i < j && !prime[i].isEssential) i++;
		while(j > i && prime[j].isEssential) j--;

		if(i < j){
			primeData tmp = prime[j];
			prime[j] = prime[i];
			prime[i] = tmp;

			int *tmp1 = table[j];
			table[j] = table[i];
			table[i] = tmp1;
		}
	}

	*returnPtr = essential;
	return essentialCount;

	FAIL:
		free_2d_pointer(essential,essentialCount);
		*returnPtr = NULL;
		return -1;
}
