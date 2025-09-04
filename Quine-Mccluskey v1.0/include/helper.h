#pragma once
using namespace std;

string ToBinary(int minterm, int variables);
int count_1s(const string &binary);
string Expression(const string &binary);
bool is_exist(string arr[], int size, string item) ;