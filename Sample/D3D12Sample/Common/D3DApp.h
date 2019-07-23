#include "d3dUtil.h"
#include "GameTimer.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")


class D3DApp
{
protected:
    D3DApp(HINSTANCE hInstance);
    
public:
    D3DApp(/* args */);
    ~D3DApp();
};

D3DApp::D3DApp(/* args */)
{
}

D3DApp::~D3DApp()
{
}
