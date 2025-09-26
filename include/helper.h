#pragma once
#include "memory_tracker.h"

#include<stdio.h>
#include<stdlib.h>

int digit(int x);
char* get_input();
char* Expression(const char *binary);
char* array_to_string(const int *arr , const int arr_size , const char *format);
char** ToBinary(int *minterms , int n_terms, int var);
char** make_line(int *width , int widthcount , const char* style , int byte);
char ***create_table(int row , int col , int n);
int count_1s(char *binary);
int is_exist(char (*arr)[100], const char *item, int size);
int get_minterms(int *minterms , int index, int max_terms);

static inline void clear_input_buffer(){
	int c;
    while ((c = getchar()) != '\n' && c != EOF);  //clear stdin
}

static inline void free_2d_pointer(char **arr, int size){
	if(arr == NULL) return;
	for(int i = 0; i < size; i++)
		free(arr[i]);
	free(arr);
}

static inline void free_3d_pointer(char ***arr, int row, int col){
    if(arr == NULL) return;
    for(int i = 0; i < row; i++)
        free_2d_pointer(arr[i], col);
    free(arr);
}

