#include "memory_tracker.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

//check for improvemnt
char* essential_implicants(quine *prime , int minterms[] , int min_count , int var){

	// create essential_table
	char ***table = create_table(prime->count , pow(2,var) , 6);
	if(table == NULL){ printf("\nERROR: Table creation failed | Low Memory | essential_implicants\n"); exit(0); }

	//array to store indexes of essential binary in prime
	int *essential = malloc(prime->count * sizeof(*essential));
	if(!essential) { printf("\nEssential array allocation failed | low memory | essential_implicants\n"); exit(0); }
	int count = 0;

	//array to store covered minterms
	int *minterm_covered = malloc(pow(2,var) * sizeof(int));
	if(!minterm_covered) { printf("\nminterm_covered array allocation failed | low memory | essential_implicants\n"); exit(0); }
	int covered = 0;

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

			// checking duplicates
			int check = 0;
			while(check < count && essential[check] != index) check++;

			//store if no duplicate
			if(check >= count) essential[count++] = index;

			//marking for visual guide
			strcpy(table[index][minterms[j]] , "(X)" );
		}
	}

	printf("\n\n\nTable to find Essential prime Implicants: \n");
    display_essential_table(prime, table, minterms, min_count);

	char *str_exp = NULL;
	int offset = 0;

	for(int i = 0; i < count; i++){

		//Binary to expression
		int idx = essential[i];
		char *exp = Expression(prime->binary[idx]);
		if(exp == NULL) { printf("\nERROR: Expression creation Failed | Low Memory | essential-implicants\n"); exit(0); }

		//Creating expression string
		int needed = snprintf(NULL , 0 , i ? " + %s" : "%s" , exp);
		int new_capacity = offset+needed+1;
		char *temp = realloc(str_exp , new_capacity * sizeof(*temp));
		if(!temp) {
			printf("\nERROR: expression string creation fail | Low Memory | essential-implicants\n");
			free(str_exp);
			free(essential);
			exit(0);
		}
		str_exp = temp;
		int written = snprintf(str_exp+offset , new_capacity-offset , i ? " + %s" : "%s" , exp);
		offset += written;
		free(exp);
	}

	//free memory
	free_3d_pointer(table , prime->count , pow(2,var));
	free(minterm_covered);
	free(essential);
	return str_exp;
}
