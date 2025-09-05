#include<iostream>
#include "quine.h" // quine struture
using namespace std;

int reduce_table(quine group[] , quine reduced[] , int variables){
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