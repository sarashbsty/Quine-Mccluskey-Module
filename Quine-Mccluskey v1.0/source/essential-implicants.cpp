#include<iostream>
#include <unordered_map>
#include "quine.h" // quine struture
#include "helper.h"
using namespace std;

int essential_implicants(const quine &prime , string arr[100][100] , unordered_map<int, int> dict, string result[100]){
	
	// table index for column is given by mid_terms[]
	//all zero initialize
	for(int i = 0; i < prime.count; i++)
		for(int j = 0; j < dict.size(); j++)
			arr[i][j] = " 0";
	
	//selective entering one
	for(int i = 0; i < prime.count; i++){
		for(int x = 0; x < prime.mintermCount[i]; x++){
			int key = prime.minterms[i][x];
			arr[i][dict[key]] = " 1";
		}
	}
	
	// note : extra space in " 1" and " 0" is given to align with "(1)" in below.
	
	//Finding the essential implicant by finding column with only one '1' and the prime implecant in that 1's row
	int count = 0,index, ones;
	for(int j = 0; j < dict.size(); j++){
		
		ones = 0;
		for(int i = 0; i < prime.count; i++){
			if(arr[i][j] == " 1"){
				ones++;
				index = i;
			}
		}
		
		if(ones == 1){ 
			arr[index][j] = "(1)";
			
			// checking ones is only 1 and duplicates
			if ( !is_exist(result, count , prime.binary[index]) )
				result[count++] = prime.binary[index];
		}
			
	}
	
	return count;
}