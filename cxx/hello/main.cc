#include <iostream>

#define NL cout << endl

using namespace std;

extern string yannis;

class Yannis {
	public:
		Yannis operator<<(string str) {
			cout << "Yannis: " << str << endl;
			return *this;
		}
		Yannis operator<<(int a) {
			cout << "Yannis: " << a << endl;
			return *this;
		}

//		void operator and(string str) {
//			cout << "Yannis: operator and " << str << endl;
//		}

		void operator&&(string str) {
			cout << "Yannis: operator && " << str << endl;
		}
};

Yannis y;

bool first() {
	y << "first";
	return false;
}

bool second() {
	y << "second";
	return true;
}

int main() {
	string var = "I'm a var!";

	y << "Hello World!!" << "toto";
	y << (string) var;
	NL;
	y << yannis;
	NL;

	y and "coucou";
	y && "coucou";
	if (first() and second()) {
		y << "true";
	}

	if (first() && second()) {
		y << "true";
	}

	int a = 1;
	a &= 2;
	y << "a=" << a;
	a and_eq 2;
	y << "a=" << a;
	a = 1;
	a and_eq 3;
	y << "a=" << a;

	int foo = 10, bar = 15;
	int res = 0;
    asm volatile("addl  %%ebx,%%eax"
                         :"=r"(res)
                         :"a"(foo), "b"(bar)
                         );
    y << "foo+bar=" << res;
    y << "foo=" << foo;
    y << "bar=" << bar;
    return 0;
}