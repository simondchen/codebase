#include <stdio.h>
#include <iostream>
using namespace std;
int main(){
	string s="abc";
	char result[3];
	result[0]=s[0];
	result[1]=s[1];
	string ss(result);
	cout<<ss<<endl;
	return 0;
}
