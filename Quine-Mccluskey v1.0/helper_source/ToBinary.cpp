#include<iostream>
#include "helper.h"
using namespace std;

string ToBinary(int minterm, int variables) {
    string binary = "";
    for (int i = variables - 1; i >= 0; i--) {
        binary += ((minterm >> i) & 1) ? '1' : '0';
    }
    return binary;
}