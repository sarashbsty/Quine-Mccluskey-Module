#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "helper.h"
#include "quine.h" // quine struture

int reduce_table(quine group[] , quine reduced[] , int var){

    int newCount = 0;

	//initialize reduce group
    for (int i = 0; i <= var; i++)
       init_quine(&reduced[i]);

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

				//Only if diff is 1 , those two Binary and its Minterms will be combined
                if (diff == 1){

					//Building Reduced Binary
					char *reducedBinary = malloc((var+1) * sizeof(*reducedBinary));
					if(!reducedBinary) { printf("\nERROR: Building reduced Binary Failed | Low Memory | reduce-table\n"); exit(0); }
					strcpy(reducedBinary , group[i].binary[a]);
					reducedBinary[pos] = '-';

					// To check whether the current reduced binary is redundant
					int check = find_string(reduced[i].binary, reduced[i].count, reducedBinary);
					if(check != -1) free(reducedBinary);
					else{

						//allocating quine items
						if(reduced[i].capacity == 0){
							int flag = allocate(&reduced[i] , 4);
							if(flag) { printf("\nERROR: quine items allocation failed | Low memory | reduced-table\n"); exit(0); }
						}
						else if(reduced[i].count >= reduced[i].capacity){
							int new_cap = (reduced[i].capacity) * 2;
							int flag = allocate(&reduced[i] , new_cap);
							if(flag) { printf("\nERROR: quine items allocation failed | Low memory | reduced-table\n"); exit(0); }
						}

						//allocating memory minterm array
						int size = group[i].mintermCount[a] + group[i+1].mintermCount[b];
						int *mergedArray = malloc(size * sizeof(*mergedArray));
						if(!mergedArray) { printf("\nERROR: minterm array creation failed | Low memory | reduced-table\n"); exit(0); }

						//merge minterms into new merged minterm array
						int mCount = 0;
						for (int m = 0; m < group[i].mintermCount[a]; m++)
							mergedArray[mCount++] = group[i].minterms[a][m];
						for (int m = 0; m < group[i+1].mintermCount[b]; m++)
							mergedArray[mCount++] = group[i+1].minterms[b][m];

						//inserting quine items
						int idx = reduced[i].count;
						reduced[i].binary[idx] = reducedBinary;
						reduced[i].minterms[idx] = mergedArray;
						reduced[i].mintermCount[idx] = mCount;
						reduced[i].combined[idx] = 0;
						reduced[i].expression[idx] = NULL;
						reduced[i].count++;
					}

					group[i].combined[a] = 1;
					group[i+1].combined[b] = 1;
					newCount++;
				}
            }
        }
    }
    return newCount; // how many min terms combined. 0 -> no min terms combined means no futher reduction
}
