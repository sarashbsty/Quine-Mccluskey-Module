#include<stdlib.h>
#include<string.h>
#include "quine.h"

int getEssentialPi(char ***returnPtr, primeData *prime, int primeCount){

	char **essential = NULL;
	int count = 0, cap = 0;

	for(int i = 0; i < primeCount; i++){
		if(prime[i].minimal == 0) continue;
		if (count >= cap){
			cap += 1;
			char **tmp = realloc(essential , cap * sizeof(*tmp));
			if(!tmp) goto FAIL;
			essential = tmp;
		}
		char *str = strdup(prime[i].expression);
		if(!str) goto FAIL;
		essential[count++] = str;
	}

	*returnPtr = essential;
	return count;

	FAIL:
		free_2d_pointer(essential,count);
		*returnPtr = NULL;
		return -1;
}
