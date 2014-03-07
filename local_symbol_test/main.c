int printf(const char* format, ...);

void bye(char *name);

static int hello_jlg(char *name) {
	printf("Hello %s!\n", name);
	return 0;
}

void coucou_jlg(char *name) {
	printf("Hello %s!\n", name);
}

int main() {
	coucou_jlg("Jean-Louis");
	bye("Machin");
	return hello_jlg("JL");
}