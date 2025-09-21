#pragma once

typedef struct {
	char **arr;
	int size;
	int capacity;
} string_arr;

int add_string(string_arr *str , const char *item);
void free_string_array(string_arr *str);
