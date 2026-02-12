#pragma once

#include "memory_tracker.h"
#include<stdlib.h>

static inline void free_2d_pointer(char **arr, int size){
	if(arr == NULL) return;
	for(int i = 0; i < size; i++)
		if(arr[i]) free(arr[i]);
	free(arr);
}

static inline void free_3d_pointer(char ***arr, int row, int col){
    if(arr == NULL) return;
    for(int i = 0; i < row; i++)
        free_2d_pointer((char**)arr[i], col);
    free(arr);
}