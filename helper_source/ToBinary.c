#include<stdlib.h>
#include "helper.h"

char** ToBinary(int minterms[] , int n_terms, int var){
	char **binary = malloc(sizeof(*binary) * n_terms); // create array of n_terms pointers
	if(binary == NULL) return NULL;

	char* block = malloc(sizeof(*block) * (var+1) * n_terms);
	if(block == NULL) return NULL;

	for(int i = 0; i < n_terms; i++){
		binary[i] = block + (i*(var+1));  //pointing to first character of that offset
		for(int j = 0; j < var; j++)
			binary[i][var-1-j] = ((minterms[i] >> j) & 1) ? '1' : '0';
		binary[i][var] = '\0';
	}
	return binary;
}

/*
//multi malloc version
char** ToBinary(int minterms[] , int n_terms, int var){
	char **binary = malloc(sizeof(*binary) * n_terms); // create array of n_terms pointers
	if(binary == NULL){ printf("Low Memory: Binary creation failed at filling Group data code: 101 \n"); exit(0); }

	for(int i = 0; i < n_terms; i++){
		binary[i] = malloc(sizeof(char) * (var+1));
		if(binary[i] == NULL){ printf("Low Memory: Binary creation failed at filling Group data code: 102 \n"); exit(0); }

		for(int j = 0; j < var; j++)
			binary[i][var-1-j] = ((minterms[i] >> j) & 1) ? '1' : '0';
		binary[i][var] = '\0';
	}
	return binary;
}
*/
