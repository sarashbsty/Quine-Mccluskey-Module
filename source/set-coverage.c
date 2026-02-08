#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

int getSetCoverage(char** setArr, quine *prime,int **table ,int *uncovered_terms ,int uncovered_count){

	int max_literals = prime->count , count = 0;

	for(int i = 0; i < uncovered_count; i++){
		char *term = malloc(sizeof(*term) * (max_literals+1));
		if(!term) { printf("\nERROR: Memory Allocation Failed | petrick"); exit(0); }

		int offset = 0;
		for(int j = 0; j < prime->count; j++){
			if(table[j][uncovered_terms[i]] != 0)
				term[offset++] = 'A'+j;
		}
		term[offset] = '\0';
		setArr[count++] = term;
	}

	for(int i = 0; i < uncovered_count; i++){
		int first = 1;
		printf("PI(%d) = {" , uncovered_terms[i]);
		for(char *x = setArr[i]; *x; x++){
			int idx = *x - 'A';
			printf((first) ? "%s" : " , %s",prime->expression[idx]);
			first = 0;
		}
		printf("}\n");
	}
	return count;
}


