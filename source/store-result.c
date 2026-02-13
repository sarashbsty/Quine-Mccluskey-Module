#include <stdio.h>
#include <stdlib.h>
#include "quine.h"

char* storeResult(const quine *prime, int var){
	int offset = 0;
	int cap = ((2*var)+3) * prime->count + 1;
	char *data = malloc(cap * sizeof(*data));

    offset += snprintf(data+offset, cap-offset, "Result: Y(");
    for (int i = 1; i <= var; i++)
		offset += snprintf(data+offset, cap-offset, (i == var) ? "%c" : "%c,", 'A' + i-1);
    offset += snprintf(data+offset, cap-offset, ") = ");

	int first = 1;
	for(int i = 0; i < prime->count; i++){
		if(prime->minimal[i] == 0) continue;
		offset += snprintf(data+offset, cap-offset, (first) ? "%s" : " + %s" , prime->expression[i]);
		first = 0;
	}
	return data;
}
