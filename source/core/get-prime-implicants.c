#include<stdlib.h>
#include<string.h>
#include "quine.h" // quine struture

static char* Expression(const char *binary);
static int *intDupArr(const int *src, size_t n);

int getPrimeImplicants(quine *group , quine *prime , int var){

	for (int i = 0; i <= var; i++){
		if (group[i].count == 0) continue; // skip empty groups

        for (int j = 0; j < group[i].count; j++){

			if(group[i].combined[j] == 1) continue; //skip Combined Binaries

			//allocating quine items
			if(prime->capacity == 0){
				if(allocate(prime , 4))
					return 1;
			}
			else if(prime->count >= prime->capacity){
				int new_cap = prime->capacity + 5;
				if(allocate(prime , new_cap))
					return 1;
			}

			//duplicate binary
			char *bin = strdup(group[i].binary[j]);
			if(!bin) return 1;

			//duplicare int array
			int *arr = intDupArr(group[i].minterms[j], group[i].mintermCount[j]);
			if(!arr) return 1;

			//get expression string from binary
			char *expres = Expression(group[i].binary[j]);
			if(!expres)
				return 1;

			//Inserting quine items
			int index = prime->count;
			prime->binary[index] = bin;
			prime->expression[index] = expres;
			prime->minterms[index] = arr;
			prime->mintermCount[index] = group[i].mintermCount[j];
			prime->minimal[index] = 0;
			prime->cost[index] = strlen(expres);
			prime->count++;
        }
	}
	return 0;
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

static int *intDupArr(const int *src, size_t n){
    if (src == NULL || n == 0)
        return NULL;

    int *copy = malloc(n * sizeof *copy);
    if (!copy)
        return NULL;

    memcpy(copy, src, n * sizeof *copy);
    return copy;
}
