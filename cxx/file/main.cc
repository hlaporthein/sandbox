#include <iostream>
#include <libgen.h>
using namespace std;

#include "file_reader.hpp"

int main (int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage: " << basename(argv[0]) << " <file>\n";
		return 1;
	}

	string file = argv[1];
	print_hexa(file);
	return 0;
}