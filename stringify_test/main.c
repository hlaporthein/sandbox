#include <stdio.h>

#define STRING(x) STRING2(x)
#define STRING2(x) #x

#define JLG Jean-Louis  GUENEGO

int main() {
	printf("Hello " STRING2(JLG) "\n");
	printf("Hello " STRING(JLG) "\n");
	return 0;
}

