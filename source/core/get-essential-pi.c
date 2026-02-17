#include<stdlib.h>
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

			if(prime[index].isEssential == 0){
				int error = insertEntry(&essential , &essentialCount, &cap , prime[index].expression);
				if(error) goto FAIL;
				prime[index].isEssential = 1;
			}
		}
	}

	//put essential Primes below the list/table
	for(int i = 0; i < primeCount; i++)
	{
		if(prime[i].isEssential == 0) continue;

		int y = primeCount-1;
		while(prime[y].isEssential == 1 && y > i) y--;
		if(y <= i) break;

		primeData tmp = prime[y];
		prime[y] = prime[i];
		prime[i] = tmp;

		int *tmp1 = table[y];
		table[y] = table[i];
		table[i] = tmp1;

	}

	*returnPtr = essential;
	return essentialCount;

	FAIL:
		free_2d_pointer(essential,essentialCount);
		*returnPtr = NULL;
		return -1;
}
