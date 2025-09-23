#include<string.h>
#include<stdlib.h>
#include "helper.h"

void Expression(char *binary){
	char *str = NULL;
	int cap = 0 , count = 0;

	for(int i = 0; binary[i] != '\0'; i++){

		if(count >= cap-2){
			cap += 5;
			char *temp = realloc(str , cap*sizeof(*temp));
			if(temp == NULL) { printf("ERROR : Low memory   code : Expression\n"); exit(0); }
			str = temp;
		}

		char var = 'A'+i;
		if(binary[i] == '0') { str[count++] = var; str[count++] = '\''; }
		else if(binary[i] == '1') str[count++] = var;
		else continue;
	}
	if(count == 0) str[count++] = '1';

	str[count] = '\0';
	strcpy(binary,str);
	free(str);
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


