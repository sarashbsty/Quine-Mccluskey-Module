#include<stdio.h>
#include "helper.h"

char* array_to_string(const int *arr , const int arr_size ,const char *format){
	int capacity = 16;
	int offset = 0;
	char *str = malloc(capacity * sizeof(*str));
	if(!str) return NULL;

	for(int i = 0; i < arr_size; i++){

		// geting required size
		int needed = snprintf(NULL , 0 , (i==0) ? "%d" : format , arr[i]);
		int size = offset + needed + 1;

		//reallocate
		if(size > capacity){
			while(capacity < size) capacity *= 2;
			char *temp = realloc(str , capacity * sizeof(*temp));
			if(!temp) { free(str); return NULL; }
			str = temp;
		}

		//Write
		int written = snprintf(str+offset , capacity-offset , (i==0) ? "%d" : format , arr[i]);
		offset += written;
	}
	return str;
}


