#include<iostream>
#include<iomanip>
#include "quine.h" // quine struture
using namespace std;

//check for improvemnt

void display_essential_table(const quine &prime , int arr[100][100] , int min_terms[] , int min_count){
	
	cout<<"    ";
	for(int i=0;i<min_count;i++) 
		cout << setw(4) << min_terms[i];
	
	cout << "\n" <<string(4+(min_count*4), '-');
	
	for(int i = 0; i < prime.count; i++){
		cout<< "\n" << setw(2) << char('A'+i) <<"| ";
		for(int x = 0; x < min_count; x++)
			cout << setw(4) << arr[i][min_terms[x]];
		
		cout << "\n" <<string(4+(min_count*4), '-');
	}
}