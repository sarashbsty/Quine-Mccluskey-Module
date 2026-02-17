#include "memory_tracker.h"
#include<string.h>
#include<stdlib.h>
#include "insert_entry.h"

int insertEntry(char ***returnPtr ,int *countPtr , int *capPtr , char *entry)
{
	if(!returnPtr || !entry) return 0;

	char **arr = *returnPtr;
	int count = *countPtr;
	int cap = *capPtr;

	if(cap == 0){
		arr = calloc(2 , sizeof(*arr));
		if(!arr) return 1;
		cap += 2;
	}

	if(count >= cap){
		cap += 2;
		char **tmp = realloc(arr ,cap * sizeof(*tmp));
		if(!tmp) return 1;
		arr = tmp;
	}

	char *newEntry = strdup(entry);
	if(!newEntry) return 1;

	arr[count++] = newEntry;

	*returnPtr = arr;
	*countPtr = count;
	*capPtr  =  cap;
	return 0;
}
