#include "RaaxDx.h"

#include "../../Utilities/Error.h"

#include "../../External-Libs/LazyImporter.h"
#include "../../External-Libs/skCrypter.h"

#include "../Hooks.h"

#include "../../External-Libs/minhook/include/MinHook.h"

// DX11
#include <dxgi.h>
#include <d3d11.h>
#include "../../Utilities/Logger.h"
#include "../../Globals.h"

// DX12
//#include <dxgi.h>
//#include <d3d12.h>

#pragma comment(lib, "d3d11.lib")

RaaxDx::Status RaaxDx::Init() {
	if (Initalized) {
		return Status::AlreadyInitialized;
	}

	Initalized = true;

	// Detect DX Version
	HMODULE DXModule = NULL;

	if (DXModule = LI_FN(GetModuleHandleA).safe()(skCrypt("d3d11.dll")); DXModule != NULL) {
		DXVersion = 11;
	}
	else if (DXModule = LI_FN(GetModuleHandleA).safe()(skCrypt("d3d12.dll")); DXModule != NULL) {
		DXVersion = 12;
	}
	else {
		return Status::DxNotFound;
	}

	if (DXVersion == 11) {
		// Create D3D11 Device and SwapChain
		D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
		const D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1 };

		DXGI_RATIONAL RefreshRate;
		ZeroMemory(&RefreshRate, sizeof(DXGI_RATIONAL));
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;

		DXGI_MODE_DESC BufferDesc;
		ZeroMemory(&BufferDesc, sizeof(DXGI_MODE_DESC));
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC SampleDesc;
		ZeroMemory(&SampleDesc, sizeof(DXGI_SAMPLE_DESC));
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 1;
		SwapChainDesc.OutputWindow = GetForegroundWindow();
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ID3D11Device* Device = nullptr;
		ID3D11DeviceContext* DeviceContext = nullptr;
		IDXGISwapChain* SwapChain = nullptr;

		HRESULT Result = ((HRESULT(*)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**))D3D11CreateDeviceAndSwapChain)(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, FeatureLevels, 1, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, &FeatureLevel, &DeviceContext);
		if (FAILED(Result)) {
			return Status::CreateFailed;
		}

		// Create Method Table (blindly pasted from kiero hook tbh. no clue what all these magic numbers are supposed to be)
		VFT = (uint64_t*)calloc(205, sizeof(uint64_t));
		if (VFT == NULL) {
			return Status::MemoryError;
		}

		memcpy(VFT, *(uint64_t**)SwapChain, 18 * sizeof(uint64_t));
		memcpy(VFT + 18, *(uint64_t**)Device, 43 * sizeof(uint64_t));
		memcpy(VFT + 18 + 43, *(uint64_t**)DeviceContext, 144 * sizeof(uint64_t));

		MH_Initialize();

		SwapChain->Release();
		Device->Release();
		DeviceContext->Release();

		Initalized = true;
	}
	else if (DXVersion == 12) {
		// TO-DO: Add DX12 Support
		return Status::DxNotSupported;
	}

	return Status::Success;
}
RaaxDx::Status RaaxDx::Hook() {
	// Init MinHook
	MH_STATUS InitStats = MH_Initialize();
	if (InitStats != MH_OK && InitStats != MH_ERROR_ALREADY_INITIALIZED) {
		return Status::InitMH;
	}

	if (SDK::IsValidPointer(VFT) == false) {
		THROW_ERROR(std::string(skCrypt("VFT not initalized!")), true);
	}

	// Hook Present
	if (Initalized) {
		if (MH_CreateHook((void*)VFT[8], &Hooks::Present::Present, (void**)&Hooks::Present::PresentOriginal) != MH_OK || MH_EnableHook((void*)VFT[8]) != MH_OK) {
			return Status::UnknownError;
		}

		if (MH_CreateHook((void*)VFT[13], &Hooks::ResizeBuffers::ResizeBuffers, (void**)&Hooks::ResizeBuffers::ResizeBuffersOriginal) != MH_OK || MH_EnableHook((void*)VFT[13]) != MH_OK) {
			return Status::UnknownError;
		}

		return Status::Success;
	}

	// If not initalized, throw error
	THROW_ERROR(std::string(skCrypt("RaaxDx not initalized")), false);
}
void RaaxDx::Unhook() {
	// Init MinHook
	MH_STATUS InitStats = MH_Initialize();
	if (InitStats != MH_OK && InitStats != MH_ERROR_ALREADY_INITIALIZED) {
		return;
	}

	// Hook Present
	if (Initalized && VFT) {
		MH_DisableHook((void*)VFT[8]);
		MH_RemoveHook((void*)VFT[8]);
		MH_DisableHook((void*)VFT[13]);
		MH_RemoveHook((void*)VFT[13]);
		MH_Uninitialize();

		delete VFT;
		VFT = nullptr;

		DEBUG_LOG(LOG_INFO, std::string(skCrypt("Unhooked")));

		return;
	}

	// If not initalized, throw error
	THROW_ERROR(std::string(skCrypt("RaaxDx not initalized")), false);
}

RaaxDx::Status RaaxDx::InitImGui(IDXGISwapChain* Swapchain) {
	if (SUCCEEDED(Swapchain->GetDevice(__uuidof(ID3D11Device), (void**)&Device))) {
		Device->GetImmediateContext(&DeviceContext);

		DXGI_SWAP_CHAIN_DESC Desc;
		Swapchain->GetDesc(&Desc);
		Window = Desc.OutputWindow;

		ID3D11Texture2D* BackBuffer;
		Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
		Device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);
		BackBuffer->Release();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui_ImplWin32_Init(Window);
		ImGui_ImplDX11_Init(Device, DeviceContext);

#if WNDPROC_INLINE_HOOK
		LONG_PTR WndProc = GetWindowLongPtrW(Window, GWLP_WNDPROC);

		if (WndProc == 0) {
			return Status::WindowError;
		}

		if (MH_CreateHook((void*)WndProc, &Hooks::WndProc::WndProc, (void**)&Hooks::WndProc::WndProcOriginal) != MH_OK || MH_EnableHook((void*)WndProc) != MH_OK) {
			return Status::WindowError;
		}
#else
		Hooks::WndProc::WndProcOriginal = (WNDPROC)SetWindowLongPtr(Window, GWLP_WNDPROC, (__int3264)(LONG_PTR)Hooks::WndProc::WndProc);
#endif

		return Status::Success;
	}

	return Status::DxNotSupported;
}
