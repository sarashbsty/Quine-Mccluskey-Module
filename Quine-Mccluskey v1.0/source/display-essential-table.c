#include<stdio.h>
#include<string.h>
#include "quine.h" // quine struture

//check for improvemnt
void display_essential_table(const quine *prime , char arr[100][100][6] , int min_terms[] , int min_count){
	
	int line = 1+4+3+(5*min_count)+3;
    char str_line[500] , str_line2[500];
	if (line >= sizeof(str_line)) line = sizeof(str_line) - 1; // prevent overflow
	
	memset(str_line, '=', line);
    str_line[line] = '\0';
	memset(str_line2, '-', line);
    str_line2[line] = '\0';
	
	printf("%s\n",str_line);
	printf("|         ");
	for(int i=0;i<min_count;i++) 
		printf("%-5d" , min_terms[i]);
	printf("|\n%s\n",str_line);
	
	for(int i = 0; i < prime->count; i++){
		printf("| [%c] | " , (char)('A'+i));
		for(int x = 0; x < min_count; x++)
			printf("%-5s" , arr[i][min_terms[x]]);
		printf("  |\n%s\n",str_line2);
	}
}