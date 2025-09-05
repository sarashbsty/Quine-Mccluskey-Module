#include<iostream>
#include "quine.h" // quine struture
using namespace std;

void display_implicants(const quine &prime){
	cout<<"\n\nPrime Implicants:";
	cout<<"\n\nBinary \t\tminterms\n";
	cout << string(25, '-');
	for(int i = 0; i < prime.count; i++){
		cout<<"\n"<<prime.binary[i]<<"\t  |\t";
		for(int j = 0; j < prime.mintermCount[i]; j++){
			cout<<prime.minterms[i][j];
			if(j < prime.mintermCount[i]-1)
				cout<<",";
		}
	}
}