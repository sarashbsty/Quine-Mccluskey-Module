#include<stdlib.h>
#include "helper.h"

char** ToBinary(int minterms[] , int n_terms, int var){
	char **binary = malloc(n_terms * sizeof(*binary)); // create array of n_terms pointers
	if(!binary) return NULL;

	for(int i = 0; i < n_terms; i++){
		binary[i] = malloc((var+1) * sizeof(char));
		if(binary[i] == NULL){
			free_2d_pointer(binary , i);
			return NULL;
		}

		for(int j = 0; j < var; j++)
			binary[i][var-1-j] = ((minterms[i] >> j) & 1) ? '1' : '0';
		binary[i][var] = '\0';
	}
	return binary;
}
