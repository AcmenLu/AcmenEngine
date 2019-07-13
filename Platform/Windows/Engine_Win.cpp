#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	HWND hwnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = _T("AcmenEngine");

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(0,
						_T("AcmenEngine"),		// name of the window class
						_T("Hello Engine"),		// title of the window
						WS_OVERLAPPEDWINDOW,	// window style
						300,					// x-position of the window
						300,					// y-position of the window
						500,					// width of the window
						400,					// height of the window
						NULL,					// we have no parent window, NULL
						NULL,					// we aren't using menus, NULL
						hInstance,				// application handle
						NULL);					// used with multiple windows, NULL
	ShowWindow(hwnd, nCmdShow);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				RECT rec = {20, 20, 60, 80 };
				HBRUSH brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
				FillRect(hdc, &rec, brush);
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}