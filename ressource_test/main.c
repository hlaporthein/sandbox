#include <windows.h>
#include <stdio.h>

#define IDM_TEST 1
#define IDM_HELLO 2
#define IDM_GOODBYE 3
#define IDM_EXIT 4

char s_app_name[32] = "Our First Window";

LRESULT CALLBACK proc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	switch (uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_COMMAND:
			switch(wParam) {
				case IDM_TEST:
					MessageBox(NULL, "You selected Test menu item", s_app_name, MB_OK);
					break;
				case IDM_HELLO:
					MessageBox(NULL, "Hello, my friend", s_app_name, MB_OK);
					break;
				case IDM_GOODBYE:
					MessageBox(NULL, "See you again, bye", s_app_name, MB_OK);
					break;
				default:
					DestroyWindow(hwnd);
			}
			break;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

int main() {
	printf("COLOR_WINDOW=%d\n", COLOR_WINDOW);
	HINSTANCE hInst = GetModuleHandle(NULL);
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "SimpleWinClass";
	wc.hIconSm = 0;

	RegisterClassEx(&wc);
	HMENU hMenu = LoadMenu(hInst, "FirstMenu");

	HWND hWnd = CreateWindowEx(WS_EX_LTRREADING, wc.lpszClassName, "Our First Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, hMenu, hInst, NULL);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	MSG msg;
	BOOL bRet;

	while ((bRet = GetMessage(&msg, hWnd, 0, 0)) != 0) {
		if (bRet == -1) {
			// handle the error and possibly exit
			break;
		} else {
			//TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	return msg.wParam;
}