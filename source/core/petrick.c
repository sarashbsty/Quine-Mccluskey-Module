#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "boolean_algebra.h"
#include "memory.h"
#include "petrick.h"

static int getMinLiterals(char **SOP_terms, int SOP_count, int max_literals);
static int removeNonMinimalTerms(char **SOP_terms, int SOP_count, int min_literal);
static int logger(char** returnPtr, char **SOP_terms, int SOP_count, char **POS_terms, int POS_count, int i);
static int convertStyle(char **ptr);

petrickData* petrick(quine *prime , char **POS_terms, int POS_count, int var){

	//all dynamic variable Declaration
	petrickData *P = calloc(1, sizeof(*P));
	if(!P) goto FAIL;

	char **processArr = NULL, **combinations = NULL, **SOP_terms = NULL;
	int *costArr = NULL, combiCount = 0;

	//initialization
	int processCount = 0;
	processArr       = malloc(POS_count * sizeof(*processArr));
	if(!processArr)  goto FAIL;


	SOP_terms      = malloc(sizeof(*SOP_terms) * 1);
	if(!SOP_terms) goto FAIL;

	SOP_terms[0]      = strdup("");
	if(!SOP_terms[0]) goto FAIL;

	int SOP_count = 1, max_literals = prime->count;

	//Petrick Algorithm
	for(int i = 0; i < POS_count; i++)
	{
		char **new_SOP_terms = NULL;
		int new_SOP_count    = distributive(&new_SOP_terms, SOP_terms, SOP_count, POS_terms[i], max_literals+1);
		if(new_SOP_count == -1) goto FAIL;

		new_SOP_count = absorp(new_SOP_terms,new_SOP_count);

		free_2d_pointer(SOP_terms,SOP_count);
		SOP_terms = new_SOP_terms;
		SOP_count = new_SOP_count;

		new_SOP_terms = NULL;
		new_SOP_count = 0;

		if(logger(&processArr[processCount++], SOP_terms, SOP_count, POS_terms, POS_count, i)) { P->error = 1; return P; }
	}

	int min_literal = getMinLiterals(SOP_terms, SOP_count, max_literals);
	SOP_count       = removeNonMinimalTerms(SOP_terms, SOP_count, min_literal);

	combiCount   = 0;
	combinations = malloc(SOP_count * sizeof(*combinations));
	if(!combinations) goto FAIL;

	costArr = malloc(SOP_count * sizeof(*costArr));
	if(!costArr) goto FAIL;

	//storing combinations of Expressions and cost
	for(int i = 0; i < SOP_count; i++)
	{
		int cost = 0 , offset = 0 , cap = (var*2 + 1) * min_literal + 1;

		char* str = malloc(cap * sizeof(*str)); //A'B'C', = 7
		if(!str) goto FAIL;

		char* term = SOP_terms[i];
		for(int j = 0; term[j] != '\0'; j++)
		{
			int idx = term[j] - 'A';
			offset += snprintf(str+offset, cap-offset, "%s,", prime->expression[idx]);
			cost   += prime->cost[idx];
		}

		str[offset-1] = '\0';
		combinations[combiCount++] = str;
		costArr[i] = cost;

		str = NULL;
	}

	//determine index of the SOP term with minimum cost
	int min_cost = costArr[0] , minCostIdx = 0;
	for(int i = 1; i < SOP_count; i++){
		if(costArr[i] < min_cost){
			minCostIdx = i;
			min_cost   = costArr[i];
		}
	}

	char* term = SOP_terms[minCostIdx];
	for(int i = 0; term[i] != '\0'; i++){
		int idx = term[i] - 'A';
		prime->minimal[idx] = 1;
	}

	for(int i = 0; i < SOP_count; i++)
		convertStyle(&SOP_terms[i]);  // convert ABC to P1P2P3

	P->process      = processArr;
	P->processCount = processCount;
	P->SOP_terms    = SOP_terms;
	P->SOP_count    = SOP_count;
	P->combinations = combinations;
	P->cost         = costArr;
	P->minCostIdx   = minCostIdx;

	processArr   = NULL;
	SOP_terms    = NULL;
	combinations = NULL;
	costArr      = NULL;

	return P;

	FAIL:
		free_2d_pointer(processArr , processCount);
		free_2d_pointer(SOP_terms, SOP_count);
		free_2d_pointer(combinations, combiCount);
		free(costArr);
		P->error = 1;
		return P;
}

static int convertStyle(char **ptr){
	char *str = *ptr;
	int N = 0;
	for(char *ch = str; *ch; ch++)
		N += snprintf(NULL, 0, "P%d" , 1+(*ch)-'A');

	char *newStr = malloc(N+1 * sizeof(*newStr));
	if(!newStr) return 1;

	int offset = 0;
	for(char *ch = str; *ch; ch++)
		offset += snprintf(newStr+offset, (N+1)-offset, "P%d" , 1+(*ch)-'A');

	free(str);
	*ptr = newStr;
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

static int logger(char** returnPtr, char **SOP_terms, int SOP_count, char **POS_terms, int POS_count, int i){
	//two pass method
	int needed = 0;

	needed += snprintf(NULL, 0, "P = (");
	for(int j = 0; j < SOP_count; j++){
		char *str = SOP_terms[j];
		if(j > 0) needed += snprintf(NULL, 0," + ");
		while(*str){
			needed += snprintf(NULL, 0,"P%d" , 1+(*str-'A') );
			str++;
		}
	}
	needed += snprintf(NULL, 0, ")");

	for(int k = i+1; k < POS_count; k++){
		needed += snprintf(NULL, 0,"·(");
		for(int x = 0; POS_terms[k][x] != '\0'; x++){
			char ch = POS_terms[k][x];
			needed += snprintf(NULL, 0,(x == 0) ? "P%d" : " + P%d", 1+(ch-'A') );
		}
		needed += snprintf(NULL, 0,")");
	}


	int cap = needed+1;
	char *log = malloc(cap * sizeof(*log));
	if(!log) return 1;
	int offset = 0;

	offset += snprintf(log+offset, cap-offset, "P = (");
	for(int j = 0; j < SOP_count; j++){
		char *str = SOP_terms[j];
		if(j > 0) offset += snprintf(log+offset, cap-offset, " + ");
		while(*str){
			offset += snprintf(log+offset, cap-offset, "P%d" , 1+(*str-'A') );
			str++;
		}
	}
	offset += snprintf(log+offset, cap-offset, ")");

	for(int k = i+1; k < POS_count; k++){
		offset += snprintf(log+offset, cap-offset, "·(");
		for(int x = 0; POS_terms[k][x] != '\0'; x++){
			char ch = POS_terms[k][x];
			offset += snprintf(log+offset, cap-offset, (x == 0) ? "P%d" : " + P%d", 1+(ch-'A') );
		}
		offset += snprintf(log+offset, cap-offset, ")");
	}

	*returnPtr = log;
	return 0;
}
