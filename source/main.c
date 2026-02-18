#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "qmMinimizer.h"
#include "memory.h"

static inline void clear_input_buffer(){
	int c;
    while ((c = getchar()) != '\n' && c != EOF);  //clear stdin
}

static int get_minterms(int *minterms , int index, int max_terms);

int main() {

    #ifdef _WIN32
	system("cls");
    SetConsoleOutputCP(CP_UTF8);
    #endif

    // Data input
    printf("Enter no. of variables: ");
	int var;
    scanf("%d", &var);
	clear_input_buffer();

	if(var < 1){
		printf("%d is Invalid\n" , var);
		return 0;
	}

	int maxTerms = pow(2, var);
	int *minterms = malloc(maxTerms * sizeof(*minterms));
	if(minterms == NULL){ printf("ERROR: failed creating minterm array for input | low memory | main"); exit(0); }

    printf("Enter min terms : ");
	int minCount = get_minterms(minterms , 0 , maxTerms);

	if(minCount == 0){
		printf("No Minterms Entered!\n");
		free(minterms);
		return 0;
	}

	printf("Enter dont care : ");
	int dont_care_count = get_minterms(minterms , minCount , maxTerms);

    int n_terms = minCount + dont_care_count;

    printf("\n\n%d Min terms: ", minCount);
    for (int i = 0; i < minCount; i++)
        printf("%d ", minterms[i]);

    printf("\n%d Dont care: ", dont_care_count);
    for (int i = minCount; i < n_terms; i++)
        printf("%d ", minterms[i]);
	printf("\n");

    qmData data = qmMinimizer(minterms, n_terms, minCount, var);

	if(data.error){
		printf("%s\n",data.errorMsg);
		destroyQmData(&data);
		free(minterms);
		return 1;
	}

	for(int i = 0 ; i < data.tableCount; i++){
		printf("\nTABLE #%d",i+1);
		displayGroups(data.groupTables[i], data.groupSize[i] - 1);
	}

	printf("\nPrime Implicants:");
	displayPi(data.prime , data.primeCount);

	printf("\nPrime Implicant Chart:");
	displayPiChart(data.prime, data.primeCount, data.piChart, minterms, minCount);
	if(data.essential){
		printf("Essential Implicants: ");
		for(int i = 0; i < data.essentialCount; i++)
			printf("%s ", data.essential[i]);
		printf("\n");
	}

	if(data.uncoveredCount)
	{
		if(data.essentialCount)
		{
			printf("\nUncovered minterms Prime Implicant Chart:");
			displayPiChart(data.prime, data.noEssentialPrimeCount, data.piChart, data.uncoveredTerms, data.uncoveredCount);
			printf("Essential Expressions Removed.\n");
		}

		if(data.newUncoveredCount)
		{
			printf("\nMinterm : ");
			for(int i = 0; i < data.uncoveredCount; i++){
				int exist = 0;
				for(int j = 0; j < data.newUncoveredCount; j++){
					if(data.uncoveredTerms[i] == data.newUncoveredTerms[j])
					{ exist = 1; break; }
				}
				if(exist == 0) printf("%d ",data.uncoveredTerms[i]);
			}
			printf("Removed through Column Domination");

			displayPiChart(data.prime, data.noEssentialPrimeCount,  data.piChart, data.newUncoveredTerms, data.newUncoveredCount);
		}

		printf("\nlet,\n");
		for(int i = 0; i < data.noEssentialPrimeCount; i++)
			printf("  P%d = %s\n", i+1, data.prime[i].expression);

		printf("\nSET Representation:\n");
		for(int i = 0; i < data.setCount; i++)
		{
			if(data.newUncoveredTerms) printf("PI(%d) = ",data.newUncoveredTerms[i]);
			else printf("PI(%d) = ",data.uncoveredTerms[i]);
			for(int j = 0; data.set[i][j]; j++){
				char ch = data.set[i][j];
				printf(j == 0 ? "P%d" : " + P%d", 1+ch-'A');
			}
			printf("\n");
		}

		if(data.petrick)
		{
			printf("\nBy law of Distribution and absorption,\n");
			for(int i = 0; i < data.petrick->processCount; i++)
				printf("\n%s\n",data.petrick->process[i]);

			printf("\n\nMinimum literal SOP Terms: ");
			for(int i = 0; i < data.petrick->SOP_count; i++)
				printf("%s ",data.petrick->SOP_terms[i]);

			printf("\n\n\nPossible Combinations and Cost:\n\n");
			for(int i = 0; i < data.petrick->SOP_count; i++)
			{
				printf("%d. ",i+1);

				char **terms = data.petrick->combinations[i].terms;
				int termsCount = data.petrick->combinations[i].termsCount;

				for(int j = 0; j < termsCount; j++)
					printf( j == 0 ? "%s" : " + %s" , terms[j]);

				printf(" \t\t (%d)\n", data.petrick->cost[i]);
			}

			printf("\nChosen #%d Combination.\n",1+data.petrick->minCostIdx);
		}
	}
	else printf("\nAll Minterms Covered By Essential Implicants.\n");

	printf("\nResult : Y(");
	for(int i = 0; i < var; i++)
		printf( i==0 ? "%c" : ",%c", 'A'+i);
	printf(") = ");

	for(int i = 0; i < data.resultCount; i++)
		printf( i == 0 ? "%s" : " + %s", data.result[i]);
	printf("\n");

	destroyQmData(&data);

	free(minterms);

    return 0;
}

static char* get_input(){
	size_t capacity = 16;
	size_t size = 0;
	char *buffer = malloc(capacity * sizeof(*buffer));
	if(buffer == NULL) return NULL;

	int ch;
	while((ch = getchar()) != '\n' && ch != EOF){

		if (size >= capacity-1){

			capacity *= 2;
			char* temp = realloc(buffer , capacity * sizeof(*temp));
			if(temp == NULL){
				clear_input_buffer();
				free(buffer);
				return NULL;
			}
			buffer = temp;
		}
		buffer[size++] = (char)ch;
	}
	buffer[size] = '\0';
	return buffer;
}

static void clear_separators(char **ch){
	while (**ch == ' ' || **ch == '\t' || **ch == ',') (*ch)++;
}

static int get_minterms(int *minterms , int index, int max_terms){

	char *input = get_input();

	if(input == NULL){
		printf("ERROR: minterms Input failed | Low Memory | get-minterms\n");
		return 0;
	}

	if (input[0] == '\0'){  // No change if empty input
		free(input);
		return 0;
	}

	int initial_index = index;
    char *ptr = input;
    int num;

    while (index < max_terms){

		clear_separators(&ptr);
        if(sscanf(ptr, "%d", &num) == 1){

            if (num < 0) printf("Error: Negative value %d ignored\n", num);
			else if(num >= max_terms) printf("Error: %d is not a valid minterm of %d variables. Ignored\n", num, (int)log2(max_terms));
            else  minterms[index++] = num;

            while (*ptr != ',' && *ptr != ' ' && *ptr != '\t' && *ptr != '\0') ptr++;   // Move pointer past the character
        }

		else if (*ptr != '\n' && *ptr != '\0'){ // Check if there's invalid input

            printf("Error: Invalid input '%c' ignored\n", *ptr);
            ptr++;   // Move pointer past that character
        }
        else break;
    }

	clear_separators(&ptr);
	if(*ptr != '\0')                            //note: *ptr == ptr[0]
		printf("Error: Excess Input: %s\n",ptr);

	free(input);
	return index - initial_index;
}
