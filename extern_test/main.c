typedef struct {
	char x;
	char y;
} xxx;

void __stdcall hello(char x, char* name, xxx y);

int main() {
	xxx y;
	hello('a', "Yannis", y);
	//_Z5hellov(); // Mangled
	return 0;
}