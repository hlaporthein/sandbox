#include <stdio.h>

double my_sqrt(double a) {
	asm("fld %0;"
		"fsqrt;"
		"fstp %0;"
		: "+m" (a));
	return a;
}

double add(double a, double b) {
	double c = 1.0;
	asm("fld %1;"
		"fld %2;"
		"faddp st(1), st(0);"
		"fstp %0;"
		: "=m" (c) : "m" (a), "m" (b));
	return c;
}

void display(char *string) {
	int a;
	asm("push %[string];"
		"call _puts;"
		: "=a"(a) : [string] "r" (string));
}

void display2(char *string) {
	asm("push %[string];"
		"call _puts;"
		"pop eax;"
		:: [string] "r" (string));
}

int main() {
	double a = 2;
	double b = 5.67;
	printf("sqrt(%f) = %f\n", a, my_sqrt(a));
	printf("%f + %f = %f\n", a, b, add(a, b));
	display2("finished.");
	return 0;
}