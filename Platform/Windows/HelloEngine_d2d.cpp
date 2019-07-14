#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <d2d1.h>

ID2D1Factory			*pFactory = nullptr;
ID2D1HwndRenderTarget	*pRenderTarget = nullptr;
ID2D1SolidColorBrush	*pLightSlateGrayBrush  = nullptr;
ID2D1SolidColorBrush	*pCornflowerBlueBrush = nullptr;

template<class T>
inline void SafeRelease(T **ppInterfaceToRelease)
{
	if(*ppInterfaceToRelease != nullptr)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = nullptr;
	}
}

HRESULT CreateGraphicsResources(HWND hWnd)
{
	HRESULT hr = S_OK;
	if(pRenderTarget == nullptr)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, size),
			&pRenderTarget);
		if(SUCCEEDED(hr))
		{
			hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray), &pLightSlateGrayBrush);
		}
		if (SUCCEEDED(hr))
		{
			hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &pCornflowerBlueBrush);
		}
	}
	return hr;
}

void DiscardGraohicsResources()
{
	SafeRelease(&pRenderTarget);
	SafeRelease(&pLightSlateGrayBrush);
	SafeRelease(&pCornflowerBlueBrush);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	HWND hwnd;
	WNDCLASSEX wc;
	// initialize COM
    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
		return -1;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = _T("AcmenEngine");

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(0,
						_T("AcmenEngine"),		// name of the window class
						_T("Hello Engine![Direct 2D]"),		// title of the window
						WS_OVERLAPPEDWINDOW,	// window style
						100,					// x-position of the window
						100,					// y-position of the window
						960,					// width of the window
						540,					// height of the window
						NULL,					// we have no parent window, NULL
						NULL,					// we aren't using menus, NULL
						hInstance,				// application handle
						NULL);					// used with multiple windows, NULL
	ShowWindow(hwnd, nCmdShow);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	CoUninitialize();
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