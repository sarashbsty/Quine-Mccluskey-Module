#include "memory_tracker.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

static int isexist(int *arr , int size, int item){
	for(int i = 0; i < size; i++)
		if(item == arr[i]) return i;
	return -1;
}

int set_minimizer(quine *prime , int *minterm_uncovered , int uncovered){

	int covered = 0, covers;
	do{
		covers = 0;
		int pos = -1;
		//determines the prime-implicant which covers most minterms in every iteration
		for(int i = 0; i < prime->count; i++){
			if(prime->minimal[i] == 1) continue;
			int new_covers = 0;
			for(int j = 0; j < uncovered; j++){
				if(minterm_uncovered[j] == -1) continue;
				int check = isexist(prime->minterms[i] , prime->mintermCount[i] , minterm_uncovered[j]);
				if(check != -1) new_covers++;
			}
			if(new_covers > covers){
				pos = i;
				covers = new_covers;
			}
		}

		if(pos != -1){

			//Binary to expression
			char *exp = Expression(prime->binary[pos]);
			if(exp == NULL) { printf("\nERROR: Expression creation Failed | Low Memory | essential-implicants\n"); exit(0); }
			printf("\nSelected %s for minterms : ",exp);
			free(exp);

			//remove covered minterm
			for(int j = 0; j < uncovered; j++){
				if(minterm_uncovered[j] == -1) continue;
				int check = isexist(prime->minterms[pos] , prime->mintermCount[pos] , minterm_uncovered[j]);
				if(check != -1){
					printf("%d ",minterm_uncovered[j]);
					covered++;
					minterm_uncovered[j] = -1;
				}
			}
			//marking the prime implicant as minimal cover
			prime->minimal[pos] = 1;
		}
	}while(covers != 0);

	if(covered == uncovered) return 1;
	else return 0;
}


