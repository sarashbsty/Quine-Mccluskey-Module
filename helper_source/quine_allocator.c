#include "memory_tracker.h"
#include "helper.h"
#include "quine.h"

int allocate(quine *var , int size){
	//Memory Allocation for quine items
	//binary
	char **temp1 = realloc(var->binary , size * sizeof(*temp1));
	if(!temp1) return 1;

	//mintermCount
	int *temp2 = realloc(var->mintermCount , size * sizeof(*temp2));
	if(!temp2){
		free(temp1);
		return 1;
	}

	//combined
	int *temp3 = realloc(var->combined , size * sizeof(*temp3));
	if(!temp3){
		free(temp1);
		free(temp2);
		return 1;
	}

	var->binary = temp1;
	var->mintermCount = temp2;
	var->combined = temp3;

	return 0;
}
