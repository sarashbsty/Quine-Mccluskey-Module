#include<stdio.h>
#include<string.h>
#include "quine.h" // quine struture
#include "helper.h"

void displayGroups(quine group[] , int variables){
	
	int width[4] = {5,20,15,8};
	int n = 2 + width[0] + 3 + width[1] + 3 + width[2] + 3 + width[3] + 2;
	int count = 0;
	
	char line[4][22*3+1];
	for(int i = 0; i < 4; i++)
		make_line(line[i] ,"─", width[i]+2 ,3);

	
	printf("╭%s┬%s┬%s┬%s╮\n",line[0],line[1],line[2],line[3]);
	printf("│ %-5s │ %-20s │ %-15s │ %-8s │\n", "Group", "MinTerm", "Binary", "Combined");
	
    for (int i = 0; i <= variables; i++) {
		
        if (group[i].count == 0) continue; // skip empty groups
		
		// separator line before every group
		printf("├%s┼%s┼%s┼%s┤\n",line[0],line[1],line[2],line[3]);
		
        for (int j = 0; j < group[i].count; j++) {
			
			char str[100] , No[2]; 
			
			//print group No. only at first
			if(j == 0) snprintf(No,2,"%d",i);       
			else { No[0] = ' '; No[1] = '\0'; }
			
			// print all minterms associated with this binary
			int offset = 0 , size = group[i].mintermCount[j];
            for (int k = 0; k < size; k++) {
				int num = group[i].minterms[j][k];
				int written = snprintf(str+offset , sizeof(str)-offset , (k < size-1) ? "%d," : "%d" , num);
				offset += written;
            }
			
			char *symbol = (group[i].combined[j] == 0) ? "   ❌" : "   ✅";  // in conditional the symbols decay to char* type
		
			printf("│ %-5s │ %-20s │ %-15s │ %-9s │\n", No , str , group[i].binary[j] , symbol);
                 
        }
    }
	printf("╰%s┴%s┴%s┴%s╯\n",line[0],line[1],line[2],line[3]);
}