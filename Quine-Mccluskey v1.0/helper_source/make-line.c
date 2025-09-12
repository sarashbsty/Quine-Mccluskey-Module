#include<string.h>
#include "helper.h"

void make_line(char str[] , const char* c , int n , int byte){
	for (int i = 0; i < n; i++) {
		memcpy(&str[i*byte], c, byte);
	}
	str[n*byte] = '\0';
}