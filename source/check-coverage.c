#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

int checkCoverage(quine *prime , int *uncovered_terms, int minterms[] , int min_count){

	//checking whether the essential implicants suffice and create a array of uncovered minterms
	int count = 0;
	for(int i = 0; i < min_count; i++){
		int check = -1;
		for(int j = 0; j < prime->count; j++){
			if(prime->minimal[j] == 0) continue;
			check = find_int(prime->minterms[j] , prime->mintermCount[j] , minterms[i]);
			if(check != -1) break;
		}
		if(check == -1) uncovered_terms[count++] = minterms[i];
	}

	if(count > 0){
		printf("Minterms not covered : ");
		for(int i = 0; i < count; i++)
			printf("%d ",uncovered_terms[i]);
	}
	return count;
}
