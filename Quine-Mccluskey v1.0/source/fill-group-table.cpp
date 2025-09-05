#include<iostream>
#include "quine.h" // quine struture
#include "helper.h"
using namespace std;


void fill_group_table(quine group[] , int Mid_terms[], string Binary[], int n_terms, int variables){
	for(int i = 0; i <= variables; i++){
		group[i].count = 0; // initialize
		
		for(int j = 0; j < n_terms; j++){
			
			if(count_1s(Binary[j]) == i){ 
			
				int &index = group[i].count;
				group[i].binary[index] = Binary[j];
				group[i].minterms[index][0] = Mid_terms[j];
				group[i].mintermCount[index] = 1;
				index++;
			}
		}
	}
}