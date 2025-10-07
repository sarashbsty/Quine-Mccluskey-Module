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

	//array to store indexes of binary for minimal cover
	int *minimal = malloc(prime->count * sizeof(*minimal));
	if(!minimal) { printf("\nEssential array allocation failed | low memory | essential_implicants\n"); exit(0); }
	int count = 0;

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

			// checking duplicates
			int check = 0;
			while(check < count && minimal[check] != index) check++;

			//store if no duplicate
			if(check >= count) minimal[count++] = index;

			//marking for visual guide
			strcpy(table[index][minterms[j]] , "(X)" );
		}
	}

	printf("\n\n\nTable to find Essential prime Implicants: \n");
    display_essential_table(prime, table, minterms, min_count);

	//checking whether the essential implicants suffice and create a array of uncovered minterms
	for(int i = 0; i < min_count; i++){
		int check = -1;
		for(int j = 0; j < count; j++){
			int idx = minimal[j];
			check = isexist(prime->minterms[idx] , prime->mintermCount[idx] , minterms[i]);
			if(check != -1) break;
		}
		if(check == -1) minterm_uncovered[uncovered++] = minterms[i];
	}

	if(uncovered == 0) printf("All Minterms covered by essential-implicants\n");
	else{
		printf("Minterms not covered : ");
		for(int i = 0; i < uncovered; i++)
			printf("%d ",minterm_uncovered[i]);
		printf("\n");

		int covered = 0, covers;
		do{
			covers = 0;
			int pos = -1;

			//determines the prime-implicant which covers most minterms in every iteration
			for(int i = 0; i < prime->count; i++){
				int new_covers = 0;
				for(int j = 0; j < uncovered; j++){
					if(minterm_uncovered[j] == -1) continue;
					int check = isexist(prime->minterms[i] , prime->mintermCount[i] , minterm_uncovered[j]);
					if(check != -1) new_covers++;
				}
				if(new_covers > covers){
					pos = i;
					covers = new_covers;
				}
			}

			//removes the covered minterm and also print the selected prime-implicant
			if(pos != -1){

				//Binary to expression
				char *exp = Expression(prime->binary[pos]);
				if(exp == NULL) { printf("\nERROR: Expression creation Failed | Low Memory | essential-implicants\n"); exit(0); }
				printf("\nSelected %s for minterms : ",exp);
				free(exp);

				for(int j = 0; j < uncovered; j++){
					if(minterm_uncovered[j] == -1) continue;
					int check = isexist(prime->minterms[pos] , prime->mintermCount[pos] , minterm_uncovered[j]);
					if(check != -1){
						printf("%d ",minterm_uncovered[j]);
						covered++;
						minterm_uncovered[j] = -1;
					}
				}
				minimal[count++] = pos;
			}

		}while(covers != 0);

		if(covered == uncovered) printf("\nAll minterns now covered\n");
		else printf("\nStill not all covered : \n");
	}

	char *str_exp = NULL;
	int offset = 0;

	for(int i = 0; i < count; i++){

		//Binary to expression
		int idx = minimal[i];
		char *exp = Expression(prime->binary[idx]);
		if(exp == NULL) { printf("\nERROR: Expression creation Failed | Low Memory | essential-implicants\n"); exit(0); }

		//Creating expression string
		int needed = snprintf(NULL , 0 , i ? " + %s" : "%s" , exp);
		int new_capacity = offset+needed+1;
		char *temp = realloc(str_exp , new_capacity * sizeof(*temp));
		if(!temp) {
			printf("\nERROR: expression string creation fail | Low Memory | essential-implicants\n");
			free(str_exp);
			free(minimal);
			exit(0);
		}
		str_exp = temp;
		int written = snprintf(str_exp+offset , new_capacity-offset , i ? " + %s" : "%s" , exp);
		offset += written;
		free(exp);
	}

	//free memory
	free_3d_pointer(table , prime->count , pow(2,var));
	free(minterm_uncovered);
	free(minimal);
	return str_exp;
}
