#include "memory_tracker.h"

#include<stdio.h>
#include<stdlib.h>
#include "helper.h"

char* get_input(){
	size_t capacity = 16;
	size_t size = 0;
	char *buffer = malloc(capacity * sizeof(*buffer));
	if(buffer == NULL) return NULL;

	int ch;
	while((ch = getchar()) != '\n' && ch != EOF){

		if (size >= capacity-1){

			capacity *= 2;
			char* temp = realloc(buffer , capacity * sizeof(*temp));
			if(temp == NULL){
				clear_input_buffer();
				free(buffer);
				return NULL;
			}
			buffer = temp;
		}
		buffer[size++] = (char)ch;
	}
	buffer[size] = '\0';
	return buffer;
}
