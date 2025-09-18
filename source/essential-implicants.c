#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

//check for improvemnt

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
	char **str = NULL;
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
			if(!isexist(str, prime->binary[index] , count)){
				if(count >= capacity){
					capacity += 5;
					char **temp = realloc(str , capacity*sizeof(*str));
					if(temp == NULL){ printf("Low Memory : Failed allocating at essential implicants code:101"); exit(0); }
					str = temp;
				}
				
				size_t len = (2*strlen(prime->binary[index])) + 1;
				str[count] = malloc(len * sizeof(char));
				if(str[count] == NULL){ printf("Low Memory : Failed allocating at essential implicants code:102"); exit(0); }
				
				strcpy(str[count++] , prime->binary[index]);
			}
		}
	}
	
	int offset = 0;
	for(int i = 0; i < count; i++){
		Expression(str[i]);
		int written = snprintf(result+offset , size-offset , i ? "+ %s" : "%s" , str[i]);
		offset += written;
	}
	
	//free memory
	for(int i = 0; i < count; i++) free(str[i]);	
	free(str);
}