#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "string_array.h" // string_arr struture
#include "helper.h"

//check for improvemnt

static int isexist(char **arr, const char item[], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i], item) == 0)
            return 1;
    }
    return 0;
}

char* essential_implicants(quine *prime , char ***arr , int min_terms[] , int min_count){

	//all space initialize
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < min_count; j++)
			strcpy(arr[i][min_terms[j]] , " ");

	//selective marking X
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < prime->mintermCount[i]; j++)
			strcpy(arr[i][prime->minterms[i][j]] , " X");

	//Finding the essential implicant by finding column(minterm) with only one 'X' and the prime implecant that covers it
	char** essential = NULL;
	int count = 0 , capacity = 0;

	for(int j = 0; j < min_count; j++){

		int index ,ones = 0;
		for(int i = 0; i < prime->count; i++){
			if(strcmp(arr[i][min_terms[j]], " X") == 0){
				ones++;
				index = i;
			}
		}

		if(ones == 1){

			strcpy(arr[index][min_terms[j]] , "(X)" );

			// checking duplicates
			int check = isexist(essential, prime->binary[index] , count);
			if(check == 1) continue;

			//Creating Dynamic string to store binary
			if(count >= capacity){
				capacity += 5;
				char **temp = realloc(essential , capacity*sizeof(*essential));
				if(temp == NULL){
					printf("\nERROR: Binary copying fail | Low Memory | essential-implicants\n");
					free_2d_pointer(essential , count);
					exit(0);
				}
				essential = temp;
			}

			size_t len = strlen(prime->binary[index]) + 1;
			char *str = malloc(len * sizeof(*str));
			if(!str){
				printf("\nERROR: Binary copying fail | Low Memory | essential-implicants\n");
				free_2d_pointer(essential , count);
				exit(0);
			}
			strcpy(str , prime->binary[index]);
			essential[count++] = str;
		}
	}

	char *str_exp = NULL;
	int offset = 0;

	for(int i = 0; i < count; i++){

		//Binary to expression
		char *exp = Expression(essential[i]);
		if(exp == NULL) { printf("\nERROR: Expression creation Failed | Low Memory | essential-implicants\n"); exit(0); }

		//Creating expression string
		int needed = snprintf(NULL , 0 , i ? " + %s" : "%s" , exp);
		int new_capacity = offset+needed+1;
		char *temp = realloc(str_exp , new_capacity * sizeof(*temp));
		if(!temp) {
			printf("\nERROR: expression string creation fail | Low Memory | essential-implicants\n");
			free(str_exp);
			free_2d_pointer(essential , count);
			exit(0);
		}
		str_exp = temp;
		int written = snprintf(str_exp+offset , new_capacity-offset , i ? " + %s" : "%s" , exp);
		offset += written;
		free(exp);
	}
	//free memory
	free_2d_pointer(essential , count);
	return str_exp;
}
