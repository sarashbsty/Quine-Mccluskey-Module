#include "memory_tracker.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

static int get_no_of_common_terms(int arr1[] , int size1 , int arr2[] , int size2) {
	int common = 0;
	for(int i = 0; i < size1; i++){
		for(int j = 0; j < size2; j++) {
			if(arr1[i] == arr2[j]){
				common++;
				break;
			}
		}
	}
	return common;
}

static void delete_at(int *arr, int *size, int pos){
    if (!arr || !size) return;
    if (pos < 0 || pos >= *size) return;

    for (int i = pos; i < (*size) - 1; i++)
        arr[i] = arr[i + 1];

    (*size)--;
}


int set_minimizer(quine *prime , int *minterm_uncovered , int *uncovered){

	int uncoveredSize = *uncovered;
	while(1){
		int covers = 0, pos = -1;

		//determines the prime-implicant which covers most minterms in every iteration
		for(int i = 0; i < prime->count; i++){
			if(prime->minimal[i] == 1) continue;
			int new_covers = get_no_of_common_terms(prime->minterms[i] , prime->mintermCount[i] , minterm_uncovered , uncoveredSize);
			if(new_covers > covers){
				pos = i;
				covers = new_covers;
			}
		}

		if(covers == 0) break;
 		else{

			//marking the prime implicant as minimal cover
			prime->minimal[pos] = 1;

			printf("\nSelected %s for minterms : ", prime->expression[pos]);

			//remove covered minterms from the array
			int idx = 0;
			while(idx < uncoveredSize){
				int check = find_int(prime->minterms[pos] , prime->mintermCount[pos] , minterm_uncovered[idx]);
				if(check != -1){
					printf("%d ",minterm_uncovered[idx]);
					delete_at(minterm_uncovered, &uncoveredSize, idx);
				}
				else idx++;
			}
		}
	}

	*uncovered = uncoveredSize;
	if(*uncovered > 0) return 1;
	else return 0;
}


