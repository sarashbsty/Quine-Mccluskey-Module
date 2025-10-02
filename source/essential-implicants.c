#include "memory_tracker.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "string_array.h" // string_arr struture
#include "helper.h"

//check for improvemnt
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
			int check = is_exist(essential, prime->binary[index] , count);
			if(check == 1) continue;

			//Creating Dynamic array of pointer to store prime binary address
			if(count >= capacity){
				capacity += 5;
				char **temp = realloc(essential , capacity*sizeof(*essential));
				if(temp == NULL){
					printf("\nERROR: Binary copying fail | Low Memory | essential-implicants\n");
					free(essential);
					exit(0);
				}
				essential = temp;
			}
			essential[count++] = prime->binary[index];
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
			free(essential);
			exit(0);
		}
		str_exp = temp;
		int written = snprintf(str_exp+offset , new_capacity-offset , i ? " + %s" : "%s" , exp);
		offset += written;
		free(exp);
	}
	//free memory
	free(essential);
	return str_exp;
}
