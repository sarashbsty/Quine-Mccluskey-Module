#ifdef _WIN32
#include <windows.h>
#endif
#include "memory_tracker.h"

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

	// prime_implicant_chart_table
	char ***table = create_table(prime.count , pow(2,var) , 6);
	if(table == NULL){ printf("\nERROR: Table creation failed | Low Memory | main\n"); exit(0); }

	//store uncovered_minterms
	int *uncovered_terms = malloc(min_count * sizeof(int));
	if(!uncovered_terms) { printf("\nminterm_uncovered array allocation failed | low memory | main\n"); exit(0); }

	printf("\nPrime Implicants:\n");
	displayPi(&prime);

	createPiChart(&prime, table, minterms, min_count);

	printf("\nPrime Implicants Chart:\n");
    displayPiChart(&prime, table, minterms, min_count);

	int uncovered_count = checkCoverage(&prime, uncovered_terms, minterms, min_count);

	if(uncovered_count > 0){

		if(uncovered_count == min_count) printf("No Essential Prime-Implicants\n");
		else{
			printf("Essential-implicants:");
			for(int i = 0; i < prime.count; i++)
				if(prime.minimal[i] == 1)
					printf(" %s",prime.expression[i]);

			printf("\n\nUncovered Minterms PI Chart:\n");
			displayPiChart(&prime, table, uncovered_terms, uncovered_count);
		}

		char **setArr = malloc(uncovered_count * sizeof(*setArr));
		if(!setArr) { printf("\nset coverage array allocation failed | low memory | main\n"); exit(0); }

		printf("\n#Set Coverage (column):\n");
		int setArrCount = getSetCoverage(setArr, &prime, table, uncovered_terms, uncovered_count);

		printf("\n#Column Domination by eliminating equal and Supersets:\n");
		int new_uncovered_count = column_domination(setArr, &setArrCount, uncovered_terms, uncovered_count);

		if(new_uncovered_count < uncovered_count){
			uncovered_count = new_uncovered_count;
			printf("\nColumn Reduced Uncovered Minterms PI Chart:\n");
			displayPiChart(&prime, table, uncovered_terms, uncovered_count);
		}

		//Petrick Algorithm
		petrick(&prime,setArr,setArrCount,var);
		free_2d_pointer(setArr,setArrCount);

	}
	else{
		printf("All Minterms covered by essential-implicants:");
		for(int i = 0; i < prime.count; i++)
			if(prime.minimal[i] == 1)
				printf(" %s",prime.expression[i]);
	}

	printResult(&prime,var);

	free(uncovered_terms);
	free_3d_pointer(table , prime.count , pow(2,var));
	clear_quine(&prime);
	free(minterms);
    return 0;
}
