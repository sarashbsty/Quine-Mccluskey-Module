#pragma once
#include<stdio.h>
#include<stdlib.h>

int digit(int x);
char** ToBinary(int minterms[] , int ,int);
char** make_line(int width[] , int , const char* , int);
int count_1s(char binary[]);
void Expression(char binary[]);
int is_exist(char (*arr)[100], const char item[] , int);
int get_minterms(int min_terms[] , int , int);
void array_to_string(const int *arr , const int , char *str , int);
char ***create_table(int, int, int);

static inline void clear_input_buffer(){
	int c;
    while ((c = getchar()) != '\n' && c != EOF);  //clear stdin
}

static inline void free_pointer_array(char** arr, int size){
	if(arr == NULL) return;
	for(int i = 0; i < size; i++)
		free(arr[i]);
	free(arr);
}
