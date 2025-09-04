#include<iostream>
#include "helper.h"
using namespace std;

bool is_exist(string arr[], int size, string item) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == item) 
            return true;
    }
    return false;
}