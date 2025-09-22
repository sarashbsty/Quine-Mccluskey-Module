#include<stdlib.h>

char ***create_table(int row , int col , int n){
	char ***table = malloc(row * sizeof(*table));
	if(!table) return NULL;

	for(int i = 0; i < row; i++){

		//create individaul string-array
		char** temp = malloc(col * sizeof(*temp));
		if(!temp){
			for(int x = 0; x < i ; x++){
				free(table[x][0]);
				free(table[x]);
			}
			free(table);
			return NULL;
		}
		table[i] = temp;

		//create big block
		char *block = malloc((col*n) * sizeof(*block));
		if(!block){
			for(int x = 0; x < i ; x++){
				free(table[x][0]);
				free(table[x]);
			}
			free(table[i]);
			free(table);
			return NULL;
		}

		//assigning segments of bigblock
		for(int idx = 0; idx < col; idx++)
			table[i][idx] = block + (idx*n);
	}
	return table;
}
