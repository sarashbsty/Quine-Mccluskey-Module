#include<stdlib.h>
#include "helper.h"

char* ToBinary(int minterm, int var){

	if (var == 0) return NULL;

	char* binary = malloc((var+1) * sizeof(*binary));
	if(!binary) return NULL;

	for(int j = 0; j < var; j++)
		binary[var-1-j] = ((minterm >> j) & 1) ? '1' : '0';
	binary[var] = '\0';

	return binary;
}
