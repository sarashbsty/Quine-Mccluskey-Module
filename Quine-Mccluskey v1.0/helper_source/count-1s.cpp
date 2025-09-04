#include<iostream>
#include "helper.h"
using namespace std;

int count_1s(const string &binary){
	int count = 0;
	for(int i = 0; i < binary.size(); i++)
		if(binary[i] == '1') count++;
	return count;
}
