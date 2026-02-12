#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture

static int find_string(char **arr, int size, const char item[]);

int reduce_table(quine *group , quine *newGroup, int var){

    int anyReduce = 0;

    // Compare group[i] with group[i+1]
    for (int i = 0; i < var; i++) {
        for (int a = 0; a < group[i].count; a++) {
            for (int b = 0; b < group[i+1].count; b++) {

                // Compare binaries
                int diff = 0, pos = -1;
                for (int x = 0; x < var && diff < 2; x++) {
                    if (group[i].binary[a][x] != group[i+1].binary[b][x]) {
                        diff++;
                        pos = x;
                    }
                }

				//Only if diff is 1 , those two Binary and its Minterms will be combined
                if (diff == 1){

					//Building Reduced Binary
					char *reducedBinary = strdup(group[i].binary[a]);
					if(!reducedBinary) return -1;
					reducedBinary[pos] = '-';

					// To check whether the current newGroup binary is redundant
					int exist = find_string(newGroup[i].binary, newGroup[i].count, reducedBinary);
					if(exist) free(reducedBinary);
					else{

						//allocating quine items
						if(newGroup[i].capacity == 0){
							if(allocate(&newGroup[i] , 4))
								return -1;
						}
						else if(newGroup[i].count >= newGroup[i].capacity){
							int new_cap = (newGroup[i].capacity) * 2;
							if(allocate(&newGroup[i] , new_cap))
								return -1;
						}

						//allocating memory minterm array
						int size = group[i].mintermCount[a] + group[i+1].mintermCount[b];
						int *mergedArray = malloc(size * sizeof(*mergedArray));
						if(!mergedArray) return -1;

						//merge minterms into new merged minterm array
						int mCount = 0;
						for (int m = 0; m < group[i].mintermCount[a]; m++)
							mergedArray[mCount++] = group[i].minterms[a][m];
						for (int m = 0; m < group[i+1].mintermCount[b]; m++)
							mergedArray[mCount++] = group[i+1].minterms[b][m];

						//inserting quine items
						int idx = newGroup[i].count;
						newGroup[i].binary[idx] = reducedBinary;
						newGroup[i].minterms[idx] = mergedArray;
						newGroup[i].mintermCount[idx] = mCount;
						newGroup[i].combined[idx] = 0;
						newGroup[i].expression[idx] = NULL;
						newGroup[i].count++;
					}

					group[i].combined[a] = 1;
					group[i+1].combined[b] = 1;
					anyReduce = 1;;
				}
            }
        }
    }
    return anyReduce; // how many min terms combined. 0 -> no min terms combined means no futher reduction
}

static int find_string(char **arr, int size, const char item[]) {
    for (int i = 0; i < size; i++) {
        if(strcmp(arr[i], item) == 0) return 1;
    }
    return 0;
}
