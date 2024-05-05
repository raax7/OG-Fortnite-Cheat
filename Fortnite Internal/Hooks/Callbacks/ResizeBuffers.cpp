#include "../Hooks.h"

#include "../../Configs/Config.h"
#include "../../Game/Game.h"
#include "../../Utilities/Logger.h"

HRESULT __stdcall Hooks::ResizeBuffers::ResizeBuffers(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    if (RaaxDx::RenderTargetView)
    {
        RaaxDx::DeviceContext->OMSetRenderTargets(0, 0, 0);
        RaaxDx::RenderTargetView->Release();
    }

    HRESULT hr = ResizeBuffersOriginal(pThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);

    ID3D11Texture2D* pBuffer;
    pThis->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
    // Perform error handling here!

    RaaxDx::Device->CreateRenderTargetView(pBuffer, NULL, &RaaxDx::RenderTargetView);
    // Perform error handling here!
    pBuffer->Release();

    RaaxDx::DeviceContext->OMSetRenderTargets(1, &RaaxDx::RenderTargetView, NULL);

    // Set up the viewport.
    D3D11_VIEWPORT vp;
    vp.Width = Width;
    vp.Height = Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    RaaxDx::DeviceContext->RSSetViewports(1, &vp);

    return hr;
}