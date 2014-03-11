#include <iostream>

using namespace std;

int main() {
	char *var = "I'm a var!";
	cout << "Hello World!!" << endl << "I'm a C++ program" << endl;
	#warning Pas bien.
	cout << (string) var;
}