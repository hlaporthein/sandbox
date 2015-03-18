#include <windows.h>
#include <stdio.h>

void hello2(HWND hwnd, HINSTANCE hinst, LPSTR cmdline, int a) {
	char buffer[1024];
	snprintf(buffer, 1024, "hwnd=0x%08x\n", hwnd);
	MessageBox(NULL, buffer, "coucou", 0);
	snprintf(buffer, 1024, "hinst=0x%08x\n", hinst);
	MessageBox(NULL, buffer, "coucou", 0);
	snprintf(buffer, 1024, "cmdline=%s\n", cmdline);
	MessageBox(NULL, buffer, "coucou", 0);
	snprintf(buffer, 1024, "a=%d\n", a);
	MessageBox(NULL, buffer, "coucou", 0);
}
