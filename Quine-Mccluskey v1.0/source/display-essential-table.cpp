#include<iostream>
#include <unordered_map>
#include<iomanip>
#include "quine.h" // quine struture
using namespace std;

void display_essential_table(const quine &prime , string arr[100][100] , unordered_map<int, int> dict){
	
	int col = dict.size();
	int row = prime.count;
	int s = 5;
	int line = col*s;
	
	cout << string(18+line , '-') << "\n";
	
	cout<<"|                ";  // 13 spaces
	for(int i = 0; i < col; i++) 
		cout << setw(s) << dict[i];
	
	cout << "|\n" <<string(18+line , '-');
	
	for(int i = 0; i < row; i++){
		
		string str = prime.binary[i] + " [" + char('A'+i) + "]  | ";
		cout<< "\n|  " << str;
		for(int j = 0; j < col; j++)
			cout << setw(s) << arr[i][j];
		cout << " |\n" <<string(18+line , '-');
	}
	
}