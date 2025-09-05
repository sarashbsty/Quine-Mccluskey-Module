/*
	Copyright (c) 2025 Sarash Basumatary 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

// bugs found : after entering minterms input, instead of ending it with -1 but with *1, all the leftover space of array is filled with 0

#include<iostream>
#include<math.h>
#include <iomanip>

#include "quine.h" // quine struture and its functions
#include "helper.h"
using namespace std;
					
int main(){
	int var , essential_table[100][100] , min_terms[10000] , min_count = 0, dont_care_count = 0 , temp;
	
	// data input start
	cout<<"Enter no. of variables : ";
	cin>>var;
	
	cout<<"Enter min terms (-1 to end) : ";
	while(min_count < pow(2,var)){
		cin>>temp;
		if(temp == -1) break;
		min_terms[min_count++] = temp;
	}
	
	cout<<"Enter dont care (-1 to end) : ";
	while((min_count+dont_care_count) < pow(2,var)){
		cin>>temp;
		if(temp == -1) break;
		min_terms[min_count + dont_care_count] = temp;
		dont_care_count++;
	}
	
	int n_terms = min_count + dont_care_count;
	string binary[10000];
	for(int i = 0; i < n_terms; i++) binary[i] = ToBinary(min_terms[i],var);
	
	
	cout<<"\n\n"<< min_count<<" Mid terms : "; for(int i=0;i<min_count;i++) cout<<min_terms[i]<<" ";
	cout<< "\n" << dont_care_count <<" Dont care : "; for(int i = min_count; i < n_terms; i++) cout<<min_terms[i]<<" ";
	
	//data input stops
	
	string result[100];
	static quine group[100], reduced[100] , prime;  // stack memory very low (8mb). group[100] -> 30mb
	
	fill_group_table(group, min_terms, binary, n_terms, var);
	
	int CanReduce = reduce_table(group, reduced, var);
	prime_implicants(group, prime, var); // get prime-implicant from the Uncombineds
	cout << "\n\nInitial Grouping:\n";
    displayGroups(group, var);
	
	
	int i = 1;
    while(CanReduce){
		for (int i = 0; i <= var; i++) group[i] = reduced[i];
        CanReduce = reduce_table(group, reduced, var);
		prime_implicants(group, prime, var);
		
		cout << "\n" << i++ <<"th Reduction: \n";
        displayGroups(group, var);	
    }	
	
	display_implicants(prime);
	
	int iterate = essential_implicants(prime, essential_table, min_terms, min_count, result);
	
	cout<<"\n\n\nTable to find Essential prime Implicants: \n\n";
	display_essential_table(prime, essential_table, min_terms, min_count);
	
	cout<<"\n\nEssential Prime Implicants : ";
	for(int i = 0; i < iterate; i++)
		cout<<result[i]<<"  ";
	cout<<endl;
	
	cout<<"\nresult : Y ";
	for(int i = 0; i < var; i++)
		cout << (i ? "," : "(") << char('A'+i);    //print: A,B,C,....
	for(int i = 0; i < iterate; i++)
		cout << (i ? " + " : ") = ") << Expression(result[i]);
	cout<<"\n\n";
	
	return 0;
}
	
			
	
	