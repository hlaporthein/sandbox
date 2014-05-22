#include <stdio.h>
#include <string.h>

void print_array(int* a, size_t s) {
	printf("{");
	for (int i = 0; i < s; i++) {
		if (i > 0) {
			printf(", ");
		}
		printf("%d", a[i]);
	}
	printf("}\n");
}

typedef struct {
	int a[4];
} array;

int main() {
	int a[4] = {3, 6, 9, 12};
	print_array(a, 4);

	int b[4];

	array *cp = (array*) a;
	array *dp = (array*) b;
	*dp = *cp;
	print_array(b, 4);
	a[0] = 123;
	print_array(a, 4);

	return 0;
}