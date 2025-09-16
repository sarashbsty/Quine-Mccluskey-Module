#include<stdio.h>
#include<string.h>
#include "helper.h"
#include "quine.h" // quine struture

void display_essential_table(const quine *prime , char arr[100][100][6] , int Minterms[] , int min_count){
	
	int width[3] = { 1 , strlen(prime->binary[0])+1 ,5*min_count };
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
		
		if(i == 0) printf("├%s┬%s┬%s┤\n",line2[0],line2[1],line2[2]);
		else printf("├%s┼%s┼%s┤\n",line2[0],line2[1],line2[2]);
		
		printf("│ %-*c │ %-*s │ " , width[0] , (char)('A'+i) , width[1] , prime->binary[i]);
		for(int j = 0; j < min_count; j++)
			printf("%-5s" , arr[i][Minterms[j]]);
		printf(" │\n");
	}
	printf("╚%s┴%s┴%s╝\n",line2[0],line2[1],line2[2]);
}