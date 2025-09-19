#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "quine.h" // quine struture
#include "helper.h" // for digit , make_line , array_to_string

void displayGroups(quine group[] , int var){
	
	int m = (digit(pow(2,var)) + 1) * group[0].mintermCount[0];      // digit + 1 = no. of digits + comma(,)
	int width[4] = {5 , (m<8) ? 8 : m+1 , (var<6) ? 6 : var+1 , 6};
	
	char line[4][200];
	for(int i = 0; i < 4; i++)
		make_line(line[i] ,"─", width[i]+2 ,3);
	
	printf("╭%s┬%s┬%s┬%s╮\n",line[0],line[1],line[2],line[3]);
	printf("│ %-*s │ %-*s │ %-*s │ %-*s │\n", width[0], "GROUP", width[1], "MINTERMS", width[2], "BINARY", width[3], "REDUCE");
	
    for (int i = 0; i <= var; i++) {
		
        if (group[i].count == 0) continue; // skip empty groups
		
		// separator line before every group
		printf("├%s┼%s┼%s┼%s┤\n",line[0],line[1],line[2],line[3]);
		
        for (int j = 0; j < group[i].count; j++){
			
			char *str = malloc((m+1) * sizeof(*str)) , No[5] = " ";
			if(str == NULL) { printf("display-implicants : Memory Allocation failed code : 101"); exit(0); }
			
			//Print group No. only at first
			if(j == 0) snprintf(No,5,"%d",i);      
            
			//Store all minterms associated with this binary to a string var
			array_to_string(group[i].minterms[j] , group[i].mintermCount[j] , str , m+1);    
			
			//In conditional the symbols decay to char* type
			char *symbol = (group[i].combined[j] == 0) ? "  ❌" : "  ✅";                            
		
			printf("│ %-*s │ %-*s │ %-*s │ %-*s │\n", width[0], No, width[1], str, width[2], group[i].binary[j], width[3]+1, symbol);
			free(str);
        }
    }
	printf("╰%s┴%s┴%s┴%s╯\n",line[0],line[1],line[2],line[3]);
}