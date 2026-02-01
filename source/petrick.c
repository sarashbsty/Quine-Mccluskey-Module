#include "memory_tracker.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "quine.h" // quine struture
#include "helper.h"

static int isexist(int *arr , int size, int item){
	for(int i = 0; i < size; i++)
		if(item == arr[i]) return i;
	return -1;
}

typedef struct {
	int minterm;
    char PI[50];  //prime-implicant
	int count;
	int used;
} petrick_struct;

void petrick(char ***table , int *minterm_uncovered , int uncovered, int primeCount){
	petrick_struct P[20];
	int struct_count = uncovered;

	//Gathering data
	for(int i = 0; i < uncovered; i++){
		int idx = 0;
		for(int j = 0; j < primeCount; j++){
			if(strcmp(table[j][minterm_uncovered[i]], " ") != 0)
				P[i].PI[idx++] = ('A'+j);
		}
		P[i].PI[idx] = '\0';
		P[i].count = idx;
		P[i].minterm = minterm_uncovered[i];
		P[i].used = 0;
	}

	//Display data
	printf("\n\nPetrick used!");
	for(int i = 0; i < struct_count; i++){
		printf("\n%d  -  %c",P[i].minterm , P[i].PI[0]);
		for(int j = 1; j < P[i].count; j++)
			printf(" + %c",P[i].PI[j]);
	}
	printf("\n\n");

	//Processing data
	/*
	char equation[100][10]; int eq_idx = 0;
	for(int i = 0; i < struct_count - 1; i = i+2){
		for(int j = 0; j < P[i].count; j++){
			int k = 0;
			for(; k < P[i+1].count; k++){
				equation[eq_idx++][k] = P[i].PI[j];
	*/



}
