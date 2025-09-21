#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "helper.h"
#include "quine.h" // quine struture

void display_essential_table(const quine *prime , char arr[][100][6] , int Minterms[] , int min_count){

	int n = strlen(prime->binary[0]);
	int width[2] = {(n<10) ? 10 : (n*2)+1 , 5*min_count };

	int line_width[2];
	for(int i = 0; i < 2; i++) line_width[i] = width[i]+2;

	char **line = make_line(line_width , 2 , "─" , 3);
	if(line == NULL) { printf("\nERROR: Line creation Failed | Low Memory | Display-Essential-Table\n"); exit(0); }

	printf("╔%s┬%s╗\n",line[0] , line[1]);

	printf("│ %-*s │  " , width[0] , "Expression");
	for(int j = 0; j < min_count; j++)
		printf("%-5d" , Minterms[j]);
	printf("│\n");

	for(int i = 0; i < prime->count; i++){

		//Binary to expression
		char *exp = malloc((2*strlen(prime->binary[i]))+1 * sizeof(*exp));
		if(exp == NULL) { printf("\nERROR: Expression creation Failed | Low Memory | Display-Essential-Table\n"); exit(0); }
		strcpy(exp , prime->binary[i]);
		Expression(exp);

		printf("├%s┼%s┤\n",line[0] , line[1]);

		printf("│ %-*s │ " , width[0] , exp);
		for(int j = 0; j < min_count; j++)
			printf("%-5s" , arr[i][Minterms[j]]);
		printf(" │\n");

		free(exp);
	}
	printf("╚%s┴%s╝\n",line[0],line[1]);
	free_pointer_array(line , 2);
}
