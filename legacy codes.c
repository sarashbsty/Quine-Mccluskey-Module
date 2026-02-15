
char** ToBinary(int *minterms , int n_terms, int var){

	if (n_terms == 0 || var == 0) return NULL;

	char **binary = malloc(n_terms * sizeof(*binary)); // create array of n_terms pointers
	if(!binary) return NULL;

	for(int i = 0; i < n_terms; i++){
		char* str = malloc((var+1) * sizeof(*str));
		if(!str){
			free_2d_pointer(binary , i);
			return NULL;
		}

		for(int j = 0; j < var; j++)
			str[var-1-j] = ((minterms[i] >> j) & 1) ? '1' : '0';
		str[var] = '\0';
		binary[i] = str;
	}
	return binary;
}

int expression(string_arr *binary){
	for(int index = 0; index < binary->size; index++){

		size_t len = (2*strlen(binary->arr[index])) + 1;
		char *str = malloc(len);
		if(!str) return 0;

		int count = 0;
		for(int i = 0; binary->arr[index][i] != '\0'; i++){
			char var = 'A'+i;
			if(binary->arr[index][i] == '0') { str[count++] = var; str[count++] = '\''; }
			else if(binary->arr[index][i] == '1') str[count++] = var;
			else continue;
		}
		str[count] = '\0';
		free(binary->arr[index]);
		binary->arr[index] = str;
	}
	return 1;
}

char *str_exp = NULL;
int offset = 0;
for(int i = 0; i < prime.count; i++){

	if(prime.minimal[i] == 0) continue;

	//Binary to expression
	char *exp = Expression(prime.binary[i]);
	if(exp == NULL) { printf("\nERROR: Expression creation Failed | Low Memory | essential-implicants\n"); exit(0); }

	//Creating expression string
	int needed = snprintf(NULL , 0 , (offset) ? " + %s" : "%s" , exp);
	int new_capacity = offset+needed+1;
	char *temp = realloc(str_exp , new_capacity * sizeof(*temp));
	if(!temp) {
		printf("\nERROR: expression string creation fail | Low Memory | essential-implicants\n");
		free(str_exp);
		exit(0);
	}
	str_exp = temp;
	int written = snprintf(str_exp+offset , new_capacity-offset , (offset) ? " + %s" : "%s" , exp);
	offset += written;		free(exp);
}

char *result = str_exp;

printf("\n\nResult: Y(");
for (int i = 1; i <= var; i++){
	printf((i == var) ? "%c" : "%c,", 'A' + i);
printf(") = %s\n\n", (result ? result : "No result"));
free(result);

static inline void init_quine(quine *var){
	if(!var) return;
	var->count = 0;
	var->capacity = 0;
	var->binary = NULL;
	var->minterms = NULL;
	var->mintermCount = NULL;
	var->combined = NULL;
	var->minimal = NULL;
	var->expression = NULL;
	var->cost = NULL;
}



/*


static int isSubset(char* A, char* B){
	int count = 0;
	int sizeA = strlen(A);
	int sizeB = strlen(B);

	if(sizeA < sizeB){
		for(int a = 0; a < sizeA; a++){
			char x = A[a];
			if(strchr(B, x)) count++;
		}
		if(count == sizeA) return 1; // thus, A is subset of B
		else return 0;
	}

	else if(sizeB < sizeA){
		for(int b = 0; b < sizeB; b++){
			char x = B[b];
			if(strchr(A, x)) count++;
		}
		if(count == sizeB) return 2; //thus, B is subset of A
		else return 0;
	}

	else{
		int count[256] = {0};

		while(*A){ count[(unsigned char)*A]++; A++; }
		while(*B){ count[(unsigned char)*B]--; B++; }

		for(int i = 0; i < 256; i++)
			if(count[i] != 0)
				return 0; // means they are different set

		return 1; // means they are same set
	}
}

*/

static void displayProcess(char **SOP_terms, int SOP_count, char **POS_terms, int POS_count, int i){
	printf("\n\nP = (");
	for(int j = 0; j < SOP_count; j++){
		char *str = SOP_terms[j];
		if(j > 0) printf(" + ");
		while(*str){ printf("P%d" , 1+(*str-'A') ); str++; }
	}
	printf(")");

	for(int k = i+1; k < POS_count; k++){
		printf("·(");
		for(int x = 0; POS_terms[k][x] != '\0'; x++){
			char ch = POS_terms[k][x];
			printf((x == 0) ? "P%d" : " + P%d", 1+(ch-'A') );
		}
		printf(")");
	}
}

#include "memory_tracker.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

static int get_no_of_common_terms(int arr1[] , int size1 , int arr2[] , int size2) {
	int common = 0;
	for(int i = 0; i < size1; i++){
		for(int j = 0; j < size2; j++) {
			if(arr1[i] == arr2[j]){
				common++;
				break;
			}
		}
	}
	return common;
}

static void delete_at(int *arr, int *size, int pos){
    if (!arr || !size) return;
    if (pos < 0 || pos >= *size) return;

    for (int i = pos; i < (*size) - 1; i++)
        arr[i] = arr[i + 1];

    (*size)--;
}


int set_minimizer_greedy(quine *prime , int *uncovered_terms , int *uncovered_count){

	int uncoveredSize = *uncovered_count;
	while(1){
		int covers = 0, pos = -1;

		//determines the prime-implicant which covers most minterms in every iteration
		for(int i = 0; i < prime->count; i++){
			if(prime->minimal[i] == 1) continue;
			int new_covers = get_no_of_common_terms(prime->minterms[i] , prime->mintermCount[i] , uncovered_terms , uncoveredSize);
			if(new_covers > covers){
				pos = i;
				covers = new_covers;
			}
		}

		if(covers == 0) break;
 		else{
			//marking the prime implicant as minimal cover
			prime->minimal[pos] = 1;

			printf("\nSelected %s for minterms : ", prime->expression[pos]);

			//remove covered minterms from the array
			int idx = 0;
			while(idx < uncoveredSize){
				int check = find_int(prime->minterms[pos] , prime->mintermCount[pos] , uncovered_terms[idx]);
				if(check != -1){
					printf("%d ",uncovered_terms[idx]);
					delete_at(uncovered_terms, &uncoveredSize, idx);
				}
				else idx++;
			}
		}
	}

	*uncovered_count = uncoveredSize;
	if(*uncovered_count > 0) return 1;
	else return 0;
}



