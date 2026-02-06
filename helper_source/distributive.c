#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

int distributive(char** newTermsArr, char **TermsArr , int count , char *term , int term_size){
	int newCount = 0;
	for(int i = 0; i < count; i++){

		char *termA = TermsArr[i] , *termB = term;

		for(char *x = termB; *x; x++){
			char *newTerm = malloc(sizeof(*newTerm) * term_size);
			if(!newTerm){ printf("\nERROR: memory allocation failed | distributive | pretrick"); exit(0); }

			//if 'x' literal already exist in 'termA', then just copy 'termA' to newTerm and continue to next
			if(strchr(termA, *x))
				strcpy(newTerm, termA);
			else
				snprintf(newTerm, term_size, "%s%c" , termA, *x);

			newTermsArr[newCount++] = newTerm;
		}
	}
	return newCount;
}
