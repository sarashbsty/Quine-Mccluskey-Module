#pragma once
#include<stdio.h>

void ToBinary(char binary[][100] , int minterms[] , int ,int);
int count_1s(char binary[]);
void Expression(char binary[]);
int is_exist(char arr[][100], const char item[] , int);
int get_minterms(int min_terms[] , int start , int max_terms);
void make_line(char str[] , const char* c , int n , int byte);

static inline void clear_input_buffer(){
	int c;
    while ((c = getchar()) != '\n' && c != EOF);  //clear stdin
}
