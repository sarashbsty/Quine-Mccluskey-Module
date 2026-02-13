#include<stdlib.h>
#include "quine.h"

int getUncovered(int **returnPtr, quine *prime, int **piChart, int *minterms, int minCount){

	int *uncoveredTerms = malloc(minCount * sizeof(int));
	if(!uncoveredTerms) return -1;

	//Gather Uncovered Minterms if exist
	int count = 0;
	for(int i = 0; i < minCount; i++){
		int exist = 0;
		for(int j = 0; j < prime->count; j++){
			if(prime->minimal[j] == 0) continue;
			if(piChart[j][minterms[i]] != 0){
			 	exist = 1; break;
			}
		}
		if(!exist) uncoveredTerms[count++] = minterms[i];
	}

	if(!count){ free(uncoveredTerms); uncoveredTerms = NULL; }

	*returnPtr = uncoveredTerms;
	return count;
}
