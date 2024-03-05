#pragma once
#include "../../Globals.h"

#include <d3d11.h>
#include <dxgi.h>

namespace RaaxDx {
	enum class Status {
		AlreadyInitialized = -1,
		DxNotFound = -2,
		DxFunctionNotFound = -3,
		CreateFailed = -4,
		NotInitalized = -5,
		BadParams = -6,
		UnknownError = -7,
		WindowError = -8,

		Success = 0,
	};

	/* Init RaaxDx for DX hooking */
	Status Init();

	/* Hook DX rendering */
	Status Hook();
	/* Unhook any DX rendering. Cannot rehook after without DLL restart! */
	void Unhook();

	void InitImGui(IDXGISwapChain* Swapchain);


	inline ID3D11Device* Device;
	inline ID3D11DeviceContext* DeviceContext;
	inline ID3D11RenderTargetView* RenderTargetView;
	inline HWND Window = nullptr;

	inline uintptr_t* VFT = nullptr;

	inline bool Initalized = false;

	inline int DXVersion;
}