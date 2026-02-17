#include<stdlib.h>
#include<stdio.h>
#include<string.h>
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

			prime[index].isEssential = 1;

			int error = insertEntry(&essential , &essentialCount, &cap , prime[index].expression);
			if(error) goto FAIL;

			int y = primeCount-1;
			while(prime[y].isEssential == 1) y--;

			primeData tmp = prime[y];
			prime[y] = prime[index];
			prime[index] = tmp;

			int *tmp1 = table[y];
			table[y] = table[index];
			table[index] = tmp1;
		}
	}

	*returnPtr = essential;
	return essentialCount;

	FAIL:
		free_2d_pointer(essential,essentialCount);
		*returnPtr = NULL;
		return -1;
}
