#include<iostream>
#include<iomanip>
#include "quine.h" // quine struture
using namespace std;

void displayGroups(quine group[] , int variables){
	
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