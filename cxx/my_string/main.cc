#include <iostream>
#include <string>

#include <stdlib.h>
#include <strings.h>

using namespace std;

class my_string {
	private:
		char *str = NULL;


	public:
		my_string() {
		};

		my_string(const char *str) {
			cout << "Copy constructor A" << endl;
			this->_assign(str);
		};

		~my_string() {
			free(this->str);
			this->str = NULL;
		}

		my_string& operator=(const char * str) {
			cout << "= A(const char * str)" << endl;
			this->_assign(str);
			return *this;
		}

		my_string& operator=(const string& s) {
			cout << "= A(const string& s)" << endl;
			this->_assign(s.c_str());
			return *this;
		}

		void println() {
			cout << this->str << endl;
		}

	private:
		void _assign(const char * str) {
			if (this->str) {
				free(this->str);
			}
			size_t size = strlen(str) + 1;
			this->str = (char *) malloc(sizeof(char) * size);
			strcpy(this->str, str);
		}
};

int main() {
	my_string str = "Hello world!";
	str.println();
	str = "Coucou";
	str.println();
	string a = "Salut";
	str = a;
	str.println();
	return 0;
}