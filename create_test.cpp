// Created on Wed Feb 20 2013

#include <iostream>
#include "create.h"

using namespace std;

int main() 
{
	cout << "Hello, World!" << endl;
	create_connect();
	move_while(500 , 1000);
	cout << GDIST << endl;
	create_disconnect();
	return 0;
}
