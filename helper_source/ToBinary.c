#include "helper.h"

void ToBinary(char binary[][500] , int minterms[] , int n_terms, int var){
	for(int i = 0; i < n_terms; i++){
		for(int j = 0; j < var; j++)
			binary[i][var-1-j] = ((minterms[i] >> j) & 1) ? '1' : '0';
		binary[i][var] = '\0';
	}
}	