#include<stdlib.h>
#include<string.h>
#include "string_array.h"

int add_string(string_arr *str , const char item[]) {

    if (str->size >= str->capacity) {
        int new_capacity = str->capacity + 5;
        char **temp = realloc(str->arr, new_capacity * sizeof(*temp));
        if (!temp) return 0;
		str->arr = temp;

		for(int i = str->capacity; i < new_capacity; i++)
			str->arr[i] = NULL;
		str->capacity = new_capacity;
    }

    size_t len = strlen(item) + 1;
	char *temp = malloc(len);
    if (!temp) return 0;

    strcpy(temp, item);
	str->arr[str->size] = temp;
    str->size++;
    return 1;
}
