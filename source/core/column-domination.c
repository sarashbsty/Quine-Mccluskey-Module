#include "memory_tracker.h"
#include<stdlib.h>
#include "boolean_algebra.h"

int column_domination(char** setArr, int* setArrCount,int *uncovered_terms ,int uncovered_count){

	int count = *setArrCount;
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
			else if(subset == 2){  //if B ⊂ A
				free(A);
				setArr[a] = NULL;
				uncovered_terms[a] = -1;
				break;
			}
		}
	}

	int new_count = 0;
	for(int i = 0; i < count; i++){
		if(setArr[i] == NULL) continue;
		setArr[new_count++] = setArr[i];
	}
	*setArrCount = new_count;

	int newUncoveredCount = 0;
	for(int i = 0; i < uncovered_count; i++){
		if(uncovered_terms[i] == -1) continue;
		uncovered_terms[newUncoveredCount++] = uncovered_terms[i];
	}

	return newUncoveredCount;
}
