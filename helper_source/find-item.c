#include<stdio.h>
#include <string.h>
#include "helper.h"

int find_string(char **arr, int size, const char item[]) {
    for (int i = 0; i < size; i++) {
        if(strcmp(arr[i], item) == 0) return i;
    }
    return -1;
}

int find_int(int *arr, int size, int item){
	for(int i = 0; i < size; i++)
		if(item == arr[i]) return i;
	return -1;
}
