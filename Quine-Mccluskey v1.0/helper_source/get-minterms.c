#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

static char* get_input(){
	size_t capacity = 100;
	size_t size = 0;
	char *buffer = (char*)malloc(capacity);
	
	if(buffer == NULL) return NULL;
		
	int ch;
	while((ch = getchar()) != '\n' && ch != EOF){
		
		if (size >= capacity-1){
			capacity += 10;
			char* temp = realloc(buffer,capacity);
			
			if(temp == NULL){                        
				buffer[size] = '\0';
				printf("Error: no space left\n");
				printf("       Read %s\n",buffer);
				clear_input_buffer();
				break;
			}
			
			buffer = temp;
		}
		else
			buffer[size++] = ch;	
	}
	buffer[size] = '\0';
	return buffer;
}
	
int get_minterms(int min_terms[] , int index, int max_terms){
	
	char *input = get_input();
	
	if(input == NULL){
		printf("Memory Allocation Error!\n");
		return 0;
	}
	
	if (input[0] == '\0'){  // No change if empty input  note: *ptr == ptr[0]  
		free(input);
		return 0;
	}
	
	int initial_index = index;    
    char *ptr = input;
    int num;
	
    while (index < max_terms){
		
		while (*ptr == ' ' || *ptr == '\t') ptr++;   // Skips whitespace
		
        if(sscanf(ptr, "%d", &num) == 1){
				
            if (num < 0) printf("Error: Negative value %d ignored\n", num);
            else  min_terms[index++] = num;
                 
            while (*ptr != '\0' && *ptr != ' ' && *ptr != '\t') ptr++;   // Move pointer past the character
        } 
		
		else if (*ptr != '\0' && *ptr != '\n'){ // Check if there's invalid input
		
            printf("Error: Invalid input '%c' ignored\n", *ptr);
            ptr++;   // Move pointer past that character	
        } 
        else break;      
    }
	
	free(input);
	return index - initial_index;
} 


/*
int get_minterms(int min_terms[], int index, int max_terms) {
	
	size_t size = max_terms*(3+4) + 2;  // 3 for per minterm and 4 for whitespace (let)
    char* input = (char*)calloc(size , sizeof(char));
    
    if(input == NULL){
		printf("Memory allocation Error!\n");
		return 0;
	}
   
	int initial_index = index;
    if (fgets(input, size, stdin)){
        
        if (input[0] == '\n')  return 0; // No change if empty input
		
        // check whether Input was truncated - clear the rest from stdin
		if (strchr(input, '\n') == NULL) {
			printf("Input was too long! Only read: %s\n", input);
			clear_input_buffer();
		}
        
        char *ptr = input;
        int num;
        
        while (index < max_terms){
			
			while (*ptr == ' ' || *ptr == '\t') ptr++;                          // Skip whitespace
			
            if(sscanf(ptr, "%d", &num) == 1){
				
                if (num < 0) printf("Error: Negative value %d ignored\n", num);
                else  min_terms[index++] = num;
                 
                while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;  // Move pointer past the number                
            } 
			else if (*ptr != '\0' && *ptr != '\n'){   // Check if there's invalid input
                
                printf("Error: Invalid input '%ch' ignored\n", *ptr);
                ptr++;   // Move pointer past the character
			} 
			else 
				break;                    
        }
    }
	free(input);
    return index - initial_index;
}
*/