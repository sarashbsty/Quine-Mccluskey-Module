#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include "quine.h" // Group struture

static int find_binary(Implicant *implicants, int size, const char item[]);

Group* getReducedGroups(Group *groups , int var){

    Group *newGroups = calloc(var+1 , sizeof(*newGroups));
	if(!newGroups) return NULL;

	int *mergedArray = NULL;
	char *reducedBinary = NULL;

	int anyReduce = 0;
    // Compare groups[i] with groups[i+1]
    for (int i = 0; i < var; i++) {
        for (int a = 0; a < groups[i].count; a++) {
            for (int b = 0; b < groups[i+1].count; b++) {

				//let groups[i] be A and groups[i+1] be B
				Implicant *implicantsOfA = groups[i].implicants;
				Implicant *implicantsOfB = groups[i+1].implicants;

                // Compare binaries
                int diff = 0, pos = -1;
                for (int x = 0; x < var && diff < 2; x++) {
                    if (implicantsOfA[a].binary[x] != implicantsOfB[b].binary[x]) {
                        diff++;
                        pos = x;
                    }
                }

				//Only if diff is 1 , those two Binary and its Minterms will be combined
                if (diff == 1){

					//Building Reduced Binary
					reducedBinary = strdup(implicantsOfA[a].binary);
					if(!reducedBinary) goto FAIL;
					reducedBinary[pos] = '-';

					// To check whether the current newGroup binary is redundant
					int exist = find_binary(newGroups[i].implicants, newGroups[i].count, reducedBinary);
					if(exist) free(reducedBinary);
					else
					{
						int count = newGroups[i].count;
						int cap = newGroups[i].capacity;

						//allocating implicants
						if(count >= cap){
							cap = (!cap) ? 2 : cap * 2;
							Implicant *tmp = realloc(newGroups[i].implicants , cap * sizeof(*tmp));
							if(!tmp) goto FAIL;
							newGroups[i].implicants = tmp;
							newGroups[i].capacity = cap;
						}

						//allocating memory minterm array
						int size = implicantsOfA[a].mintermCount + implicantsOfB[b].mintermCount;
						mergedArray = malloc(size * sizeof(*mergedArray));

						if(!mergedArray) goto FAIL;

						//merge minterms into new merged minterm array
						int mCount = 0;
						for (int m = 0; m < implicantsOfA[a].mintermCount; m++)
							mergedArray[mCount++] = implicantsOfA[a].minterms[m];
						for (int m = 0; m < implicantsOfB[b].mintermCount; m++)
							mergedArray[mCount++] = implicantsOfB[b].minterms[m];

						//inserting Group items
						int idx = count;
						newGroups[i].implicants[idx].binary = reducedBinary;
						newGroups[i].implicants[idx].minterms = mergedArray;
						newGroups[i].implicants[idx].mintermCount = mCount;
						newGroups[i].implicants[idx].isCombined = false;
						newGroups[i].count++;

						reducedBinary = NULL;
						mergedArray = NULL;
					}

					implicantsOfA[a].isCombined = true;
					implicantsOfB[b].isCombined = true;
					anyReduce = 1;;
				}
            }
        }
    }

	if(!anyReduce){     // if no new reduction, return old groups
		free(newGroups);
		return groups;
	}
    else return newGroups;

	FAIL:
		free(reducedBinary);
		free(mergedArray);
		for(int i = 0; i < var+1; i++)
			Group_destroy(&newGroups[i]);
		free(newGroups);

		return NULL;

}

static int find_binary(Implicant *implicants, int size, const char item[]) {
    for (int i = 0; i < size; i++) {
        if(strcmp(implicants[i].binary, item) == 0) return 1;
    }
    return 0;
}
