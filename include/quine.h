#pragma once
typedef struct {
    int count;
    char **binary;
    int **minterms;
    int *mintermCount;
    int *combined;
} quine;

void fill_group_table(quine *group , int *minterms , int n_terms, int var);
void displayGroups(quine *group , int var);
int reduce_table(quine *group , quine *reduce , int var);
void prime_implicants(quine *group , quine *prime , int var);
void display_implicants(const quine *prime);
char* essential_implicants(quine *prime , char ***arr , int *minterms , int min_count);
void display_essential_table(const quine *prime , char ***arr , int *minterms , int min_count);
