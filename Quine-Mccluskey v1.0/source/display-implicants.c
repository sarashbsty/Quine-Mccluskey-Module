#include<stdio.h>
#include<string.h>
#include "quine.h" // quine struture

void display_implicants(const quine *prime){
	
	int line = 2+7+3+20+2;
    char str_line[100];	
	memset(str_line, '=', line);
    str_line[line] = '\0';
	
	printf("\nPrime Implicants:\n");
	printf("%s\n",str_line);
	printf("%-2s%-7s%-3s%-20s%2s" , "|" , "Binary" , " |" , "minterms" , "|");
	printf("\n%s\n",str_line);
	
	for(int i = 0; i < prime->count; i++){
		
		// create a string of minterms
		char str[100]; int offset = 0 , size = prime->mintermCount[i];
		for(int j = 0; j < size; j++){
			int data = prime->minterms[i][j];
			int written = snprintf(str+offset , sizeof(str)-offset , (j < size-1) ? "%d," : "%d" , data);
			offset += written;
		}
		
		printf("%-2s%-7s%-3s%-20s%2s" , "|" , prime->binary[i] , " |" , str , "|");
		printf("\n");
	}
	printf("%s\n",str_line);
}