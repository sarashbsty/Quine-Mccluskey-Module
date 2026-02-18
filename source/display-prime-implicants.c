#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "quine.h" // quine struture
#include "memory.h"
#include "display_tools.h" // for make_line , array_to_string , digit

void displayPi(primeData *prime , int primeCount){
	if(!prime || !primeCount){ printf("\nEmpty Implicant table\n"); return; }

	int maxIdx = 0;
	for(int i = 0; i < primeCount; i++){
		if(prime[i].mintermCount > prime[maxIdx].mintermCount)
			maxIdx = i;
	}

	int n = strlen(prime[0].binary);                                         // also gives no. of variables
	int maxPossibleMinterm = pow(2,n);
	int m = (digit(maxPossibleMinterm) + 1) * prime[maxIdx].mintermCount;      // digit + 1 = no. of digits + comma(,)

	int width[3] = {((n*2) > 10) ? n*2 : 10 , (n > 6) ? n : 6 , (m > 8) ? m : 8};

	int line_width[3];
	for(int i = 0; i < 3; i++) line_width[i] = width[i]+2;

	char **line = make_line(line_width , 3 , "─" , 3);
	if(line == NULL) { printf("\nERROR: Line creation Failed | Low Memory | Display-Implicants\n"); exit(0); }

	printf("\n╭%s┬%s┬%s╮\n",line[0],line[1],line[2]);
	printf("│ %-*s │ %-*s │ %-*s │\n", width[0], "Expression" ,width[1], "Binary" , width[2], "Minterms");

	for(int i = 0; i < primeCount; i++){

		// create a string of minterms
		char *str = array_to_string(prime[i].minterms , prime[i].mintermCount , ",%d");
		if(str == NULL) { printf("\nERROR: minterms string creation Failed | Low Memory | Display-Implicants\n"); exit(0); }

		printf("├%s┼%s┼%s┤\n",line[0],line[1],line[2]);
		printf("│ %-*s │ %-*s │ %-*s │\n", width[0], prime[i].expression , width[1], prime[i].binary , width[2], str);

		free(str);
	}
	printf("╰%s┴%s┴%s╯\n",line[0],line[1],line[2]);
	free_2d_pointer(line , 3);
}
