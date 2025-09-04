#include<iostream>
#include "helper.h"
#include "quine.h"
using namespace std;

// better and faster
void prime_implicants(int variables){
	
	for (int i = 0; i <= variables; i++) {
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


/*
void prime_implicants(int variables){
	
	for (int i = 0; i <= variables; i++) {
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){
				
			if(group[i].combined[j] == false){
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
	 
	//clearing out commons
	table2 temp;
	temp.count = 0;
	
	for(int i = 0; i < prime.count; i++){
		 
		int duplicate = 0;
		for(int j = 0; j < temp.count; j++){
			if(temp.binary[j] == prime.binary[i]){	
				duplicate++;
				break;
			}
		}
		
		if(duplicate == 0){
			temp.binary[temp.count] = prime.binary[i];
			for(int x = 0; x < prime.mintermCount[i]; x++)
				temp.minterms[temp.count][x] = prime.minterms[i][x];
			temp.mintermCount[temp.count] = prime.mintermCount[i];
			temp.count++;
		}
		
	}
	prime = temp;
}
*/

