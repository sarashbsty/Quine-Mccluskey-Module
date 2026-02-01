#include "memory_tracker.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

typedef struct {
	int minterm;
    char PI[50][2];  //prime-implicant
	int count;
	int used;
} petrick_struct;

void petrick(quine *prime , char ***table , int *uncovered_terms , int uncovered_count){
	petrick_struct P[20];
	int struct_count = uncovered_count;

	//Gathering data
	for(int i = 0; i < uncovered_count; i++){
		int idx = 0;
		for(int j = 0; j < prime->count; j++){
			if(strcmp(table[j][uncovered_terms[i]], " ") != 0){
				P[i].PI[idx][0] = ('A'+j);
				P[i].PI[idx][1] = '\0';
				idx++;
			}
		}
		P[i].count = idx;
		P[i].minterm = uncovered_terms[i];
		P[i].used = 0;
	}

	//Display data
	printf("\n\nPetrick used!");
	for(int i = 0; i < struct_count; i++){
		printf("\n%d  -  %s",P[i].minterm , P[i].PI[0]);
		for(int j = 1; j < P[i].count; j++)
			printf(" + %s",P[i].PI[j]);
	}
	printf("\n\n");


	char **equation = malloc(sizeof(*equation) * 100);
	int eq_count = 0;

	//initialize
	for(int i = 0; i < P[0].count; i++){
		char *str = malloc(sizeof(*str) * prime->count);
		strcpy(str , P[0].PI[i]);
		equation[eq_count++] = str;
	}

	//using Distributive law
	for(int i = 1; i < struct_count; i++){
		char **temp = malloc(sizeof(*equation) * 100);
		int temp_count = 0;
		for(int j = 0; j < eq_count; j++){
			for(int k = 0; k < P[i].count; k++){
				char *str = malloc(sizeof(*str) * prime->count);
				if(strstr(equation[j], P[i].PI[k])) strcpy(str,equation[j]);
				else snprintf(str, prime->count, "%s%s" , equation[j] , P[i].PI[k]);
				temp[temp_count++] = str;
			}
		}
		free_2d_pointer(equation,eq_count);
		equation = temp;
		eq_count = temp_count;
	}

	//finding smallest_literal
	int smallest_literal = prime->count;
	for(int i = 0; i < eq_count; i++){
		int count = 0;
		while(equation[i][count] != '\0') count++;
		if(count < smallest_literal) smallest_literal = count;
	}
	printf("Smallest literal = %d",smallest_literal);

	//remove other big literals
	char **temp = malloc(sizeof(*equation) * 100);
	int temp_count = 0;
	for(int i = 0; i < eq_count; i++){
		int count = 0;
		while(equation[i][count] != '\0') count++;
		if(count == smallest_literal){
			temp[temp_count++] = equation[i];
			equation[i] = NULL;
		}
	}
	free_2d_pointer(equation,eq_count);
	equation = temp;
	eq_count = temp_count;

	printf("\nTerms with %d Literals: ",smallest_literal);
	for(int i = 0; i < eq_count; i++)
		printf("%s  " , equation[i]);

	printf("\n\nPosible Combinations: \n");
	for(int i = 0; i < eq_count; i++){
		printf("%d. ",i+1);
		for(int j = 0; equation[i][j] != '\0'; j++){
			int idx = equation[i][j] - 'A';
			printf((j == 0) ? "%s" : " + %s" ,prime->expression[idx]);
		}
		printf("\n");
	}

	//Choosing first combination for our main Results
	for(int i = 0; equation[0][i] != '\0'; i++){
		int idx = equation[0][i] - 'A';
		prime->minimal[idx] = 1;
	}
	free_2d_pointer(equation,eq_count);
}
