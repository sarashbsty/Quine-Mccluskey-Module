#include "memory_tracker.h"

#include<stdlib.h>
#include "helper.h"

int **create_table(int row , int col){
	int **table = malloc(row * sizeof(*table));
	if(!table) return NULL;

	for(int i = 0; i < row; i++){
		int *arr = calloc(col, sizeof(*arr));
		if(!arr){
			free_2d_pointer((char**)table,i);
			return NULL;
		}
		table[i] = arr;
	}
	return table;
}
