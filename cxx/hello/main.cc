#include <iostream>
#define NL cout << endl

using namespace std;

extern string yannis;

class Yannis {
	public:
		void operator<<(string str) {
			cout << "Yannis: " << str << endl;
		}
};

int main() {
	string var = "I'm a var!";

	Yannis y;
	y << "Hello World!!";
	y << (string) var;
	NL;
	y << yannis;
	NL;
}