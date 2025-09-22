#pragma once
#include<stdlib.h>

typedef struct {
	char **arr;
	int size;
	int capacity;
} string_arr;

int add_string(string_arr *str , const char *item);

static inline void free_string_array(string_arr *str){
	for(int i = 0; i < str->size; i++)
		free(str->arr[i]);
	free(str->arr);

	str->arr = NULL;
	str->size = 0;
	str->capacity = 0;
}
