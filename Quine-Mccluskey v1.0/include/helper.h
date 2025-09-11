#pragma once
//note: is_exist() not fully sure

void ToBinary(char binary[][100] , int minterms[] , int ,int);
int count_1s(char binary[]);
void Expression(char binary[]);
int is_exist(char arr[][100], const char item[] , int);
int get_minterms(int min_terms[] , int start , int max_terms);
