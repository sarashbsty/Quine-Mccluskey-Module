#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "display_tools.h"
#include "memory.h"

char** make_line(int *width , int widthcount , const char* style , int byte){
	char **line = calloc(widthcount , sizeof(*line));
	if(line == NULL) return NULL;

	for(int i = 0; i < widthcount; i++){
		int n = width[i];
		char *str = malloc(((n*byte)+1) * sizeof(*str));
		if(str == NULL){
			free_2d_pointer(line,i);
			return NULL;
		}

		for (int j = 0; j < n; j++) {
			memcpy(&str[j*byte], style, byte);
		}
		str[n*byte] = '\0';
		line[i] = str;
	}
	return line;
}

char* array_to_string(const int *arr , const int arr_size ,const char *format){

	if (arr_size == 0) {
        char *str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }

	//finding requied size
	int size = 0;
	for(int i = 0; i < arr_size; i++){
		int needed = snprintf(NULL , 0 , (i==0) ? "%d" : format , arr[i]);
		size += needed;
	}

	//allocate
	char *str = malloc((size+1) * sizeof(*str));
	if(!str) return NULL;

	//write
	int offset = 0;
	for(int i = 0; i < arr_size; i++){
		int written = snprintf(str+offset , (size+1)-offset , (i==0) ? "%d" : format , arr[i]);
		offset += written;
	}
	return str;
}

int digit(int x){
	int count = 0;
	while(x > 0){
		count++;
		x = x/10;
	}
	return count;
}



