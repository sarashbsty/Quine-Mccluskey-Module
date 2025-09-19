#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "helper.h"
#include "quine.h" // quine struture

int reduce_table(quine group[] , quine reduced[] , int var){
	
    int newCount = 0;
	
    for (int i = 0; i <= var; i++)
        reduced[i].count = 0;

    // Compare group[i] with group[i+1]
    for (int i = 0; i < var; i++) {
        for (int a = 0; a < group[i].count; a++) {
            for (int b = 0; b < group[i+1].count; b++) {

                // Compare binaries
                int diff = 0, pos = -1;
                for (int x = 0; x < var; x++) {
                    if (group[i].binary[a][x] != group[i+1].binary[b][x]) {
                        diff++;
                        pos = x;
                    }
                }
				
				//Only if diff is 1 , those two Binary and its Minterms will be combined and added to Reduce
                if (diff == 1) {
					
					char *temp = malloc((var+1) * sizeof(*temp));
					if(temp == NULL) { printf("Reduce_table : Memory Allocation failed code: 101"); exit(0); }
					
					strcpy(temp , group[i].binary[a]);
					temp[pos] = '-';
					
					// Only if there is no duplicates, then the binary will be added
					int check = is_exist(reduced[i].binary, temp, reduced[i].count);
					if(check == 0){ 
					
						int idx = reduced[i].count;
						strcpy(reduced[i].binary[idx] , temp);
					
						// Merge minterm
						int mCount = 0;
						for (int m = 0; m < group[i].mintermCount[a]; m++)
							reduced[i].minterms[idx][mCount++] = group[i].minterms[a][m];
						for (int m = 0; m < group[i+1].mintermCount[b]; m++)
							reduced[i].minterms[idx][mCount++] = group[i+1].minterms[b][m];

						reduced[i].mintermCount[idx] = mCount;
						reduced[i].combined[idx] = 0;
						reduced[i].count++;
					}
					group[i].combined[a] = 1;
					group[i+1].combined[b] = 1;
					newCount++;
					free(temp);
                }
            }
        }
    }
    return newCount; // how many min terms combined. 0 -> no min terms combined means no futher reduction
}
