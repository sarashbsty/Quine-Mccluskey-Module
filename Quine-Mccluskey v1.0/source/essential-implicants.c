#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

//check for improvemnt

int essential_implicants(const quine *prime , char arr[100][100][6] , int min_terms[] , int min_count, char result[][100]){
	
	//all zero initialize
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < min_count; j++)
			strcpy(arr[i][min_terms[j]] , "  0  ");
	
	//selective entering one
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < prime->mintermCount[i]; j++)
			strcpy(arr[i][prime->minterms[i][j]] , "  1  ");
	
	//Finding the essential implicant by finding column with only one '1' and the prime implecant in that 1's row
	int count = 0;
	for(int j = 0; j < min_count; j++){
		
		int index ,ones = 0;
		for(int i = 0; i < prime->count; i++){
			if(strcmp(arr[i][min_terms[j]], "  1  ") == 0){
				ones++;
				index = i;
			}
		}		
		if(ones == 1){
			strcpy(arr[index][min_terms[j]] , " (1) " );
			
			// checking ones is only 1 and duplicates
			if(!is_exist(result, prime->binary[index] , count))
				strcpy(result[count++] , prime->binary[index]);
		}
	}
	return count;
}