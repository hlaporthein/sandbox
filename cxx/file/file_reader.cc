#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <ios>

#include <errno.h>
using namespace std;

void print_hexa(const string file_name) {
	errno = 0;

	ifstream is;
	is.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		is.open(file_name, ifstream::in | ifstream::binary);
		is.exceptions(ifstream::badbit);

		unsigned char buffer[17];
		memset(buffer, 0, 17);
		int s = 0;
		int address = 0;

		while (!is.eof()) {
			is.read((char*) buffer, 16);
			s = is.gcount();

			char buffer_hexa[64] = "";
			for (int i = 0; i < s; i++) {
				char space[2] = "";
				if (i % 4 == 0) {
					snprintf(space, 2, " ");
				}
				char str[32];
				int x = (int) buffer[i];
				snprintf(str, 32, " %s%02x", space, x);
				strcat(buffer_hexa, str);
			}
			printf("%08x:%s\n", address, buffer_hexa);
			address += 16;
		}

		is.close();
	} catch (std::ios_base::failure e) {
		cerr << "ERROR: " << strerror(errno) << endl;
		if (is.is_open()) {
			is.close();
		}
	}
}