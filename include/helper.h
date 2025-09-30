#pragma once
#include "memory_tracker.h"

#include<stdio.h>
#include<stdlib.h>
#include "quine.h"

int digit(int x);
char* get_input();
char* Expression(const char *binary);
char* array_to_string(const int *arr , const int arr_size , const char *format);
char* ToBinary(int minterm , int var);
char** make_line(int *width , int widthcount , const char* style , int byte);
char ***create_table(int row , int col , int n);
int count_1s(char *binary);
int is_exist(char **arr, const char item[], int size);
int get_minterms(int *minterms , int index, int max_terms);
int allocate(quine *var , int size);

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

static inline void free_2d_int_pointer(int **arr, int size){
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

static inline void clear_quine(quine *var){
	if(!var) return;
	if(var->binary){ free_2d_pointer(var->binary , var->count); var->binary = NULL; }
	if(var->minterms){ free_2d_int_pointer(var->minterms , var->count); var->minterms = NULL; }
	if(var->mintermCount){ free(var->mintermCount); var->mintermCount = NULL; }
	if(var->combined) { free(var->combined); var->combined = NULL; }
	var->count = 0;
}

static inline void init_quine(quine *var){
	if(!var) return;
	var->count = 0;
	var->binary = NULL;
	var->minterms = NULL;
	var->mintermCount = NULL;
	var->combined = NULL;
}

