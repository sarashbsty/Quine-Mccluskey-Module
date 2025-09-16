#include<stdio.h>
#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

void display_implicants(const quine *prime){
	
	int width[3] = {1,7,20};
	int n = 2 + width[0] + 3 + width[1] + 3 + width[2] + 2;
    char line[3][22*3+2];	
	
	for(int i=0; i<3; i++) 
		make_line(line[i] , "─" , width[i]+2 , 3);
		
	printf("\nPrime Implicants:\n");
	printf("╔%s┬%s┬%s╗\n",line[0],line[1],line[2]);
	printf("│ %-*s │ %-*s │ %-*s │\n", width[0], " " ,width[1], "Binary" , width[2], "Minterms");
	
	for(int i = 0; i < prime->count; i++){
		
		// create a string of minterms
		char str[100]; int offset = 0 , size = prime->mintermCount[i];
		for(int j = 0; j < size; j++){
			int data = prime->minterms[i][j];
			int written = snprintf(str+offset , sizeof(str)-offset , (j < size-1) ? "%d," : "%d" , data);
			offset += written;
		}
		printf("├%s┼%s┼%s┤\n",line[0],line[1],line[2]);
		printf("│ %-*c │ %-*s │ %-*s │\n", width[0], (char)('A'+i) , width[1], prime->binary[i] , width[2], str);
	}
	printf("╚%s┴%s┴%s╝",line[0],line[1],line[2]);
}