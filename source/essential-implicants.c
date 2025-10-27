#include "memory_tracker.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

static int isexist(int *arr , int size, int item){
	for(int i = 0; i < size; i++)
		if(item == arr[i]) return i;
	return -1;
}

//check for improvemnt
char* essential_implicants(quine *prime , int minterms[] , int min_count , int var){

	// create essential_table
	char ***table = create_table(prime->count , pow(2,var) , 6);
	if(table == NULL){ printf("\nERROR: Table creation failed | Low Memory | essential_implicants\n"); exit(0); }

	//array to store covered minterms
	int *minterm_uncovered = malloc(pow(2,var) * sizeof(int));
	if(!minterm_uncovered) { printf("\nminterm_uncovered array allocation failed | low memory | essential_implicants\n"); exit(0); }
	int uncovered = 0;

	//all space initialize
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < min_count; j++)
			strcpy(table[i][minterms[j]] , " ");

	//selective marking X
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < prime->mintermCount[i]; j++)
			strcpy(table[i][prime->minterms[i][j]] , " X");

	//Finding the essential implicant by finding column(minterm) with only one 'X' and the prime implecant that covers it
	for(int j = 0; j < min_count; j++){

		int index ,ones = 0;
		for(int i = 0; i < prime->count; i++){
			if(strcmp(table[i][minterms[j]], " X") == 0){
				ones++;
				index = i;
			}
		}

		if(ones == 1){
			//marking for visual guide
			strcpy(table[index][minterms[j]] , "(X)" );

			//marked for minimal cover
			prime->minimal[index] = 1;
		}
	}

	printf("\n\n\nTable to find Essential prime Implicants: \n");
    display_essential_table(prime, table, minterms, min_count);

	//checking whether the essential implicants suffice and create a array of uncovered minterms
	for(int i = 0; i < min_count; i++){
		int check = -1;
		for(int j = 0; j < prime->count; j++){
			if(prime->minimal[j] == 1){
				check = isexist(prime->minterms[j] , prime->mintermCount[j] , minterms[i]);
				if(check != -1) break;
			}
		}
		if(check == -1) minterm_uncovered[uncovered++] = minterms[i];
	}

	if(uncovered == 0) printf("All Minterms covered by essential-implicants\n");
	else{
		printf("Minterms not covered : ");
		for(int i = 0; i < uncovered; i++)
			printf("%d ",minterm_uncovered[i]);
		printf("\n");

		//greedy set minimizing algorithm
		int flag = set_minimizer(prime , minterm_uncovered , uncovered);
		if(flag) printf("\nAll minterns now covered\n");
		else{
			printf("\nLeftover uncovered : ");
			for(int i = 0; i < uncovered; i++)
				if(minterm_uncovered[i] != -1)
					printf("%d ",minterm_uncovered[i]);
			printf("\n");
		}
	}

	char *str_exp = NULL;
	int offset = 0;

	for(int i = 0; i < prime->count; i++){

		if(prime->minimal[i] == 0) continue;

		//Binary to expression
		char *exp = Expression(prime->binary[i]);
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
		offset += written;
		free(exp);
	}

	//free memory
	free_3d_pointer(table , prime->count , pow(2,var));
	free(minterm_uncovered);
	return str_exp;
}
