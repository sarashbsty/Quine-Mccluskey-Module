#include<string.h>
#include<stdlib.h>
#include "helper.h"

void Expression(char binary[]){
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
	str[count] = '\0';
	strcpy(binary,str);
	free(str);
}
