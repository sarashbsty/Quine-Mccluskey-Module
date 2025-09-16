#include<stdio.h>
#include<math.h>
#include "quine.h" // quine struture
#include "helper.h" // for digit , make_line

void displayGroups(quine group[] , int var){
	
	int m = (digit(pow(2,var)) + 1) * group[0].mintermCount[0];      // digit + 1 = no. of digits + comma(,)
	
	int width[4] = {5 , (m<8) ? 8 : m+1 , (var<6) ? 6 : var+1 , 6};
	int count = 0;
	
	char line[4][1000];
	for(int i = 0; i < 4; i++)
		make_line(line[i] ,"─", width[i]+2 ,3);

	
	printf("╭%s┬%s┬%s┬%s╮\n",line[0],line[1],line[2],line[3]);
	printf("│ %-*s │ %-*s │ %-*s │ %-*s │\n", width[0], "GROUP", width[1], "MINTERMS", width[2], "BINARY", width[3], "REDUCE");
	
    for (int i = 0; i <= var; i++) {
		
        if (group[i].count == 0) continue; // skip empty groups
		
		// separator line before every group
		printf("├%s┼%s┼%s┼%s┤\n",line[0],line[1],line[2],line[3]);
		
        for (int j = 0; j < group[i].count; j++) {
			
			char str[100] , No[5]; 
			
			//print group No. only at first
			if(j == 0) snprintf(No,5,"%d",i);       
			else { No[0] = ' '; No[1] = '\0'; }
			
			// print all minterms associated with this binary
			int offset = 0 , size = group[i].mintermCount[j];
            for (int k = 0; k < size; k++) {
				int num = group[i].minterms[j][k];
				int written = snprintf(str+offset , sizeof(str)-offset , (k < size-1) ? "%d," : "%d" , num);
				offset += written;
            }
			
			char *symbol = (group[i].combined[j] == 0) ? "  ❌" : "  ✅";  // in conditional the symbols decay to char* type
		
			printf("│ %-*s │ %-*s │ %-*s │ %-*s │\n", width[0], No, width[1], str, width[2], group[i].binary[j], width[3]+1, symbol);
                 
        }
    }
	printf("╰%s┴%s┴%s┴%s╯\n",line[0],line[1],line[2],line[3]);
}