#include<string.h>
#include "helper.h"

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
