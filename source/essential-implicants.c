#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

//check for improvemnt

void essential_implicants(const quine *prime , char arr[][100][6] , int min_terms[] , int min_count, char result[100] ,int size){
	
	//all space initialize
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < min_count; j++)
			strcpy(arr[i][min_terms[j]] , " ");
	
	//selective marking X
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < prime->mintermCount[i]; j++)
			strcpy(arr[i][prime->minterms[i][j]] , " X");
	
	//Finding the essential implicant by finding column with only one '1' and the prime implecant in that 1's row
	int count = 0; char str[100][500];
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
			
			// checking ones is only 1 and duplicates
			if(!is_exist(str, prime->binary[index] , count))
				strcpy(str[count++] , prime->binary[index]);
		}
	}
	
	int offset = 0;
	for(int i = 0; i < count; i++){
		Expression(str[i]);
		int written = snprintf(result+offset , size-offset , i ? "+ %s" : "%s" , str[i]);
		offset += written;
	}
}