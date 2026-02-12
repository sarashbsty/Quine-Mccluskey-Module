
char** ToBinary(int *minterms , int n_terms, int var){

	if (n_terms == 0 || var == 0) return NULL;

	char **binary = malloc(n_terms * sizeof(*binary)); // create array of n_terms pointers
	if(!binary) return NULL;

	for(int i = 0; i < n_terms; i++){
		char* str = malloc((var+1) * sizeof(*str));
		if(!str){
			free_2d_pointer(binary , i);
			return NULL;
		}

		for(int j = 0; j < var; j++)
			str[var-1-j] = ((minterms[i] >> j) & 1) ? '1' : '0';
		str[var] = '\0';
		binary[i] = str;
	}
	return binary;
}

int expression(string_arr *binary){
	for(int index = 0; index < binary->size; index++){

		size_t len = (2*strlen(binary->arr[index])) + 1;
		char *str = malloc(len);
		if(!str) return 0;

		int count = 0;
		for(int i = 0; binary->arr[index][i] != '\0'; i++){
			char var = 'A'+i;
			if(binary->arr[index][i] == '0') { str[count++] = var; str[count++] = '\''; }
			else if(binary->arr[index][i] == '1') str[count++] = var;
			else continue;
		}
		str[count] = '\0';
		free(binary->arr[index]);
		binary->arr[index] = str;
	}
	return 1;
}

char *str_exp = NULL;
int offset = 0;
for(int i = 0; i < prime.count; i++){

	if(prime.minimal[i] == 0) continue;

	//Binary to expression
	char *exp = Expression(prime.binary[i]);
	if(exp == NULL) { printf("\nERROR: Expression creation Failed | Low Memory | essential-implicants\n"); exit(0); }

	//Creating expression string
	int needed = snprintf(NULL , 0 , (offset) ? " + %s" : "%s" , exp);
	int new_capacity = offset+needed+1;
	char *temp = realloc(str_exp , new_capacity * sizeof(*temp));
	if(!temp) {
		printf("\nERROR: expression string creation fail | Low Memory | essential-implicants\n");
		free(str_exp);
		exit(0);
	}
	str_exp = temp;
	int written = snprintf(str_exp+offset , new_capacity-offset , (offset) ? " + %s" : "%s" , exp);
	offset += written;		free(exp);
}

char *result = str_exp;

printf("\n\nResult: Y(");
for (int i = 1; i <= var; i++){
	printf((i == var) ? "%c" : "%c,", 'A' + i);
printf(") = %s\n\n", (result ? result : "No result"));
free(result);
	


/*


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

*/	