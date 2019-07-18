// https://zhuanlan.zhihu.com/p/28719057
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <stdint.h>

#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <wrl/client.h>
#include <string>
#include <exception>

namespace Acmen
{
	class com_exception : public std::exception
	{
	private:
        HRESULT mResult;

	public:
		com_exception(HRESULT hr) : mResult(hr) {}

		virtual const char* what() const override
		{
			static char s_str[64] = {0};
			sprintf_s(s_str, "Failed with HRESULT of %08X", static_cast<unsigned int>(mResult));
			return s_str;
		}
	};

	inline void ThrowIfFailed(HRESULT hr)
	{
		if(FAILED(hr))
		{
			throw com_exception(hr);
		}
	}
}

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Acmen;
using namespace Microsoft::WRL;
using namespace std;


// const value declaration.
const uint32_t nScreenWidth = 960;
const uint32_t nScreenHeight = 480;
const uint32_t nFrameCount = 2;
const bool bUseWarpDevice = true;

// global value declaration
D3D12_VIEWPORT					g_ViewPort = { 0.0f, 0.0f, static_cast<float>(nScreenWidth), static_cast<float>(nScreenHeight)};
D3D12_RECT						g_ScissorRect = {0, 0, nScreenWidth, nScreenHeight};
ComPtr<IDXGISwapChain3>			g_pSwapChain = nullptr;
ComPtr<ID3D12Device>			g_pDevice = nullptr;
ComPtr<ID3D12Resource>			g_pRenderTargets[nFrameCount];
ComPtr<ID3D12CommandAllocator>	g_pCommandAllocator;
ComPtr<ID3D12CommandQueue>		g_pCommandQueue;
ComPtr<ID3D12RootSignature>		g_pRootSignature;
ComPtr<ID3D12DescriptorHeap>	g_pRtvHeap;
ComPtr<ID3D12PipelineState>		g_pPipelineState;

ComPtr<ID3D12GraphicsCommandList1>	g_pCommandList;

uint32_t						g_nRtvDescriptorSize;

ComPtr<ID3D12Resource>			g_pVertexBuffer;
D3D12_VERTEX_BUFFER_VIEW		g_VertexBufferView;
uint32_t						g_nFrameIndex;
HANDLE							g_hFenceEvent;
ComPtr<ID3D12Fence>				g_pFence;
uint32_t						g_nFenceValue;
struct VERTEX {
	XMFLOAT3	Position;
	XMFLOAT4	Color;
};

wstring 						g_AssetsPath;
std::wstring GetAssetsfullPath(LPCWSTR assetName)
{
	return g_AssetsPath + assetName;
}

void GetAssetPath(WCHAR* path, UINT pathSize)
{
	if(path == nullptr)
	{
		throw std::exception();
	}
	DWORD size = GetModuleNameW(nullptr, path, pathSize);
	if(size == 0 || size == pathSize)
	{
		throw std::exception();
	}
	WCHAR* lastSlash = wcsrchr(path, L"\\");
	if(lastSlash)
	{
		*(lastSlash + 1) = L"\0";
	}
}

void WaitForPreviousFrame()
{
	const uint64_t fence = g_nFenceValue;
	ThrowIfFailed(g_pCommandQueue->Signal(g_pFence.Get(), fence));
	g_nFenceValue++;
	if(g_pFence->GetCompletedValue() < fence)
	{
		ThrowIfFailed(g_pFence->SetEventOnCompletion(fence, g_hFenceEvent));
		WaitForSingleObject(g_hFenceEvent, INFINITE);
	}
	g_nFrameIndex = g_pSwapChain->GetCurrentBackBufferIndex();
}

void CreateRenderTarget()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = nFrameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ThrowIfFailed(g_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_pRtvHeap)));
	g_nRtvDescriptorSize = g_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_pRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for(uint32_t i = 0; i <nFrameCount; i ++)
	{
		ThrowIfFailed(g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&g_pRenderTargets[i])))
		
	}




























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
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = _T("AcmenEngine");

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(0,
						_T("AcmenEngine"),		// name of the window class
						_T("Hello Engine![Direct 3D]"),		// title of the window
						WS_OVERLAPPEDWINDOW,	// window style
						100,					// x-position of the window
						100,					// y-position of the window
						SCREEN_WIDTH,					// width of the window
						SCREEN_HEIGHT,					// height of the window
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

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	bool wasHandled = false;
	switch(message)
	{
		case WM_CREATE:
		{
			break;
		}
		case WM_PAINT:
		{
        	break;
		}
		case WM_SIZE:
		{
        	break;
		}
		 case WM_DESTROY:
		 {
        	break;
		 }
	}
	if (!wasHandled) { result = DefWindowProc (hWnd, message, wParam, lParam); }
	return result;
}