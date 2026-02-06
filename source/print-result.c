#include <stdio.h>
#include <stdlib.h>
#include "quine.h"
#include "helper.h"

void printResult(const quine *prime, int var){
    printf("\n\nResult: Y(");
    for (int i = 1; i <= var; i++)
		printf((i == var) ? "%c" : "%c,", 'A' + i-1);
    printf(") = ");

	int first = 1;
	for(int i = 0; i < prime->count; i++){
		if(prime->minimal[i] == 0) continue;
		printf((first) ? "%s" : " + %s" , prime->expression[i]);
		first = 0;
	}
	printf("\n\n");
}
