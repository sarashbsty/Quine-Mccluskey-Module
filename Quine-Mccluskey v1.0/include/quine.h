#pragma once
#include <unordered_map>
using namespace std;

struct quine{                   
    int count;                       
    string binary[1000];             
    int minterms[1000][50];          
    int mintermCount[1000];          
    bool combined[1000];   	
};


void fill_group_table(quine group[] , int Mid_terms[], string Binary[], int , int );
void displayGroups(quine group[] , int );
int reduce_table(quine group[] , quine reduce[] , int );
void prime_implicants(quine group[] , quine &prime , int ) ;
void display_implicants(const quine &prime);
int essential_implicants(const quine &prime , string arr[100][100] , unordered_map<int, int> dict, string result[100]);
void display_essential_table(const quine &prime , string arr[100][100] , unordered_map<int, int> dict);