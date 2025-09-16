#include<stdio.h>
#include<string.h>
#include<math.h>
#include "quine.h" // quine struture
#include "helper.h"

static int digit(int x){
	int count = 0;
	while(x > 0){
		count++;
		x = x/10;
	}
	return count;
}

void display_implicants(const quine *prime){
	
	int n = strlen(prime->binary[0]);                                         // also gives no. of variables
	int m = (digit(pow(2,n)) + 1) * prime->mintermCount[prime->count-1];      // digit + 1 = no. of digits + comma(,)
	int width[3] = {1 , (n<6) ? 6 : n+1 , (m<8) ? 8 : m+1};
    char line[3][1000];	
	
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