#include<stdlib.h>
#include<string.h>
#include "string_array.h"

void free_string_array(string_arr *str){
	for(int i = 0; i < str->size; i++)
		free(str->arr[i]);
	free(str->arr);

	str->arr = NULL;
	str->size = 0;
	str->capacity = 0;
}
