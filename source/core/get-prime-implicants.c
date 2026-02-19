#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "quine.h" // groupData struture
#include "int_array_dup.h"

static char* Expression(const char *binary);

int getPrimeImplicants(primeData **primePtr ,groupData **tables, int *groupSize, int tablesCount){

	char *bin = NULL;
	char *expres = NULL;
	int *arr = NULL;

	primeData *prime = *primePtr;
	int primeCount = 0;
	int primeCap = 0;


	for(int i = 0; i < tablesCount; i++)
	{
		groupData *group = tables[i];
		int groupCount = groupSize[i];
		for (int x = 0; x < groupCount; x++)
		{
			if (group[x].count == 0) continue; // skip empty groups

			for (int y = 0; y < group[x].count; y++){

				if(group[x].isCombined[y]) continue; //skip Combined Binaries

				if(primeCount >= primeCap){
					primeCap = (!primeCap) ? 2 : primeCap*2;
					primeData *tmp = realloc(prime , primeCap * sizeof(*tmp));
					if(!tmp) goto FAIL;
					prime = tmp;
				}

				//duplicate binary
				bin = strdup(group[x].binary[y]);
				if(!bin) goto FAIL;

				//duplicare int array
				arr = intDupArr(group[x].minterms[y], group[x].mintermCount[y]);
				if(!arr) goto FAIL;

				//get expression string from binary
				expres = Expression(bin);
				if(!expres) goto FAIL;

				//Inserting groupData items
				int idx = primeCount;
				prime[idx].binary = bin;
				prime[idx].expression = expres;
				prime[idx].minterms = arr;
				prime[idx].mintermCount = group[x].mintermCount[y];
				prime[idx].isEssential = false;
				prime[idx].cost = strlen(expres);
				primeCount++;

				bin = NULL;
				expres = NULL;
				arr = NULL;
			}
		}
	}

	*primePtr = prime;
	return primeCount;

	FAIL:
		free(bin);
		free(arr);
		free(expres);

		destroyPrimeData(prime, primeCount);
		*primePtr = NULL;

		return -1;
}

static char* Expression(const char *binary){

	if(!binary) return NULL;

	//Finding required size
	int size = 1; //reserved for '\0'
	for(int i = 0; binary[i] != '\0'; i++){
		if(binary[i] == '0') size += 2;
		else if(binary[i] == '1') size++;
	}

	//for empty binary string or string with numbers other than 0 and 1
	if(size == 1){
		char *str = strdup("1");
		return str;
	}

	//Allocate
	char *str = malloc(size * sizeof(*str));
	if(!str) return NULL;

	//write
	int count = 0;
	for(int i = 0; binary[i] != '\0'; i++){
		char ch = 'A'+i;
		if(binary[i] == '0') { str[count++] = ch; str[count++] = '\''; }
		else if(binary[i] == '1') str[count++] = ch;
		else continue;
	}
	str[count] = '\0';
	return str;
}
