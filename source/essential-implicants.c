#include "memory_tracker.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

//check for improvemnt
void essential_implicants(quine *prime , int minterms[] , int min_count , int var){

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
			if(prime->minimal[j] == 0) continue;
			check = find_int(prime->minterms[j] , prime->mintermCount[j] , minterms[i]);
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

		petrick(table ,minterm_uncovered ,uncovered, prime->count);

		//greedy set minimizing algorithm
		int notAllCovered = set_minimizer(prime , minterm_uncovered , &uncovered);
		if(notAllCovered){
			printf("\nLeftover uncovered : ");
			for(int i = 0; i < uncovered; i++)
					printf("%d ",minterm_uncovered[i]);
		}
		else printf("\nAll minterns now covered\n");
	}

	//free memory
	free_3d_pointer(table , prime->count , pow(2,var));
	free(minterm_uncovered);
}
