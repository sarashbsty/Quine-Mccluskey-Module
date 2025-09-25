#include "memory_tracker.h"

#include<stdlib.h>
#include "helper.h"

char ***create_table(int row , int col , int n){
	char ***table = malloc(row * sizeof(*table));
	if(!table) return NULL;

	for(int i = 0; i < row; i++){

		//create individaul string-array
		char** str = malloc(col * sizeof(*str));
		if(!str){
			free_3d_pointer(table , i , col);
			return NULL;
		}

		//assigning memory for each string
		for(int idx = 0; idx < col; idx++){
			str[idx] = malloc(n * sizeof(char));
			if(!str[idx]){
				free_2d_pointer(str , idx);
				free_3d_pointer(table , i , col);
				return NULL;
			}
		}
		table[i] = str;
	}
	return table;
}
