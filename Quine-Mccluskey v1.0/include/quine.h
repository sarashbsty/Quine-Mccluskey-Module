#pragma once

typedef struct {                   
    int count;                       
    char binary[1000][100];             
    int minterms[1000][50];          
    int mintermCount[1000];          
    int combined[1000];   	
} quine;

void fill_group_table(quine group[] , int Mid_terms[], char Binary[][100], int n_terms, int variables);
void displayGroups(quine group[] , int variables);
int reduce_table(quine group[] , quine reduce[] , int variables);
void prime_implicants(quine group[] , quine *prime , int variables);
void display_implicants(const quine *prime);
int essential_implicants(const quine *prime , char arr[100][100][6] , int min_terms[] , int min_count, char result[][100]);
void display_essential_table(const quine *prime , char arr[100][100][6] , int min_terms[] , int min_count);