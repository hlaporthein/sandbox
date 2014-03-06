int printf(const char* format, ...);

static int hello(name) {
	printf("Hello %s!\n", name);
	return 0;
}

int main() {
	return hello("JL");
}