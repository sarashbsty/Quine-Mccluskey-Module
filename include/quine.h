#pragma once
typedef struct {
    int count;
	int capacity;
    char **binary;
    int **minterms;
    int *mintermCount;
    int *combined;
	int *minimal;
	char **expression;
} quine;

void fill_group_table(quine *group , int *minterms , int n_terms, int var);
void displayGroups(quine *group , int var);
int reduce_table(quine *group , quine *reduce , int var);
void prime_implicants(quine *group , quine *prime , int var);
void displayPi(const quine *prime);
void createPiChart(quine *prime , char ***table, int minterms[] , int min_count);
void displayPiChart(const quine *prime , char ***table , int *minterms , int min_count);
int checkCoverage(quine *prime , int *uncovered_terms, int minterms[] , int min_count);
int set_minimizer(quine *prime , int *uncovered_terms , int *uncovered_count);
int column_domination(quine *prime, char ***table ,int *uncovered_terms ,int uncovered_count);
void petrick(quine *prime , char ***table , int *uncovered_terms , int uncovered_count);
void printResult(const quine *prime, int var);
