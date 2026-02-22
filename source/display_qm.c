#include <stdio.h>
#include <stdlib.h>
#include "parse_input.h"
#include "qmMinimizer.h"
#include "memory.h"

void display_qm(const char *input)
{
	if(!input) puts("Failed to read input");

	int var, *minterms = NULL , minCount = 0 , *dontCares = NULL, dontCareCount = 0;

	int error = parse_input_json(input, &var, &minterms, &minCount, &dontCares, &dontCareCount);

	if(error == 1){	puts("Parsing input failed due to invalid json"); return; }

	else if(error == 2){ puts("Invalid input variable or not Found"); return; }

	else if(error == 3){ puts("memory Allocation fail"); return; }

	else if(error == 4){ puts("Invalid Datatypes Found"); return; }

	int *tmp = realloc(minterms, (minCount + dontCareCount) * sizeof(*tmp));
	if(!tmp){
		puts("minterm input reallocation Failed");
		free(minterms);
		free(dontCares) ;
		return;
	}
	minterms = tmp;

	//inserting dontcare elements at the end of minterms
	for(int i = 0; i < dontCareCount; i++)
		minterms[minCount + i] = dontCares[i];

	//show minterms
	 printf("%d Min terms: ", minCount);
    for (int i = 0; i < minCount; i++)
        printf("%d ", minterms[i]);

	//show dontCares
    printf("\n%d Dont care: ", dontCareCount);
    for (int i = 0; i < dontCareCount; i++)
        printf("%d ", minterms[minCount + i]);
	printf("\n");

	//call minimizer
	qmData data = qmMinimizer(minterms, minCount, dontCares, dontCareCount, var);
	if(data.error){
		puts(data.errorMsg);
		free(minterms);
		free(dontCares) ;
		return;
	}

	minterms = NULL;
	dontCares = NULL;

	//Display Portion
	for(int i = 0 ; i < data.tablesCount; i++){
		printf("\nTABLE #%d",i+1);
		displayTable(&data.tables[i], var);
	}

	printf("\nPrime Implicants:");
	displayPi(data.prime , data.primeCount);

	printf("\nPrime Implicant Chart:");
	displayPiChart(data.prime, data.primeCount, data.piChart, data.minterms, data.minCount);
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

				printf(" \t\t (%d)\n", data.petrick->combinations[i].cost);
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
}





