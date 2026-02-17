#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture

int getSetCoverage(char*** returnPtr, int primeCount, int **table ,int *uncoveredTerms ,int uncoveredCount)
{
	char **setArr = malloc(uncoveredCount * sizeof(*setArr));
	if(!setArr) return -1;

	int max_literals = primeCount , count = 0;

	for(int i = 0; i < uncoveredCount; i++){
		char *term = malloc(sizeof(*term) * (max_literals+1));
		if(!term){ free_2d_pointer(setArr, i); return -1; }

		int offset = 0;
		for(int j = 0; j < primeCount; j++){
			if(table[j][uncoveredTerms[i]] != 0)
				term[offset++] = 'A'+j;
		}
		term[offset] = '\0';
		setArr[count++] = term;
	}

	*returnPtr = setArr;
	return count;
}
