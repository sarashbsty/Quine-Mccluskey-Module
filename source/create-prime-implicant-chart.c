#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

char*** createPiChart(quine *prime , int minterms[] , int min_count, int var){

	char ***table = create_table(prime->count , pow(2,var) , 6);
	if(table == NULL){ return NULL; }

	//all space initialize
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < min_count; j++)
			strcpy(table[i][minterms[j]] , " ");

	//Assigning X to respective cells
	for(int i = 0; i < prime->count; i++)
		for(int j = 0; j < prime->mintermCount[i]; j++)
			strcpy(table[i][prime->minterms[i][j]] , " X");

	//Finding the essential implicant by finding column(minterm) with only one 'X' and the prime implecant that covers it
	for(int j = 0; j < min_count; j++){
		int index ,count = 0;
		for(int i = 0; i < prime->count && count <= 1; i++){
			if(strcmp(table[i][minterms[j]], " X") == 0){
				count++;
				index = i;
			}
		}
		if(count == 1){
			//marking for visual guide
			strcpy(table[index][minterms[j]] , "(X)" );

			//marked for minimal cover
			prime->minimal[index] = 1;
		}
	}

	return table;
}
