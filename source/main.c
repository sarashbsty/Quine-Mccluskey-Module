#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "quine.h"
#include "helper.h"

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
	int min_count = get_minterms(minterms , 0 , maxTerms);

	if(min_count == 0){
		printf("No Minterms Entered!\n");
		free(minterms);
		return 0;
	}

	printf("Enter dont care : ");
	int dont_care_count = get_minterms(minterms , min_count , maxTerms);

    int n_terms = min_count + dont_care_count;

    printf("\n\n%d Min terms: ", min_count);
    for (int i = 0; i < min_count; i++)
        printf("%d ", minterms[i]);

    printf("\n%d Dont care: ", dont_care_count);
    for (int i = min_count; i < n_terms; i++)
        printf("%d ", minterms[i]);

    // declare data structures
	quine *group = malloc((var+1) * sizeof(quine));
	if(!group){ printf("ERROR: Create Group struture failed | low Memory | main\n"); exit(0); }

	quine *reduced = malloc((var+1) * sizeof(quine));
	if(!reduced){ printf("ERROR: Create reduced struture failed | low Memory | main\n"); exit(0); }

	quine prime;
	init_quine(&prime);

	fill_group_table(group, minterms, n_terms, var);

	int i = 0 , canReduce = 0;
	do{
		//Reduction
		canReduce = reduce_table(group, reduced, var);

		//display
		if(i) printf("\nReduction #%d:\n", i);
		else  printf("\n\nInitial Grouping:\n");
		displayGroups(group, var);
		i++;

		//get prime-implicants afer each reduction
		prime_implicants(group, &prime, var);

		//clear old group and copy new reduced group
		for (int j = 0; j <= var; j++){
			clear_quine(&group[j]);
			group[j] = reduced[j];
		}
	} while(canReduce);

	free(reduced);
	free(group);

	printf("\nPrime Implicants:\n");
	displayPi(&prime);

	// prime_implicant_chart_table
	int **table = createPiChart(&prime, minterms, min_count, var);
	if(!table){ printf("\nERROR: Table creation failed | Low Memory | main\n"); exit(0); }

	printf("\nPrime Implicants Chart:\n");
    displayPiChart(&prime, table, minterms, min_count);


	int *uncoveredTerms = malloc(min_count * sizeof(int));
	if(!uncoveredTerms) { printf("\nminterm_uncovered array allocation failed | low memory | main\n"); exit(0); }

	//Gather Uncovered Minterms if exist
	int uncoveredCount = 0;
	for(int i = 0; i < min_count; i++){
		int exist = 0;
		for(int j = 0; j < prime.count; j++){
			if(prime.minimal[j] == 0) continue;
			if(table[j][minterms[i]] != 0){
			 	exist = 1; break;
			}
		}
		if(!exist) uncoveredTerms[uncoveredCount++] = minterms[i];
	}

	if(uncoveredCount == 0)
	{
		printf("All Minterms covered by essentialPI-implicants:");
		for(int i = 0; i < prime.count; i++)
			if(prime.minimal[i] == 1)
				printf(" %s",prime.expression[i]);
	}
	else
	{
		if(uncoveredCount == min_count) printf("No Essential Prime-Implicants\n");
		else{
			printf("Essential-implicants:");
			for(int i = 0; i < prime.count; i++)
				if(prime.minimal[i] == 1)
					printf(" %s",prime.expression[i]);

			printf("\n\nUncovered Minterms PI Chart:\n");
			displayPiChart(&prime, table, uncoveredTerms, uncoveredCount);
		}

		char **setArr = malloc(uncoveredCount * sizeof(*setArr));
		if(!setArr) { printf("\nset coverage array allocation failed | low memory | main\n"); exit(0); }

		printf("\n#Set Coverage (column):\n");
		int setArrCount = getSetCoverage(setArr, &prime, table, uncoveredTerms, uncoveredCount);

		printf("\n#Column Domination by eliminating equal and Supersets:\n");
		int new_uncovered_count = column_domination(setArr, &setArrCount, uncoveredTerms, uncoveredCount);

		if(new_uncovered_count < uncoveredCount){
			uncoveredCount = new_uncovered_count;
			printf("\nColumn Reduced Uncovered Minterms PI Chart:\n");
			displayPiChart(&prime, table, uncoveredTerms, uncoveredCount);
		}

		//Petrick Algorithm
		petrick(&prime,setArr,setArrCount,var);
		free_2d_pointer(setArr,setArrCount);
	}

	printResult(&prime,var);

	free(uncoveredTerms);
	free_2d_pointer((char**)table , prime.count);
	clear_quine(&prime);
	free(minterms);
    return 0;
}
