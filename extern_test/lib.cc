#include <iostream>

typedef struct {
	char x;
	char y;
} xxx;

extern "C" {
	void __cdecl hello(char x, char* name, xxx y);
}

void hello(char x, char* name, xxx y) {
	std::cout << "Hello " << name << std::endl;
}