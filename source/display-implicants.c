#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "quine.h" // quine struture
#include "helper.h"  // for make_line , digit , array_to_string

void display_implicants(const quine *prime){
	if(prime->count == 0){ printf("\nEmpty Implicant table\n"); return; }

	int n = strlen(prime->binary[0]);                                         // also gives no. of variables
	int m = (digit(pow(2,n)) + 1) * prime->mintermCount[prime->count-1];      // digit + 1 = no. of digits + comma(,)

	int width[3] = {((n*2) > 10) ? n*2 : 10 , (n > 6) ? n : 6 , (m > 8) ? m : 8};

	int line_width[3];
	for(int i = 0; i < 3; i++) line_width[i] = width[i]+2;

	char **line = make_line(line_width , 3 , "─" , 3);
	if(line == NULL) { printf("\nERROR: Line creation Failed | Low Memory | Display-Implicants\n"); exit(0); }

	printf("\nPrime Implicants:\n");
	printf("╔%s┬%s┬%s╗\n",line[0],line[1],line[2]);
	printf("│ %-*s │ %-*s │ %-*s │\n", width[0], "Expression" ,width[1], "Binary" , width[2], "Minterms");

	for(int i = 0; i < prime->count; i++){

		// create a string of minterms
		char *str = array_to_string(prime->minterms[i] , prime->mintermCount[i] , ",%d");
		if(str == NULL) { printf("\nERROR: minterms string creation Failed | Low Memory | Display-Implicants\n"); exit(0); }

		printf("├%s┼%s┼%s┤\n",line[0],line[1],line[2]);
		printf("│ %-*s │ %-*s │ %-*s │\n", width[0], prime->expression[i] , width[1], prime->binary[i] , width[2], str);

		free(str);
	}
	printf("╚%s┴%s┴%s╝",line[0],line[1],line[2]);
	free_2d_pointer(line , 3);
}
