#include<stdlib.h>
#include<string.h>
#include "quine.h"

int getEssentialPi(char ***returnPtr, quine *prime){

	char **essentialPi = calloc(prime->count , sizeof(*essentialPi));
	if(!essentialPi) return -1;

	int count = 0;
	for(int i = 0; i < prime->count; i++){
		if(prime->minimal[i] == 0) continue;
		char *exp = strdup(prime->expression[i]);
		if(!exp) return -1;
		essentialPi[count++] = exp;
	}

	*returnPtr = essentialPi;
	return count;
}
