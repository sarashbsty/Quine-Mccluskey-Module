#include <stdio.h>
#include <stdlib.h>
#include "quine.h"

char* storeResult(const quine *prime, int var)
{
	int N = 0 , first = 1 , cap = 0, offset = 0;

	N += snprintf(NULL, 0, "Result: Y(");
	for(int i = 0; i < var; i++)
		N += snprintf(NULL, 0, (i==0) ? "%c" : ",%c", 'A'+i);
	N += snprintf(NULL, 0, ") = ");

	for(int i = 0; i < prime->count; i++){
		if(prime->minimal[i] == 0) continue;
		N += snprintf(NULL, 0, (first) ? "%s" : " + %s" , prime->expression[i]);
		first = 0;
	}

	cap = N+1;
	char *result = malloc(cap * sizeof(*result));
	if(!result) return NULL;

	offset += snprintf(result+offset, cap-offset, "Result: Y(");
    for (int i = 0; i < var; i++)
		offset += snprintf(result+offset, cap-offset, (i==0) ? "%c" : ",%c", 'A'+i);
    offset += snprintf(result+offset, cap-offset, ") = ");

	first = 1;
	for(int i = 0; i < prime->count; i++){
		if(prime->minimal[i] == 0) continue;
		offset += snprintf(result+offset, cap-offset, (first) ? "%s" : " + %s" , prime->expression[i]);
		first = 0;
	}
	return result;
}
