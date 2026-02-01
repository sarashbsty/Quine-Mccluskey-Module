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
	system("cls");

    #ifdef _WIN32
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

    printf("\n");
    display_implicants(&prime);

    essential_implicants(&prime, minterms, min_count , var);

	printResult(&prime,var);

	clear_quine(&prime);
	free(reduced);
	free(group);
	free(minterms);

    return 0;
}
