#include<stdio.h>
#include "helper.h"

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
