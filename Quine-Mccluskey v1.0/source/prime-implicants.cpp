#include<iostream>
#include "quine.h" // quine struture
#include "helper.h"
using namespace std;

void prime_implicants(quine group[] , quine &prime , int variables){
	
	for (int i = 0; i <= variables; i++){
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){
			
			if(group[i].combined[j] == false){
				
				// to check whether the implicant is already in prime or not
				bool check = is_exist(prime.binary , prime.count , group[i].binary[j]);
				if(check == true) continue;
				
				int &index = prime.count;
				prime.binary[index] = group[i].binary[j];
				
				int x;  //copying midterms
				for(x = 0; x < group[i].mintermCount[j]; x++)
					prime.minterms[index][x] = group[i].minterms[j][x];
				
				prime.mintermCount[index] = x;
				index++;
			}
        }
	}	
}