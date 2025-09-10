#include<stdio.h>
#include "helper.h"

void Expression(char binary[]){
    
    for (int i = 0; binary[i] != '\0'; i++) {
        char var = 'A' + i;   
        if (binary[i] == '0') printf("%c'",var);          // add complement
        else if (binary[i] == '1') printf("%c",var);      // normal variable
        else if (binary[i] == '-')  continue;             // don't care → skip this variable
    }
}