#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "memory.h"
#include "quine.h" // quine struture
#include "display_tools.h" //make_line

void displayPiChart(primeData *prime , int primeCount, int** table , int *minterms , int min_count){
	if(!prime || !table){ printf("Empty Prime"); return; }

	int n = strlen(prime[0].binary);
	int width[2] = {(n<10) ? 10 : (n*2)+1 , 5*min_count };

	int line_width[2];
	for(int i = 0; i < 2; i++) line_width[i] = width[i]+2;

	char **line = make_line(line_width , 2 , "─" , 3);
	if(line == NULL) { printf("\nERROR: Line creation Failed | Low Memory | Display-Essential-Table\n"); exit(0); }

	printf("\n╭%s┬%s╮\n",line[0] , line[1]);

	printf("│ %-*s │  " , width[0] , "Expression");
	for(int j = 0; j < min_count; j++)
		printf("%-5d" , minterms[j]);
	printf("│\n");

	for(int i = 0; i < primeCount; i++){

		printf("├%s┼%s┤\n",line[0] , line[1]);
		printf("│ %-*s │ " , width[0] , prime[i].expression);
		for(int j = 0; j < min_count; j++){
			if(table[i][minterms[j]] == 2) printf("%-5s" , "(X)");
			else if(table[i][minterms[j]] == 1) printf("%-5s" , " X");
			else printf("%-5s" , "");
		}
		printf(" │\n");

	}
	printf("╰%s┴%s╯\n",line[0],line[1]);
	free_2d_pointer(line , 2);
}
