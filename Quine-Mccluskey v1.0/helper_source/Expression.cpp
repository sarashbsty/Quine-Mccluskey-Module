#include<iostream>
#include "helper.h"
using namespace std;

string Expression(const string &binary) {
    string expr = "";
    int variables = binary.size();  

    for (int i = 0; i < variables; i++) {
        char var = 'A' + i;   
        if (binary[i] == '0') {
            expr += var;
            expr += '\'';     // add complement
        } else if (binary[i] == '1') {
            expr += var;      // normal variable
        } else if (binary[i] == '-') {
            // don't care → skip this variable
            continue;
        }
    }
    return expr;
}