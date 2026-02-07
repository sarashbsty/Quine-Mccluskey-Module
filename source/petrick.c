#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

static int getMinLiterals(char **SOP_terms, int SOP_count, int max_literals){
	int min_literal = max_literals; //initialize maximum possible literal
	for(int i = 0; i < SOP_count; i++){
		int size = strlen(SOP_terms[i]);
		if(size < min_literal) min_literal = size;
	}
	return min_literal;
}

static int removeNonMinimalTerms(char **SOP_terms, int SOP_count, int min_literal){
	int new_SOP_count = 0;
	for(int i = 0; i < SOP_count; i++){
		int size = strlen(SOP_terms[i]);
		if(size > min_literal){
			free(SOP_terms[i]);
			SOP_terms[i] = NULL;
		}
		else
			SOP_terms[new_SOP_count++] = SOP_terms[i];
	}

	//NULL all other redundant
	for(int i = new_SOP_count; i < SOP_count; i++)
		SOP_terms[i] = NULL;

	return new_SOP_count;
}

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

void petrick(quine *prime , char **POS_terms, int POS_count, int var){

	char **SOP_terms = malloc(sizeof(*SOP_terms) * 1);
	if(!SOP_terms) { printf("\nERROR: Memory Allocation Failed | petrick"); exit(0); }

	int SOP_count = 0, max_literals = prime->count;;

	printf("\n\nPetrick Algorithm!");
	printf("\n\nLet,\n");
	for(int i = 0; i < prime->count; i++)
		printf("P%d = %s\n", i+1 , prime->expression[i]);

	//initialization
	char *term = strdup("");
	if(!term) { printf("\nERROR: Memory Allocation Failed | petrick"); exit(0); }
	SOP_terms[SOP_count++] = term;

	//Patrick Algorithm
	printf("\nBY, Distribution and absorption:");
	for(int i = 0; i < POS_count; i++){
		char **new_SOP_terms = malloc(sizeof(*new_SOP_terms) * SOP_count * strlen(POS_terms[i]));
		if(!new_SOP_terms){ printf("\nERROR: Memory Allocation Failed | petrick"); exit(0); }

		int new_SOP_count = distributive(new_SOP_terms, SOP_terms, SOP_count, POS_terms[i], max_literals+1);
		new_SOP_count = absorp(new_SOP_terms,new_SOP_count);

		free_2d_pointer(SOP_terms,SOP_count);
		SOP_terms = new_SOP_terms;
		SOP_count = new_SOP_count;

		displayProcess(SOP_terms, SOP_count, POS_terms, POS_count , i);
	}

	int min_literal = getMinLiterals(SOP_terms, SOP_count, max_literals);
	SOP_count = removeNonMinimalTerms(SOP_terms, SOP_count, min_literal);

	printf("\n\nMinimal Terms with %d literals: ",min_literal);
	for(int i = 0; i < SOP_count; i++){
		for(char *ch = SOP_terms[i]; *ch; ch++)
			printf("P%d", 1+(*ch - 'A'));
		printf(" ");
	}

	//Printing possible combinaion and also cost calculation

	int min_cost = var * max_literals , minCostIdx = 0;
	printf("\n\nPosible Combinations:");
	for(int i = 0; i < SOP_count; i++){
		int new_cost = 0;
		printf("\n%d. ",i+1);

		//print essential implicants(if exist)
		for(int j = 0; j < prime->count; j++){
			if(prime->minimal[j] == 0) continue;
			printf("%s + ", prime->expression[j]);
			new_cost += prime->cost[j];
		}

		//print petrick SOP term's expression
		term = SOP_terms[i];
		for(int j = 0; term[j] != '\0'; j++){
			int idx = term[j] - 'A';
			printf((j == 0) ? "%s" : " + %s" ,prime->expression[idx]);
			new_cost += prime->cost[idx];
		}

		printf(" \tcost = %d",new_cost);

		if(new_cost < min_cost){
			min_cost = new_cost;
			minCostIdx = i;
		}
	}

	printf("\n\nChosen Combination no. %d\n",minCostIdx+1);
	term = SOP_terms[minCostIdx];
	for(int i = 0; term[i] != '\0'; i++){
		int idx = term[i] - 'A';
		prime->minimal[idx] = 1;
	}

	//Clear memory
	free_2d_pointer(SOP_terms, SOP_count);
}
