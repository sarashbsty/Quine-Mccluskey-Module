#include<string.h>
#include "quine.h" // quine struture

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
					
					int idx = reduced[i].count;
					strcpy(reduced[i].binary[idx] , group[i].binary[a]);
					reduced[i].binary[idx][pos] = '-';
                    
                    // Merge minterm
                    int mCount = 0;
                    for (int m = 0; m < group[i].mintermCount[a]; m++)
                        reduced[i].minterms[idx][mCount++] = group[i].minterms[a][m];
                    for (int m = 0; m < group[i+1].mintermCount[b]; m++)
                        reduced[i].minterms[idx][mCount++] = group[i+1].minterms[b][m];

                    reduced[i].mintermCount[idx] = mCount;
                    reduced[i].combined[idx] = 0;
                    reduced[i].count++;

                    group[i].combined[a] = 1;
                    group[i+1].combined[b] = 1;

                    newCount++;
                }
            }
        }
    }

    return newCount; // how many min terms combined. 0 -> no min terms combined means no futher reduction
}