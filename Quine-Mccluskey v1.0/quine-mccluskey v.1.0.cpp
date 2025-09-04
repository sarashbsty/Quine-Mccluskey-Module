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
using namespace std;

struct table1 {
    int No;                         
    int count;                       
    string binary[1000];             
    int minterms[1000][50];          
    int mintermCount[1000];          
    bool combined[1000];             
} group[100], reduced[100];

struct table2{
	int count;
	string binary[1000]; 
	int minterms[1000][50];
	int mintermCount[1000]; 
} prime;

string ToBinary(int minterm, int variables) {
    string binary = "";
    for (int i = variables - 1; i >= 0; i--) {
        binary += ((minterm >> i) & 1) ? '1' : '0';
    }
    return binary;
}

string Expression(const string &binary) {
    string expr = "";
    int variables = binary.size();  

    for (int i = 0; i < variables; i++) {
        char var = 'A' + i;   
        if (binary[i] == '0') {
            expr += var;
            expr += '\'';     // add complement
        } else if (binary[i] == '1') {
            expr += var;      // normal variable
        } else if (binary[i] == '-') {
            // don't care → skip this variable
            continue;
        }
    }
    return expr;
}

bool is_exist(string arr[], int size, string item) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == item) 
            return true;
    }
    return false;
}

int count_1s(const string &binary){
	int count = 0;
	for(int i = 0; i < binary.size(); i++)
		if(binary[i] == '1') count++;
	return count;
}

void displayGroups(int variables){
	
	int line = 2+8+2+20+2+20+1;
    
	// separator line at start
	cout << string(line, '-') << "\n";
	
	cout << left << setw(2)  << "|";
    cout << setw(8)  << "Group"   << setw(2) << "|"; 
	cout << setw(20) << "MinTerm" << setw(2) << "|";
	cout << setw(20) << "Binary"  << "|\n";
         
    for (int i = 0; i <= variables; i++) {
        if (group[i].count == 0) continue; // skip empty groups
		
		// separator line before every group
		cout << string(line, '-') << "\n";  
		
        for (int j = 0; j < group[i].count; j++) {
			
			cout << left <<setw(2)  << "|";
            if (j == 0) 
                cout << setw(8) << i ; // print group number only once
            else 
                cout << setw(8) << " ";
			cout << setw(2) << "|";

            // print all minterms associated with this binary
            string mintermStr = "";
            for (int k = 0; k < group[i].mintermCount[j]; k++) {
                mintermStr += to_string(group[i].minterms[j][k]);
                if (k < group[i].mintermCount[j] - 1) mintermStr += ","; 
            }
			if(group[i].combined[j] == false) mintermStr += " (x) ";

            cout << setw(20) << mintermStr;
			cout << setw(2) << "|";
			cout << setw(20) << group[i].binary[j];
			cout << "|\n";
                 
        }        
    }
	
	// separator line at end
    cout << string(line, '-') << "\n";
}

void display_implicants(){
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

void display_essential_table(int arr[100][100] , int min_terms[] , int min_count){
	
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
	
void group_table(int Mid_terms[], string Binary[], int n_terms, int variables){
	for(int i = 0; i <= variables; i++){
		group[i].No = i;
		int index = 0;
		for(int j = 0; j < n_terms; j++){
			if(count_1s(Binary[j]) == i){ 
				group[i].binary[index] = Binary[j];
				group[i].minterms[index][0] = Mid_terms[j];
				group[i].mintermCount[index] = 1;
				index++;
			}
		}
		group[i].count = index;
	}
}
	
int reduceGroups(int variables) {
    int newCount = 0;
	
    for (int i = 0; i <= variables; i++)
        reduced[i].count = 0;

    // Compare group[i] with group[i+1]
    for (int i = 0; i < variables; i++) {
        for (int a = 0; a < group[i].count; a++) {
            for (int b = 0; b < group[i+1].count; b++) {

                // Compare binaries
                int diff = 0, pos = -1;
                for (int x = 0; x < variables; x++) {
                    if (group[i].binary[a][x] != group[i+1].binary[b][x]) {
                        diff++;
                        pos = x;
                    }
                }

                if (diff == 1) {
                    string newBinary = group[i].binary[a];
                    newBinary[pos] = '-';
					
					int idx = reduced[i].count;
					reduced[i].binary[idx] = newBinary;
                    
                    // Merge minterm
                    int mCount = 0;
                    for (int m = 0; m < group[i].mintermCount[a]; m++)
                        reduced[i].minterms[idx][mCount++] = group[i].minterms[a][m];
                    for (int m = 0; m < group[i+1].mintermCount[b]; m++)
                        reduced[i].minterms[idx][mCount++] = group[i+1].minterms[b][m];

                    reduced[i].mintermCount[idx] = mCount;
                    reduced[i].combined[idx] = false;
                    reduced[i].count++;

                    group[i].combined[a] = true;
                    group[i+1].combined[b] = true;

                    newCount++;
                }
            }
        }
    }

    return newCount; // how many min terms combined. 0 -> no min terms combined means no futher reduction
}
		
void prime_implicants(int variables){
	for (int i = 0; i <= variables; i++) {
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){
			
			if(group[i].combined[j] == false){
				int index = prime.count;
				prime.binary[index] = group[i].binary[j];
				
				int x;
				for(x = 0; x < group[i].mintermCount[j]; x++)
					prime.minterms[index][x] = group[i].minterms[j][x];
				
				prime.mintermCount[index] = x;
				prime.count++;
			}
        }
	}
	 
	//clearing out commons
	table2 temp = {0}; 
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

int essential(int arr[100][100] , int min_terms[] , int min_count, string result[100]){
	
	//all zero initialize
	for(int i = 0; i < prime.count; i++)
		for(int x = 0; x < min_count; x++)
			arr[i][min_terms[x]] = 0;
	
	//selective entering one
	for(int i = 0; i < prime.count; i++){
		for(int x = 0; x < prime.mintermCount[i]; x++)
			arr[i][prime.minterms[i][x]] = 1;
	}
	
	//Finding the essential implicant by finding column with only one '1' and the prime implecant in that 1's row
	int count = 0,index, ones;
	for(int i = 0; i < min_count; i++){
		
		ones = 0;
		for(int x = 0; x < prime.count; x++){
			if(arr[x][min_terms[i]] == 1){
				ones++;
				index = x;
			}
		}
		
		// checking ones is only 1 and duplicates
		bool check = is_exist(result, count , prime.binary[index]);
		if(ones == 1 && check == false) 
			result[count++] = prime.binary[index];
	}
	
	return count;
}	
		
int main(){
	int var , essential_table[100][100] , min_terms[10000] , min_count = 0, dont_care_count = 0 , temp;
	string result[100];
	
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
	
	
	
	group_table(min_terms,binary,n_terms,var);
	
	int CanReduce = reduceGroups(var);
	prime_implicants(var); // get prime-implicant from the Uncombineds
	cout << "\n\nInitial Grouping:\n";
    displayGroups(var);
	
	
	int i = 1;
    while(CanReduce){
		for (int i = 0; i <= var; i++) group[i] = reduced[i];
        CanReduce = reduceGroups(var);
		prime_implicants(var);
		
		cout << "\n" << i++ <<"th Reduction: \n";
        displayGroups(var);	
    }	
	
	display_implicants();
	
	int iterate = essential(essential_table, min_terms, min_count, result);
	
	cout<<"\n\n\nTable to find Essential prime Implicants: \n\n";
	display_essential_table(essential_table,min_terms,min_count);
	
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
	
			
	
	