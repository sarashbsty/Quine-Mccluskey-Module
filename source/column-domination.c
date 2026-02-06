#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

int column_domination(quine *prime,char ***table ,int *uncovered_terms ,int uncovered_count){

	int max_literals = prime->count , count = 0;

	char **setArr = malloc(sizeof(*setArr) * uncovered_count);
	if(!setArr) { printf("\nERROR: Memory Allocation Failed | petrick"); exit(0); }

	for(int i = 0; i < uncovered_count; i++){
		char *term = malloc(sizeof(*term) * (max_literals+1));
		if(!term) { printf("\nERROR: Memory Allocation Failed | petrick"); exit(0); }

		int offset = 0;
		for(int j = 0; j < prime->count; j++){
			if(strcmp(table[j][uncovered_terms[i]], " ") != 0)
				term[offset++] = 'A'+j;
		}
		term[offset] = '\0';
		setArr[count++] = term;
	}

	for(int a = 0; a < count-1; a++){
		char* A = setArr[a];

		for(int b = a+1; b < count && A != NULL; b++){

			char* B = setArr[b];
			if(B == NULL) continue;
			int subset = isSubset(A, B);

			if(subset == 1){   //if A ⊆ B or A = B
				free(B);
				setArr[b] = NULL;
				uncovered_terms[b] = -1;
			}
			else if(subset == 2){  //if B ⊆ A
				free(A);
				setArr[a] = NULL;
				uncovered_terms[a] = -1;
				break;
			}
		}
	}

	int newUncoveredCount = 0;
	for(int i = 0; i < uncovered_count; i++){
		if(uncovered_terms[i] == -1) continue;
		uncovered_terms[newUncoveredCount++] = uncovered_terms[i];
	}

	free_2d_pointer(setArr,count);
	return newUncoveredCount;
}


