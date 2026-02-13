#include<stdlib.h>
#include<string.h>
#include "quine.h" // quine struture
#include "int_array_dup.h"

static char* Expression(const char *binary);

int getPrimeImplicants(quine *group , quine *prime , int var){

	char *bin = NULL;
	char *expres = NULL;
	int *arr = NULL;

	for (int i = 0; i <= var; i++){
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){

			if(group[i].combined[j] == 1) continue; //skip Combined Binaries

			//allocating quine items
			if(prime->capacity == 0){
				if(allocate(prime , 4))
					goto FAIL;
			}
			else if(prime->count >= prime->capacity){
				int new_cap = prime->capacity + 5;
				if(allocate(prime , new_cap))
					goto FAIL;
			}

			//duplicate binary
			bin = strdup(group[i].binary[j]);
			if(!bin) goto FAIL;

			//duplicare int array
			arr = intDupArr(group[i].minterms[j], group[i].mintermCount[j]);
			if(!arr) goto FAIL;

			//get expression string from binary
			expres = Expression(group[i].binary[j]);
			if(!expres) goto FAIL;

			//Inserting quine items
			int index = prime->count;
			prime->binary[index] = bin;
			prime->expression[index] = expres;
			prime->minterms[index] = arr;
			prime->mintermCount[index] = group[i].mintermCount[j];
			prime->minimal[index] = 0;
			prime->cost[index] = strlen(expres);
			prime->count++;

			bin = NULL;
			expres = NULL;
			arr = NULL;
        }
	}
	return 0;

	FAIL:
		free(bin);
		free(arr);
		free(expres);
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
		char var = 'A'+i;
		if(binary[i] == '0') { str[count++] = var; str[count++] = '\''; }
		else if(binary[i] == '1') str[count++] = var;
		else continue;
	}
	str[count] = '\0';
	return str;
}
