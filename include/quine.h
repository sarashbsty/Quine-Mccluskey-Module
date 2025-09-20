#pragma once

typedef struct {
    int count;
    char binary[2000][100];
    int minterms[2000][100];
    int mintermCount[2000];
    int combined[2000];
} quine;

void fill_group_table(quine group[] , int Mid_terms[], int n_terms, int variables);
void displayGroups(quine group[] , int variables);
int reduce_table(quine group[] , quine reduce[] , int variables);
void prime_implicants(quine group[] , quine *prime , int variables);
void display_implicants(const quine *prime);
void essential_implicants( quine *prime , char arr[][100][6] , int min_terms[] , int min_count, char result[] , int size);
void display_essential_table(const quine *prime , char arr[][100][6] , int min_terms[] , int min_count);
