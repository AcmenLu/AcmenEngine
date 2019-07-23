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

	void CalculateFrameStates();

	void LogAdapters();
	void LogAdapterOutPuts(IDXGIAdapter* adapter);
	void LogDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:
	static D3DApp* mApp;

	HINSTANCE	mhAppInst = nullptr;
	HWND		mhMainWnd = nullptr;

};
