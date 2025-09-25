#include "memory_tracker.h"

#include<stdlib.h>
#include "helper.h"

char** ToBinary(int *minterms , int n_terms, int var){

	if (n_terms == 0 || var == 0) return NULL;

	char **binary = malloc(n_terms * sizeof(*binary)); // create array of n_terms pointers
	if(!binary) return NULL;

	for(int i = 0; i < n_terms; i++){
		char* str = malloc((var+1) * sizeof(*str));
		if(!str){
			free_2d_pointer(binary , i);
			return NULL;
		}

		for(int j = 0; j < var; j++)
			str[var-1-j] = ((minterms[i] >> j) & 1) ? '1' : '0';
		str[var] = '\0';
		binary[i] = str;
	}
	return binary;
}
