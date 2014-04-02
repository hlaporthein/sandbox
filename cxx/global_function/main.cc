#include <iostream>
#include <cstdio>

class A {
	public:
		static void hello() {
			std::cout << "Hello\n";
		}
};

namespace X {
	class A {
		public:
			static void hello() {
				std::cout << "Coucou\n";
			}
	};
}

using namespace X;

int count = 0;

int main() {
	::A::hello();
	X::A::hello();

	int count = 1;
	std::printf("count=%d\n", count);
	std::printf("::count=%d\n", ::count);

	return 0;
}
