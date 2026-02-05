#include <stdio.h>
#include <stdlib.h>
#include "quine.h"
#include "helper.h"

void printResult(const quine *prime, int var){
    printf("\n\nResult: Y(");
    for (int i = 1; i <= var; i++)
		printf((i == var) ? "%c" : "%c,", 'A' + i-1);
    printf(") = ");

	for(int i = 0; i < prime->count; i++){
		if(prime->minimal[i] == 0) continue;
		printf((i == 0) ? "%s" : " + %s" , prime->expression[i]);
	}
	printf("\n");
}
