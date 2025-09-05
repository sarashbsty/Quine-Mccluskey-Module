#pragma once
using namespace std;

struct quine{                   
    int count;                       
    string binary[1000];             
    int minterms[1000][50];          
    int mintermCount[1000];          
    bool combined[1000];   	
};

void fill_group_table(quine group[] , int Mid_terms[], string Binary[], int n_terms, int variables);
void displayGroups(quine group[] , int variables);
int reduce_table(quine group[] , quine reduce[] , int variables);
void prime_implicants(quine group[] , quine &prime , int variables);
void display_implicants(const quine &prime);
int essential_implicants(const quine &prime , int arr[100][100] , int min_terms[] , int min_count, string result[100]);
void display_essential_table(const quine &prime , int arr[100][100] , int min_terms[] , int min_count);
