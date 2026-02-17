#include<stdlib.h>
#include<string.h>
#include "quine.h" // quine struture
#include "int_array_dup.h"

static char* Expression(const char *binary);

int getPrimeImplicants(primeData **primePtr , int *primeCountPtr, int *primeCapPtr, quine *group, int groupSize){

	char *bin = NULL;
	char *expres = NULL;
	int *arr = NULL;

	primeData *prime = *primePtr;
	int primeCount = *primeCountPtr;
	int primeCap = *primeCapPtr;

	for (int i = 0; i <= groupSize; i++){
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){

			if(group[i].combined[j] == 1) continue; //skip Combined Binaries

			if(primeCount >= primeCap){
				primeCap *= 2;
				primeData *tmp = realloc(prime , primeCap * sizeof(*tmp));
				if(!tmp) goto FAIL;
				prime = tmp;
			}

			//duplicate binary
			bin = strdup(group[i].binary[j]);
			if(!bin) goto FAIL;

			//duplicare int array
			arr = intDupArr(group[i].minterms[j], group[i].mintermCount[j]);
			if(!arr) goto FAIL;

			//get expression string from binary
			expres = Expression(bin);
			if(!expres) goto FAIL;

			//Inserting quine items
			int idx = primeCount;
			prime[idx].binary = bin;
			prime[idx].expression = expres;
			prime[idx].minterms = arr;
			prime[idx].mintermCount = group[i].mintermCount[j];
			prime[idx].isEssential = 0;
			prime[idx].cost = strlen(expres);
			primeCount++;

			bin = NULL;
			expres = NULL;
			arr = NULL;
        }
	}

	*primePtr = prime;
	*primeCountPtr = primeCount;
	*primeCapPtr = primeCap;

	prime = NULL;

	return 0;

	FAIL:
		free(bin);
		free(arr);
		free(expres);

		destroyPrimeData(prime, primeCount);

		*primePtr = NULL;
		*primeCountPtr = 0;
		*primeCapPtr = 0;

		return 1;
}

static char* Expression(const char *binary){

	//Finding required size
	int size = 1; //reserved for '\0'
	for(int i = 0; binary[i] != '\0'; i++){
		if(binary[i] == '0') size += 2;
		else if(binary[i] == '1') size++;
	}

	//for empty binary string or string with numbers other than 0 and 1
	if(size == 1){
		char *str = malloc(2);
		if(str) { str[0] = '1'; str[1] = '\0'; }
		return str;
	}

	//Allocate
	char *str = malloc(size * sizeof(*str));
	if(!str) return NULL;

	//write
	int count = 0;
	for(int i = 0; binary[i] != '\0'; i++){
		char groupSize = 'A'+i;
		if(binary[i] == '0') { str[count++] = groupSize; str[count++] = '\''; }
		else if(binary[i] == '1') str[count++] = groupSize;
		else continue;
	}
	str[count] = '\0';
	return str;
}
