#include<stdio.h>
#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

void display_implicants(const quine *prime){
	
	int width[2] = {7,20};
	int n = 2 + width[0] + 3 + width[1] + 2;
    char line[2][22*3+2];	
	
	for(int i=0; i<2; i++) 
		make_line(line[i] , "─" , width[i]+2 , 3);
		
	printf("\nPrime Implicants:\n");
	printf("╔%s┬%s╗\n",line[0],line[1]);
	printf("│ %-7s │ %-20s │\n", "Binary" , "minterms");
	printf("├%s┼%s┤\n",line[0],line[1]);
	
	for(int i = 0; i < prime->count; i++){
		
		// create a string of minterms
		char str[100]; int offset = 0 , size = prime->mintermCount[i];
		for(int j = 0; j < size; j++){
			int data = prime->minterms[i][j];
			int written = snprintf(str+offset , sizeof(str)-offset , (j < size-1) ? "%d," : "%d" , data);
			offset += written;
		}
		printf("│ %-7s │ %-20s │\n" , prime->binary[i] , str);
	}
	printf("╚%s┴%s╝",line[0],line[1]);
}