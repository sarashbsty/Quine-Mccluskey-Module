#include<stdio.h>
#include<string.h>
#include "helper.h"
#include "quine.h" // quine struture

void display_essential_table(const quine *prime , char arr[100][100][6] , int Minterms[] , int min_count){
	
	int width[2] = { 1 , 5*min_count };
	int n = 2 + width[0] + 3 + width[1] + 2;
	
    char line1[1000] , line2[2][500];
	make_line(line1 , "─" , n-2 , 3);
	make_line(line2[0] , "─" , width[0]+2 , 3);
	make_line(line2[1] , "─" , width[1]+2 , 3);
		
	printf("╔%s╗\n",line1);
	
	printf("│ %-*s    " , width[0] , " ");  // this is made just to match with the lower widths
	for(int j = 0; j < min_count; j++) 
		printf("%-5d" , Minterms[j]);
	printf("│\n");
	
	for(int i = 0; i < prime->count; i++){
		
		if(i == 0) printf("├%s┬%s┤\n",line2[0],line2[1]);
		else printf("├%s┼%s┤\n",line2[0],line2[1]);
		
		printf("│ %-*c │ " , width[0] , (char)('A'+i));
		for(int j = 0; j < min_count; j++)
			printf("%-5s" , arr[i][Minterms[j]]);
		printf(" │\n");
	}
	printf("╚%s┴%s╝\n",line2[0],line2[1]);
}