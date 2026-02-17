#include<string.h>
#include<stdlib.h>
#include "quine.h" // groupData struture

static int find_string(char **arr, int size, const char item[]);

groupData* getReducedTable(groupData *group , int var){

    groupData *newGroup = calloc(var+1 , sizeof(*newGroup));
	if(!newGroup) return NULL;

	int *mergedArray = NULL;
	char *reducedBinary = NULL;

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
					reducedBinary = strdup(group[i].binary[a]);
					if(!reducedBinary) goto FAIL;
					reducedBinary[pos] = '-';

					// To check whether the current newGroup binary is redundant
					int exist = find_string(newGroup[i].binary, newGroup[i].count, reducedBinary);
					if(exist) free(reducedBinary);
					else{

						//allocating groupData items
						if(newGroup[i].capacity == 0){
							if(allocate(&newGroup[i] , 4))
								goto FAIL;
						}
						else if(newGroup[i].count >= newGroup[i].capacity){
							int new_cap = (newGroup[i].capacity) * 2;
							if(allocate(&newGroup[i] , new_cap))
								goto FAIL;
						}

						//allocating memory minterm array
						int size = group[i].mintermCount[a] + group[i+1].mintermCount[b];
						mergedArray = malloc(size * sizeof(*mergedArray));
						if(!mergedArray) goto FAIL;

						//merge minterms into new merged minterm array
						int mCount = 0;
						for (int m = 0; m < group[i].mintermCount[a]; m++)
							mergedArray[mCount++] = group[i].minterms[a][m];
						for (int m = 0; m < group[i+1].mintermCount[b]; m++)
							mergedArray[mCount++] = group[i+1].minterms[b][m];

						//inserting groupData items
						int idx = newGroup[i].count;
						newGroup[i].binary[idx] = reducedBinary; reducedBinary = NULL;
						newGroup[i].minterms[idx] = mergedArray; mergedArray = NULL;
						newGroup[i].mintermCount[idx] = mCount;
						newGroup[i].combined[idx] = 0;
						newGroup[i].count++;
					}

					group[i].combined[a] = 1;
					group[i+1].combined[b] = 1;
					anyReduce = 1;;
				}
            }
        }
    }

	if(!anyReduce){     // if no new reduction, return old group
		free(newGroup);
		return group;
	}
    else return newGroup;

	FAIL:
		free(reducedBinary);
		free(mergedArray);
		for(int i = 0; i < var+1; i++)
			clear_quine(&newGroup[i]);
		free(newGroup);

		return NULL;

}

static int find_string(char **arr, int size, const char item[]) {
    for (int i = 0; i < size; i++) {
        if(strcmp(arr[i], item) == 0) return 1;
    }
    return 0;
}
