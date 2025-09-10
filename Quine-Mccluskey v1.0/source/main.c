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
    
    int var, min_terms[10000], min_count = 0, dont_care_count = 0, temp;
    
    // Data input
    printf("Enter no. of variables: ");
    scanf("%d", &var);
    
    printf("Enter min terms (-1 to end): ");
    while (min_count < pow(2, var)) {
        scanf("%d", &temp);
        if (temp == -1) break;
        min_terms[min_count++] = temp;
    }
    
    printf("Enter dont care (-1 to end): ");
    while ((min_count + dont_care_count) < pow(2, var)) {
        scanf("%d", &temp);
        if (temp == -1) break;
        min_terms[min_count + dont_care_count] = temp;
        dont_care_count++;
    }
    
    int n_terms = min_count + dont_care_count;
    char binary[1000][100];
    ToBinary(binary, min_terms, n_terms, var);
    
    printf("\n\n%d Min terms: ", min_count); 
    for (int i = 0; i < min_count; i++) 
        printf("%d ", min_terms[i]);
    
    printf("\n%d Dont care: ", dont_care_count); 
    for (int i = min_count; i < n_terms; i++) 
        printf("%d ", min_terms[i]);
    
    // Initialize data structures
    static quine group[100], reduced[100], prime;
    
    fill_group_table(group, min_terms, binary, n_terms, var);
    
    int CanReduce = reduce_table(group, reduced, var);
    prime_implicants(group, &prime, var);
    
    printf("\n\nInitial Grouping:\n");
    displayGroups(group, var);
    
    int i = 1;
    while (CanReduce) {
        for (int i = 0; i <= var; i++) group[i] = reduced[i];
        CanReduce = reduce_table(group, reduced, var);
        prime_implicants(group, &prime, var);
        
        printf("\n%dth Reduction: \n", i++);
        displayGroups(group, var);	
    }	
    
    printf("\n");
    display_implicants(&prime);
    
    char result[1000][100];
    char essential_table[100][100][6];
    
    int iterate = essential_implicants(&prime, essential_table, min_terms, min_count, result);
    
    printf("\n\n\nTable to find Essential prime Implicants: \n");
    display_essential_table(&prime, essential_table, min_terms, min_count);
    
    printf("\n\nEssential Prime Implicants: ");
    for (int i = 0; i < iterate; i++)
        printf("%s  ", result[i]);
    printf("\n");
    
    printf("\nResult: Y (");
    for (int i = 0; i < var; i++) {
        printf("%c", (char)('A' + i));
        if (i < var - 1) printf(",");
        else printf(") = ");
    }
    
    for (int i = 0; i < iterate; i++) {
        Expression(result[i]);
        if (i < iterate - 1) printf(" + ");
    }
    printf("\n\n");
    
    return 0;
}