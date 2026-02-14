#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "boolean_algebra.h"
#include "memory.h"

int isSubset(char* A, char* B){
	int A_in_B = 1;
    int B_in_A = 1;

    for (int i = 0; A[i]; i++){
        if (!strchr(B, A[i])){
			A_in_B = 0; break;
		}
	}

    for (int i = 0; B[i]; i++){
        if (!strchr(A, B[i])){
			B_in_A = 0; break;
		}
	}

    if (A_in_B && !B_in_A) return 1;   // A ⊆ B
    if (B_in_A && !A_in_B) return 2;   // B ⊆ A
    if (A_in_B && B_in_A)  return 1;   // equal
    return 0;
}

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

int distributive(char*** returnPtr, char **termsArr , int count , char *term , int term_size){

	int newCount = 0;
	char** newTermsArr = malloc(count * strlen(term) * sizeof(*newTermsArr));

	for(int i = 0; i < count; i++){

		char *termA = termsArr[i] , *termB = term;

		for(char *x = termB; *x; x++)
		{
			char *newTerm = malloc(sizeof(*newTerm) * term_size);
			if(!newTerm){ free_2d_pointer(newTermsArr, newCount); return -1; }

			//if 'x' literal already exist in 'termA', then just copy 'termA' to newTerm and continue to next
			if(strchr(termA, *x))
				strcpy(newTerm, termA);
			else
				snprintf(newTerm, term_size, "%s%c" , termA, *x);

			newTermsArr[newCount++] = newTerm;
		}
	}

	*returnPtr = newTermsArr;
	return newCount;
}
