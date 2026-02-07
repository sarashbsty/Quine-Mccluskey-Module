#include "memory_tracker.h"
#include "helper.h"
#include "quine.h"

int allocate(quine *var , int size){
	//Memory Allocation for quine items
	//binary
	char **temp1 = realloc(var->binary , size * sizeof(*temp1));
	if(!temp1) return 1;

	//minterms
	int **temp2 = realloc(var->minterms , size * sizeof(*temp2));
	if(!temp2){
		free(temp1);
		return 1;
	}

	//mintermCount
	int *temp3 = realloc(var->mintermCount , size * sizeof(*temp3));
	if(!temp3){
		free(temp1);
		free(temp2);
		return 1;
	}

	//combined
	int *temp4 = realloc(var->combined , size * sizeof(*temp4));
	if(!temp4){
		free(temp1);
		free(temp2);
		free(temp3);
		return 1;
	}

	//minimal
	int *temp5 = realloc(var->minimal , size * sizeof(*temp5));
	if(!temp5){
		free(temp1);
		free(temp2);
		free(temp3);
		free(temp4);
		return 1;
	}

	//expression
	char **temp6 = realloc(var->expression , size * sizeof(*temp6));
	if(!temp6){
		free(temp1);
		free(temp2);
		free(temp3);
		free(temp4);
		free(temp5);
		return 1;
	}

	int *temp7 = realloc(var->cost , size * sizeof(*temp7));
	if(!temp7){
		free(temp1);
		free(temp2);
		free(temp3);
		free(temp4);
		free(temp5);
		free(temp6);
		return 1;
	}

	var->binary = temp1;
	var->minterms = temp2;
	var->mintermCount = temp3;
	var->combined = temp4;
	var->minimal = temp5;
	var->expression = temp6;
	var->cost = temp7;
	var->capacity = size;
	return 0;
}
