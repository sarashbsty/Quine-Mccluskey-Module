#include "helper.h"

void ToBinary(char binary[][100] , int minterms[] , int count, int var){
	for(int i = 0; i < count; i++){
		int j;
		for(j = 0; j < var; j++)
			binary[i][var-1-j] = ((minterms[i] >> j) & 1) ? '1' : '0';
		binary[i][j] = '\0';
	}
}	