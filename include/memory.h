#pragma once
#include<stdlib.h>

static inline void free_2d_pointer(char **arr, int size){
	if(!arr) return;
	for(int i = 0; i < size; i++)
		if(arr[i]) free(arr[i]);
	free(arr);
}
