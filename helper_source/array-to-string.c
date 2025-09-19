#include<stdio.h>
#include "helper.h"

void array_to_string(const int *arr , const int arr_size , char *str ,  int str_size){
		int offset = 0;
		for(int j = 0; j < arr_size; j++){
			int written = snprintf(str+offset , str_size-offset , (j < arr_size-1) ? "%d," : "%d" , arr[j]);
			offset += written;
		}
}