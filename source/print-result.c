#include "memory_tracker.h"

#include <stdio.h>
#include <stdlib.h>
#include "quine.h"
#include "helper.h"

void printResult(const quine *prime, int var){
    printf("\n\nResult: Y(");
    for (int i = 1; i <= var; i++)
		printf((i == var) ? "%c" : "%c,", 'A' + i);
    printf(") = ");

	for(int i = 0; i < prime->count; i++){
		if(prime->minimal[i] == 0) continue;
		printf((i == 0) ? "%s" : " + %s" , prime->expression[i]);
	}
}

/*
char *str_exp = NULL;
	int offset = 0;
	for(int i = 0; i < prime.count; i++){

		if(prime.minimal[i] == 0) continue;

		//Binary to expression
		char *exp = Expression(prime.binary[i]);
		if(exp == NULL) { printf("\nERROR: Expression creation Failed | Low Memory | essential-implicants\n"); exit(0); }

		//Creating expression string
		int needed = snprintf(NULL , 0 , (offset) ? " + %s" : "%s" , exp);
		int new_capacity = offset+needed+1;
		char *temp = realloc(str_exp , new_capacity * sizeof(*temp));
		if(!temp) {
			printf("\nERROR: expression string creation fail | Low Memory | essential-implicants\n");
			free(str_exp);
			exit(0);
		}
		str_exp = temp;
		int written = snprintf(str_exp+offset , new_capacity-offset , (offset) ? " + %s" : "%s" , exp);
		offset += written;
		free(exp);
	}

	char *result = str_exp;

    printf("\n\nResult: Y(");
    for (int i = 1; i <= var; i++){
		printf((i == var) ? "%c" : "%c,", 'A' + i);
    printf(") = %s\n\n", (result ? result : "No result"));

	free(result);
*/
