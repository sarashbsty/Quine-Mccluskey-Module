#include<stdio.h>
#include <string.h>
#include "helper.h"

int is_exist(char arr[][100], const char item[], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i], item) == 0)
            return 1;
    }
    return 0;
}
