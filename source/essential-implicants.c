#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "string_array.h" // string_arr struture
#include "helper.h"

//check for improvemnt

int expression(string_arr *binary){
	for(int index = 0; index < binary->size; index++){

		size_t len = (2*strlen(binary->arr[index])) + 1;
		char *str = malloc(len);
		if(!str) return 0;

		int count = 0;
		for(int i = 0; binary->arr[index][i] != '\0'; i++){
			char var = 'A'+i;
			if(binary->arr[index][i] == '0') { str[count++] = var; str[count++] = '\''; }
			else if(binary->arr[index][i] == '1') str[count++] = var;
			else continue;
		}
		str[count] = '\0';
		free(binary->arr[index]);
		binary->arr[index] = str;
	}
	return 1;
}

static int isexist(char **arr, const char item[], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i], item) == 0)
            return 1;
    }
    return 0;
}

void essential_implicants(quine *prime , char arr[][100][6] , int min_terms[] , int min_count, char result[100] ,int size){

	//all space initialize
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < min_count; j++)
			strcpy(arr[i][min_terms[j]] , " ");

	//selective marking X
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < prime->mintermCount[i]; j++)
			strcpy(arr[i][prime->minterms[i][j]] , " X");

	//Finding the essential implicant by finding column(minterm) with only one 'X' and the prime implecant that covers it
	string_arr essential = {NULL , 0 , 0};

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
			int check = isexist(essential.arr, prime->binary[index] , essential.size);
			if(check == 1) continue;

			int pass = add_string(&essential , prime->binary[index]);
			if(pass == 0){
				printf("\nERROR: Binary copying fail | Low Memory | essential-implicants\n");
				free_string_array(&essential);
				exit(0);
			}
		}
	}

	int check = expression(&essential);
	if(check == 0){
		printf("\nERROR: expression transform fail | Low Memory | essential-implicants\n");
		free_string_array(&essential);
		exit(0);
	}

	int offset = 0;
	for(int i = 0; i < essential.size; i++){
		int written = snprintf(result+offset , size-offset , i ? "+ %s" : "%s" , essential.arr[i]);
		offset += written;
	}
	//free memory
	free_string_array(&essential);
}
