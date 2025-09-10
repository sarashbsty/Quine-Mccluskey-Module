#include "helper.h"

int count_1s(char binary[]){
	int count = 0;
	for(int i = 0; binary[i] != '\0'; i++)
		if(binary[i] == '1') count++;
	return count;
}
