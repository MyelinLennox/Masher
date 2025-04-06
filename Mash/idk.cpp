#include <Windows.h>

const wchar_t CLASS_Name[] = L"WindowClass";

LRESULT CALLBACK Windowproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	WNDCLASS wc = { };

	wc.lpfnWndProc = Windowproc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_Name;

	RegisterClass(&wc);

	CreateWindow(CLASS_Name, L"Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 500, 500, 0, 0, hInstance, 0);

	MSG msg = { };

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
WNDCLASS wc = { };

wc.lpfnWndProc = WndProc;
wc.hInstance = hInstance;
wc.lpszClassName = CLASS_Name;

