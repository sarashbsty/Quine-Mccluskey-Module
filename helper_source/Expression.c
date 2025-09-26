#include "memory_tracker.h"

#include<string.h>
#include<stdlib.h>
#include "helper.h"

char* Expression(const char *binary){

	//Finding required size
	int size = 1; //reserved for '\0'
	for(int i = 0; binary[i] != '\0'; i++){
		if(binary[i] == '0') size += 2;
		else if(binary[i] == '1') size++;
	}

	//Invalid case
	if(size == 1){
		char *str = malloc(2);
		if(str) { str[0] = '1'; str[1] = '\0'; }
		return str;
	}

	//Allocate
	char *str = malloc(size * sizeof(*str));
	if(!str) return NULL;

	//write
	int count = 0;
	for(int i = 0; binary[i] != '\0'; i++){
		char var = 'A'+i;
		if(binary[i] == '0') { str[count++] = var; str[count++] = '\''; }
		else if(binary[i] == '1') str[count++] = var;
		else continue;
	}
	str[count] = '\0';
	return str;
}

/*
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
*/


