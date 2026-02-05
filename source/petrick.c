#include "memory_tracker.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

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

static int absorbTerms(char **SOP_terms, int SOP_count){
	for(int a = 0; a < SOP_count-1; a++){
		char* A = SOP_terms[a];

		for(int b = a+1; b < SOP_count && A != NULL; b++){

			char* B = SOP_terms[b];
			if(B == NULL) continue;
			int subset = isSubset(A, B);

			if(subset == 1){   //if A ⊆ B or A = B
				free(B);
				SOP_terms[b] = NULL;
			}
			else if(subset == 2){  //if B ⊆ A
				free(A);
				SOP_terms[a] = NULL;
				break;
			}
		}
	}

	int new_SOP_count = 0;
	for(int i = 0; i < SOP_count; i++){
		if(SOP_terms[i] == NULL) continue;
		SOP_terms[new_SOP_count++] = SOP_terms[i];
	}
	return new_SOP_count;
}

static int distributive(char** new_SOP_terms, char **A , int A_count , char *B , int term_size){
	int new_SOP_count = 0;
	for(int a = 0; a < A_count; a++){

		char *term = A[a];
		for(int b = 0; B[b] != '\0'; b++){
			char *new_term = malloc(sizeof(*new_term) * term_size);
			if(!new_term){ printf("\nERROR: memory allocation failed | distributive | pretrick"); exit(0); }

			//if literal already exist in the term, then just copy the term and continue to next
			char ch = B[b];
			if(strchr(term, ch)) strcpy(new_term, term);
			else snprintf(new_term, term_size, "%s%c" , term, ch);

			new_SOP_terms[new_SOP_count++] = new_term;
		}
	}
	return new_SOP_count;
}

static int getMinLiterals(char **SOP_terms, int SOP_count, int max_literals){
	int min_literal = max_literals; //initialize maximum possible literal
	for(int i = 0; i < SOP_count-1; i++){
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
		else{
			SOP_terms[new_SOP_count++] = SOP_terms[i];
			SOP_terms[i] = NULL;
		}
	}
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

void petrick(quine *prime , char ***table , int *uncovered_terms , int uncovered_count){

	char **POS_terms = malloc(sizeof(*POS_terms) * uncovered_count);
	if(!POS_terms) { printf("\nERROR: Memory Allocation Failed | petrick"); exit(0); }

	char **SOP_terms = malloc(sizeof(*SOP_terms) * 1);
	if(!SOP_terms) { printf("\nERROR: Memory Allocation Failed | petrick"); exit(0); }

	int POS_count = 0, SOP_count = 0, max_literals = prime->count;;

	//Gathering data
	for(int i = 0; i < uncovered_count; i++){
		char *term = malloc(sizeof(*term) * (max_literals+1));
		if(!term) { printf("\nERROR: Memory Allocation Failed | petrick"); exit(0); }

		int offset = 0;
		for(int j = 0; j < prime->count; j++){
			if(strcmp(table[j][uncovered_terms[i]], " ") != 0)
				term[offset++] = 'A'+j;
		}
		term[offset] = '\0';
		POS_terms[POS_count++] = term;
	}

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
		new_SOP_count = absorbTerms(new_SOP_terms,new_SOP_count);

		free_2d_pointer(SOP_terms,SOP_count);
		SOP_terms = new_SOP_terms;
		SOP_count = new_SOP_count;

		displayProcess(SOP_terms, SOP_count, POS_terms, POS_count , i);
	}

	int min_literal = getMinLiterals(SOP_terms, SOP_count, max_literals);
	SOP_count = removeNonMinimalTerms(SOP_terms, SOP_count, min_literal);

	char** ptr = SOP_terms;
	printf("\n\nMinimal Terms: ");
	while(*ptr){
		char* str = *ptr++;
		while(*str){ printf("P%d", 1+(*str-'A') ); str++; }
		printf(" ");
	}

	printf("\n\nPosible Combinations: \n");
	for(int i = 0; i < SOP_count; i++){
		printf("%d. ",i+1);

		//print essential implicants(if exist)
		for(int j = 0; j < prime->count; j++){
			if(prime->minimal[j] == 0) continue;
			printf("%s + ", prime->expression[j]);
		}

		//print petrick SOP term's expression
		term = SOP_terms[i];
		for(int j = 0; term[j] != '\0'; j++){
			int idx = term[j] - 'A';
			printf((j == 0) ? "%s" : " + %s" ,prime->expression[idx]);
		}
		printf("\n");
	}

	//Choosing first combination for our main Results
	term = SOP_terms[0];
	for(int i = 0; term[i] != '\0'; i++){
		int idx = term[i] - 'A';
		prime->minimal[idx] = 1;
	}

	//Clear memory
	free_2d_pointer(POS_terms , POS_count);
	free_2d_pointer(SOP_terms, SOP_count);
}

/*
static int isSubset(const char *A, const char *B){
    int A_in_B = 1;
    int B_in_A = 1;

    for (int i = 0; A[i]; i++)
        if (!strchr(B, A[i])) A_in_B = 0;

    for (int i = 0; B[i]; i++)
        if (!strchr(A, B[i])) B_in_A = 0;

    if (A_in_B && !B_in_A) return 1;   // A ⊆ B
    if (B_in_A && !A_in_B) return 2;   // B ⊆ A
    if (A_in_B && B_in_A)  return 1;   // equal → delete B only
    return 0;
}

*/
