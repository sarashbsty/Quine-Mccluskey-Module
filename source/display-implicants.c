#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "quine.h" // quine struture
#include "helper.h"  // for make_line , digit , array_to_string

void display_implicants(const quine *prime){
	
	int n = strlen(prime->binary[0]);                                         // also gives no. of variables
	int m = (digit(pow(2,n)) + 1) * prime->mintermCount[prime->count-1];      // digit + 1 = no. of digits + comma(,)
	
	int width[3] = {((n*2) < 10) ? 10 : (n*2)+1 , (n<6) ? 6 : n+1 , (m<8) ? 8 : m+1};
    char line[3][1000];	
	
	for(int i=0; i<3; i++) 
		make_line(line[i] , "─" , width[i]+2 , 3);
		
	printf("\nPrime Implicants:\n");
	printf("╔%s┬%s┬%s╗\n",line[0],line[1],line[2]);
	printf("│ %-*s │ %-*s │ %-*s │\n", width[0], "Expression" ,width[1], "Binary" , width[2], "Minterms");
	
	for(int i = 0; i < prime->count; i++){
		
		char *exp = malloc((2*n + 1) * sizeof(*exp));
		if(exp == NULL) { printf("display-implicants : Memory Allocation failed code : 101"); exit(0); }
		char *str = malloc((m+1) * sizeof(*str));
		if(str == NULL) { printf("display-implicants : Memory Allocation failed code : 102"); exit(0); }
		
		// create a string of minterms
		array_to_string(prime->minterms[i] , prime->mintermCount[i] , str , m+1);
		
		printf("├%s┼%s┼%s┤\n",line[0],line[1],line[2]);
		printf("│ %-*c │ %-*s │ %-*s │\n", width[0], (char)('A'+i) , width[1], prime->binary[i] , width[2], str);
		
		free(exp);
		free(str);
	}
	printf("╚%s┴%s┴%s╝",line[0],line[1],line[2]);
}