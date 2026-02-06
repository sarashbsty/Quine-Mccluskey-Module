#include "memory_tracker.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "helper.h"

int absorp(char **termsArr, int count){
	for(int a = 0; a < count-1; a++){
		char* A = termsArr[a];

		for(int b = a+1; b < count && A != NULL; b++){

			char* B = termsArr[b];
			if(B == NULL) continue;
			int subset = isSubset(A, B);

			if(subset == 1){   //if A ⊆ B or A = B
				free(B);
				termsArr[b] = NULL;
			}
			else if(subset == 2){  //if B ⊆ A
				free(A);
				termsArr[a] = NULL;
				break;
			}
		}
	}

	int new_count = 0;
	for(int i = 0; i < count; i++){
		if(termsArr[i] == NULL) continue;
		termsArr[new_count++] = termsArr[i];
	}
	return new_count;
}
