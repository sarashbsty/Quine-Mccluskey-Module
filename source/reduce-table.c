#include "memory_tracker.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "helper.h"
#include "quine.h" // quine struture

int reduce_table(quine group[] , quine reduced[] , int var){

    int newCount = 0;

	//initialize reduce group
    for (int i = 0; i <= var; i++){
        reduced[i].count = 0;
		reduced[i].binary = NULL;
	}

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
					char *new_binary = malloc((var+1) * sizeof(*new_binary));
					if(!new_binary) { printf("\nERROR: Building reduced Binary Failed | Low Memory | reduce-table\n"); exit(0); }
					strcpy(new_binary , group[i].binary[a]);
					new_binary[pos] = '-';

					// To check whether the current reduced binary is redundant
					int check = is_exist(reduced[i].binary, new_binary, reduced[i].count);
					if(check == 1) free(new_binary);
					else{
						int idx = reduced[i].count;

						//allocating memory for new_binary
						char **temp = realloc(reduced[i].binary , (idx+1)*sizeof(*temp));
						if(!temp){ printf("\nERROR: reduced binary array allocation failed | Low memory | reduced-table\n"); exit(0); }
						reduced[i].binary = temp;

						//inserting new_binary to reduce
						reduced[i].binary[idx] = new_binary;

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
				}
            }
        }
    }
    return newCount; // how many min terms combined. 0 -> no min terms combined means no futher reduction
}
