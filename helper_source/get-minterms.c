#include "memory_tracker.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "helper.h"

static void clear_separators(char **ch){
	while (**ch == ' ' || **ch == '\t' || **ch == ',') (*ch)++;
}

int get_minterms(int *minterms , int index, int max_terms){

	char *input = get_input();

	if(input == NULL){
		printf("ERROR: minterms Input failed | Low Memory | get-minterms\n");
		return 0;
	}

	if (input[0] == '\0'){  // No change if empty input
		free(input);
		return 0;
	}

	int initial_index = index;
    char *ptr = input;
    int num;

    while (index < max_terms){

		clear_separators(&ptr);
        if(sscanf(ptr, "%d", &num) == 1){

            if (num < 0) printf("Error: Negative value %d ignored\n", num);
			else if(num >= max_terms) printf("Error: %d is not a valid minterm of %d variables. Ignored\n", num, (int)log2(max_terms));
            else  minterms[index++] = num;

            while (*ptr != ',' && *ptr != ' ' && *ptr != '\t' && *ptr != '\0') ptr++;   // Move pointer past the character
        }

		else if (*ptr != '\n' && *ptr != '\0'){ // Check if there's invalid input

            printf("Error: Invalid input '%c' ignored\n", *ptr);
            ptr++;   // Move pointer past that character
        }
        else break;
    }

	clear_separators(&ptr);
	if(*ptr != '\0')                            //note: *ptr == ptr[0]
		printf("Error: Excess Input: %s\n",ptr);

	free(input);
	return index - initial_index;
}
