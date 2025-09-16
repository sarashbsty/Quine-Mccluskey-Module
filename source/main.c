/*
	Copyright (c) 2025 Sarash Basumatary 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <math.h>
#include "quine.h"
#include "helper.h"

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    
    int var, minterms[10000];
    
    // Data input
    printf("Enter no. of variables: ");
    scanf("%d", &var);
	
	if(var < 1){
		printf("%d is Invalid\n" , var);
		return 0;
	}
	
	clear_input_buffer();
    
    printf("Enter min terms : ");
	int min_count = get_minterms(minterms , 0 , pow(2, var));
	
	if(min_count == 0){
		printf("No Minterms Entered!\n");
		return 0;
	}
	
	printf("Enter dont care : ");
	int dont_care_count = get_minterms(minterms , min_count , pow(2, var));
	  
    int n_terms = min_count + dont_care_count;
    
    printf("\n\n%d Min terms: ", min_count); 
    for (int i = 0; i < min_count; i++) 
        printf("%d ", minterms[i]);
    
    printf("\n%d Dont care: ", dont_care_count); 
    for (int i = min_count; i < n_terms; i++) 
        printf("%d ", minterms[i]);
    
    // Initialize data structures
    static quine group[100], reduced[100], prime;
	
	fill_group_table(group, minterms, n_terms, var);
	
	int i = 0 , canReduce = 0;
	do{
		canReduce = reduce_table(group, reduced, var);
		prime_implicants(group, &prime, var);
		
		if(i) printf("\nReduction #%d:\n", i);
		else  printf("\n\nInitial Grouping:\n");
		
        displayGroups(group, var);
		i++;
		
		if(canReduce)
			for (int j = 0; j <= var; j++) group[j] = reduced[j];
		
	} while(canReduce);
    
    printf("\n");
    display_implicants(&prime);
    
    char essential_table[100][100][6] , result[100];
    
    essential_implicants(&prime, essential_table, minterms, min_count, result , sizeof(result));
    
    printf("\n\n\nTable to find Essential prime Implicants: \n");
    display_essential_table(&prime, essential_table, minterms, min_count);
  
    printf("\n\nResult: Y(");
    for (int i = 0; i < var; i++) 
		printf( i ? ",%c" : "%c", (char)('A' + i));
    printf(") = %s\n\n",result);
	
    return 0;
}