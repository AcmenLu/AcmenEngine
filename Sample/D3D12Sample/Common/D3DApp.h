#pragma once

#include "d3dUtil.h"
#include "GameTimer.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")


class D3DApp
{

protected:
    D3DApp(HINSTANCE hInstance);
	D3DApp(const D3DApp& rhs) = delete;
	virtual ~D3DApp();
    
public:
	static D3DApp* GetApp();
	HINSTANCE	AppInst() const;
	HWND		MainWnd() const;
	float		AspectRation() const;
	bool		Get4xMsaaState() const;
	void		Set4xMsaaState(bool value);
	int			Run();
	virtual bool Initialize();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void CreateRtvAndDsvDescriptorHeaps();
	virtual void OnResize();
	virtual void Update(const GameTimer& gt) = 0;
	virtual void Draw(const GameTimer& gt) = 0;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) {}
	virtual void OnMouseUp(WPARAM btnState, int x, int y) {}
	virtual void OnMouseMove(WPARAM btnState, int x, int y) {}

protected:
	bool InitMainWindow();
	bool InitDirect3D();
	void CreateCommandObjects();
	void CreateSwapChain();

	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer() const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

	void CalculateFrameStats();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:
	static D3DApp* mApp;
	static const int SwapChainBufferCount = 2;

	HINSTANCE	mhAppInst = nullptr;
	HWND		mhMainWnd = nullptr;
	bool		mAppPaused = false;
	bool		mMinimized = false;
	bool		mMaximized = false;
	bool		mResizeing = false;
	bool		mFullScreenState = false;

	bool		m4xMsaaState = false;
	UINT		m4xMsaaQuality = 0;

	GameTimer	mTimer;

	Microsoft::WRL::ComPtr<IDXGIFactory4>		mDxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain>		mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Device>		mD3DDevice;
	
	Microsoft::WRL::ComPtr<ID3D12Fence>			mFence;
	UINT64										mCurrentFence = 0;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>	mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>	mDirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	mCommandList;

	int mCurrbackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource>		mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource>		mDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	mRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	mDsvHeap;

	D3D12_VIEWPORT								mScreenViewport;
	D3D12_RECT									mScissorRect;
	UINT		mRtvDescriptorSize = 0;
	UINT		mDsvDescriptorSize = 0;
	UINT		mCbvSrvUavDescriptorSize = 0;
	
	std::wstring mMainWndCaption = L"d3d App";
	D3D_DRIVER_TYPE			md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT				mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth = 800;
	int mClientHeight = 600;
};
