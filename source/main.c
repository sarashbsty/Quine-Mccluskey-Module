#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "qmMinimizer.h"
#include "memory.h"

static inline void clear_input_buffer(){
	int c;
    while ((c = getchar()) != '\n' && c != EOF);  //clear stdin
}

static int get_minterms(int *minterms , int index, int max_terms);

int main() {

    #ifdef _WIN32
	system("cls");
    SetConsoleOutputCP(CP_UTF8);
    #endif

    // Data input
    printf("Enter no. of variables: ");
	int var;
    scanf("%d", &var);
	clear_input_buffer();

	if(var < 1){
		printf("%d is Invalid\n" , var);
		return 0;
	}

	int maxTerms = pow(2, var);
	int *minterms = malloc(maxTerms * sizeof(*minterms));
	if(minterms == NULL){ printf("ERROR: failed creating minterm array for input | low memory | main"); exit(0); }

    printf("Enter min terms : ");
	int min_count = get_minterms(minterms , 0 , maxTerms);

	if(min_count == 0){
		printf("No Minterms Entered!\n");
		free(minterms);
		return 0;
	}

	printf("Enter dont care : ");
	int dont_care_count = get_minterms(minterms , min_count , maxTerms);

    int n_terms = min_count + dont_care_count;

    printf("\n\n%d Min terms: ", min_count);
    for (int i = 0; i < min_count; i++)
        printf("%d ", minterms[i]);

    printf("\n%d Dont care: ", dont_care_count);
    for (int i = min_count; i < n_terms; i++)
        printf("%d ", minterms[i]);

    qmData data = qmMinimizer(minterms, n_terms, min_count, var);

	if(data.error) printf("%s\n",data.errorMgs);
	else{
		printf("\n");

		for(int i = 0 ; i < data.tableCount; i++){
			displayGroups(data.groupTables[i], data.groupCount[i] - 1);
		}
		printf("\n\n");
		printf("%s",data.result);
	}

	destroyQmGroupTables(&data);
	free(data.result);

    return 0;
}

static char* get_input(){
	size_t capacity = 16;
	size_t size = 0;
	char *buffer = malloc(capacity * sizeof(*buffer));
	if(buffer == NULL) return NULL;

	int ch;
	while((ch = getchar()) != '\n' && ch != EOF){

		if (size >= capacity-1){

			capacity *= 2;
			char* temp = realloc(buffer , capacity * sizeof(*temp));
			if(temp == NULL){
				clear_input_buffer();
				free(buffer);
				return NULL;
			}
			buffer = temp;
		}
		buffer[size++] = (char)ch;
	}
	buffer[size] = '\0';
	return buffer;
}

static void clear_separators(char **ch){
	while (**ch == ' ' || **ch == '\t' || **ch == ',') (*ch)++;
}

static int get_minterms(int *minterms , int index, int max_terms){

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
