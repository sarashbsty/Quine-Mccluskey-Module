#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

int get_minterms(int min_terms[], int index, int max_terms) {
	
	size_t size = max_terms*(3+4) + 2;  // 3 for per minterm and 4 for whitespace (let)
    char* input = (char*)calloc(size , sizeof(char));
    int initial_index = index;
	
    if (fgets(input, size, stdin)){
        
        // Check for empty input
        if (input[0] == '\n')  return index - initial_index; // No change if empty input
		
        // check whether Input was truncated - clear the rest from stdin
		if (strchr(input, '\n') == NULL) {
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			printf("Input was too long! Only read: %s\n", input);
		}
        
        char *ptr = input;
        int num;
        
        while (index < max_terms){
			
            if(sscanf(ptr, "%d", &num) == 1){
				
                if (num < 0) printf("Error: Negative value %d ignored\n", num);
                else  min_terms[index++] = num;
                 
                while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;  // Move pointer past the number
                while (*ptr == ' ' || *ptr == '\t') ptr++;                          // Skip whitespace
            } 
			else{
                // Check if there's invalid input
                if (*ptr != '\0' && *ptr != '\n'){
                    printf("Error: Invalid input '%c' ignored\n", *ptr);
                    while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;   // Move pointer past the character
					while (*ptr == ' ' || *ptr == '\t') ptr++;     						 // Skip whitespace
                } 
				else break;                    
            }
        }
    }
	free(input);
    return index - initial_index;
}