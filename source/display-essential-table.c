#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "helper.h"
#include "quine.h" // quine struture

void display_essential_table(const quine *prime , char arr[][100][6] , int Minterms[] , int min_count){

	int n = strlen(prime->binary[0]);
	int width[2] = {(n<10) ? 10 : (n*2)+1 , 5*min_count };

    char line[2][500];
	for(int i = 0; i < 2; i++)
		make_line(line[i] ,"─", width[i]+2 ,3);

	printf("╔%s┬%s╗\n",line[0] , line[1]);

	printf("│ %-*s │  " , width[0] , "Expression");
	for(int j = 0; j < min_count; j++)
		printf("%-5d" , Minterms[j]);
	printf("│\n");

	for(int i = 0; i < prime->count; i++){

		char *exp = malloc((2*strlen(prime->binary[i]))+1 * sizeof(*exp));
		if(exp == NULL) { printf("display-essential-table : Memory Allocation failed code : 101"); exit(0); }

		//convert to expression
		strcpy(exp , prime->binary[i]);
		Expression(exp);

		printf("├%s┼%s┤\n",line[0],line[1]);

		printf("│ %-*s │ " , width[0] , exp);
		for(int j = 0; j < min_count; j++)
			printf("%-5s" , arr[i][Minterms[j]]);
		printf(" │\n");

		free(exp);
	}
	printf("╚%s┴%s╝\n",line[0],line[1]);
}
