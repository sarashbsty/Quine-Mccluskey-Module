#include<string.h>
#include "helper.h"

void Expression(char binary[]){
	int len = strlen(binary) , count = 0;
	char str[100];
	for(int i = 0; i < len; i++){
		char var = 'A'+i;
		if(binary[i] == '0') { str[count++] = var; str[count++] = '\''; }
		else if(binary[i] == '1') str[count++] = var;
		else continue;			
	}
	str[count] = '\0';
	strcpy(binary,str);
}
