#include "helper.h"

int digit(int x){
	int count = 0;
	while(x > 0){
		count++;
		x = x/10;
	}
	return count;
}