#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "helper.h"
#include "quine.h" // quine struture

void display_essential_table(const quine *prime , char arr[][100][6] , int Minterms[] , int min_count){
	
	int len = strlen(prime->binary[0]);  
	int width[3] = { (len<10) ? 10 : (len*2)+1  , strlen(prime->binary[0])+1 ,5*min_count };
	int n = 2 + width[0] + 3 + width[1] + 3 + width[2] + 2;
	
    char line1[1000] , line2[3][500];
	make_line(line1 , "─" , n-2 , 3);
	for(int i = 0; i < 3; i++)
		make_line(line2[i] ,"─", width[i]+2 ,3);
		
	printf("╔%s╗\n",line1);
	
	printf("│ %-*s   %-*s    " , width[0] , " " , width[1] , " ");  // this is made just to match with the lower widths
	for(int j = 0; j < min_count; j++) 
		printf("%-5d" , Minterms[j]);
	printf("│\n");
	
	for(int i = 0; i < prime->count; i++){
		
		char *exp = malloc((2*strlen(prime->binary[i]))+1 * sizeof(*exp));
		if(exp == NULL) { printf("display-essential-table : Memory Allocation failed code : 1021"); exit(0); }
		
		//convert to expression
		strcpy(exp , prime->binary[i]);
		Expression(exp);
		
		if(i == 0) printf("├%s┬%s┬%s┤\n",line2[0],line2[1],line2[2]);
		else printf("├%s┼%s┼%s┤\n",line2[0],line2[1],line2[2]);
		
		printf("│ %-*s │ %-*s │ " , width[0] , exp , width[1] , prime->binary[i]);
		for(int j = 0; j < min_count; j++)
			printf("%-5s" , arr[i][Minterms[j]]);
		printf(" │\n");
		
		free(exp);
	}
	printf("╚%s┴%s┴%s╝\n",line2[0],line2[1],line2[2]);
}