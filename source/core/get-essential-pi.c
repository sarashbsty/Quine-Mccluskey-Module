#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "quine.h"

int getEssentialPi(char **returnPtr, quine *prime){

	int N = 0;
	for(int i = 0; i < prime->count; i++)
	{
		if(prime->minimal[i] == 0) continue;
		char *exp = prime->expression[i];
		N += snprintf(NULL, 0, "%s,", prime->expression[i]);
	}

	if(N < 0) return 1;
	else if(N == 0){
		*returnPtr = NULL;
		return 0;
	}

	char *exp = malloc(N+1 * sizeof(*exp));
	if(!exp) return 1;

	int offset = 0;
	for(int i = 0; i < prime->count; i++)
	{
		if(prime->minimal[i] == 0) continue;
		offset += snprintf(exp+offset, (N+1)-offset, "%s," ,prime->expression[i]);
	}
	exp[offset-1] = '\0';

	*returnPtr = exp;
	return 0;
}
