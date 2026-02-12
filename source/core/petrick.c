#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "boolean_algebra.h"
#include "memory.h"

static int getMinLiterals(char **SOP_terms, int SOP_count, int max_literals);
static int removeNonMinimalTerms(char **SOP_terms, int SOP_count, int min_literal);
static void displayProcess(char **SOP_terms, int SOP_count, char **POS_terms, int POS_count, int i);

int petrick(quine *prime , char **POS_terms, int POS_count, int var){

	//initialization
	char **SOP_terms = malloc(sizeof(*SOP_terms) * 1);
	if(!SOP_terms) return 1;

	SOP_terms[0] = strdup("");
	if(!SOP_terms[0]) return 1;

	int SOP_count = 1, max_literals = prime->count;

	//Patrick Algorithm
	for(int i = 0; i < POS_count; i++){
		char **new_SOP_terms = malloc(sizeof(*new_SOP_terms) * SOP_count * strlen(POS_terms[i]));
		if(!new_SOP_terms){
			free_2d_pointer(SOP_terms,SOP_count);
			return 1;
		}

		int new_SOP_count = distributive(new_SOP_terms, SOP_terms, SOP_count, POS_terms[i], max_literals+1);
		new_SOP_count = absorp(new_SOP_terms,new_SOP_count);

		free_2d_pointer(SOP_terms,SOP_count);
		SOP_terms = new_SOP_terms;
		SOP_count = new_SOP_count;
	}

	int min_literal = getMinLiterals(SOP_terms, SOP_count, max_literals);
	SOP_count = removeNonMinimalTerms(SOP_terms, SOP_count, min_literal);

	int min_cost = (var*2) * min_literal , minCostIdx = 0;
	for(int i = 0; i < SOP_count; i++){
		int new_cost = 0;
		char* term = SOP_terms[i];

		for(int j = 0; term[j] != '\0'; j++){
			int idx = term[j] - 'A';
			new_cost += prime->cost[idx];
		}

		if(new_cost < min_cost){
			min_cost = new_cost;
			minCostIdx = i;
		}
	}

	char* term = SOP_terms[minCostIdx];
	for(int i = 0; term[i] != '\0'; i++){
		int idx = term[i] - 'A';
		prime->minimal[idx] = 1;
	}

	//Clear memory
	free_2d_pointer(SOP_terms, SOP_count);
	return 0;
}

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
