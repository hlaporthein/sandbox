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

		const int buffer_size = 1 << 16;

		char buffer[buffer_size];
		memset(buffer, 0, buffer_size);
		int s = 0;
		int address = 0;

		while (!is.eof()) {
			is.read(buffer, buffer_size);
			s = is.gcount();

			for (int i = 0; i < s; i += 16) {
				char buffer_hexa[64] = "";
				int c = s - i;
				if (c >= 16) {
					c = 16;
				}
				for (int j = 0; j < c; j++) {
					char space[2] = "";
					if (j % 4 == 0) {
						snprintf(space, 2, " ");
					}
					char str[32];
					int x = (int) (unsigned char) buffer[i + j];
					snprintf(str, 32, " %s%02x", space, x);
					strcat(buffer_hexa, str);
				}
				printf("%08x:%s\n", address, buffer_hexa);
				address += 16;
			}
		}

		is.close();
	} catch (std::ios_base::failure e) {
		cerr << "ERROR: " << strerror(errno) << endl;
		if (is.is_open()) {
			is.close();
		}
	}
}