#include<stdio.h>
#include<string.h>
#include "quine.h" // quine struture


void displayGroups(quine group[] , int variables){
	
	int line = 2+5+3+20+3+15+3+8+2;
    char str_line[500] , str_line2[500];
	if (line >= sizeof(str_line)) line = sizeof(str_line) - 1; // prevent overflow
	
	memset(str_line, '=', line);
    str_line[line] = '\0';
	memset(str_line2, '-', line);
    str_line2[line] = '\0';
	
	printf("%s\n",str_line);
	printf("| %-5s | %-20s | %-15s | %-8s |\n", "Group", "MinTerm", "Binary", "Combined");
    printf("%s\n",str_line);
	
    for (int i = 0; i <= variables; i++) {
		
        if (group[i].count == 0) continue; // skip empty groups
		
        for (int j = 0; j < group[i].count; j++) {
		
			// print all minterms associated with this binary
            char str[100]; 
			int offset = 0 , size = group[i].mintermCount[j];
            for (int k = 0; k < size; k++) {
				int num = group[i].minterms[j][k];
				int written = snprintf(str+offset , sizeof(str)-offset , (k < size-1) ? "%d," : "%d" , num);
				offset += written;
            }
					
			char *symbol = (group[i].combined[j] == 0) ? "❌" : "✅";  // in conditional the symbols decay to char* type
			if(j == 0)
				printf("| %-5d | %-20s | %-15s | %-9s |", i , str , group[i].binary[j] , symbol);
			else
				printf("| %-5s | %-20s | %-15s | %-9s |", " " , str , group[i].binary[j] , symbol);
			printf("\n");
                 
        }
		
		// separator line after every group
		printf("%s\n",str_line2);  
    }
}