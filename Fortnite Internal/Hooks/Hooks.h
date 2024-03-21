#pragma once
#include <Windows.h>

#ifdef _IMGUI
#include <d3d11.h>
#include <dxgi.h>
#include <mutex>
#include "RaaxDx/RaaxDx.h"
#include "../External-Libs/ImGui/imgui.h"
#include "../External-Libs/ImGui/imgui_impl_win32.h"
#include "../External-Libs/ImGui/imgui_impl_dx11.h"
#endif

#include "../Game/SDK/Classes/Basic.h"
#include "../Game/SDK/Classes/Engine_Structs.h"

namespace Hooks {
	// Virtual Function Table Hook

	class VFTHook {
	private:
		void** VFT;				// The virtual function table
		uintptr_t VFTIndex;		// The index of the virtual function
		void* Original;			// The original function
	public:
		/*
		* @brief Hook a virtual function
		*
		* @param VFT The virtual function table
		* @param VFTIndex The index of the virtual function
		* @param Original The original function
		* @param Hook The hook function
		*/
		template <typename T>
		VFTHook(void** VFT, const uintptr_t VFTIndex, T& Original, void* Hook);

		/*
		* @brief Revert the VFT hook back to the original function
		*/
		~VFTHook();
	};



	// Hooks

#ifdef _IMGUI
	namespace Present {
		typedef HRESULT(__stdcall* oPresent) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		inline oPresent PresentOriginal = nullptr;

		HRESULT __stdcall Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

		inline bool ImGuiBeenSetup = false;

		inline std::mutex PresentMutex;

		inline ImFont* Font;
		inline ImFont* LargeFont;
	}

	namespace ResizeBuffers {
		using ResizeBuffersParams = HRESULT(*)(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
		inline ResizeBuffersParams ResizeBuffersOriginal = nullptr;

		inline bool Resized = false;

		HRESULT __stdcall ResizeBuffers(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	}

	namespace WndProc {
		using WndProcParams = LRESULT(*)(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		inline WndProcParams WndProcOriginal = nullptr;

		inline std::recursive_timed_mutex WndProcMutex;

		inline HWND Window;

		LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
#endif // _IMGUI

	namespace DrawTransition {
		using DrawTransitionParams = void(*)(uintptr_t this_, uintptr_t Canvas);
		inline DrawTransitionParams DrawTransitionOriginal = nullptr;

		void DrawTransition(uintptr_t this_, uintptr_t Canvas);

		inline Hooks::VFTHook* Hook = nullptr;
	}

	namespace CalculateShot {
		using CalcShotParams = SDK::FTransform*(*)(void**, uintptr_t, uintptr_t);
		inline CalcShotParams CalculateShotOriginal = nullptr;

		SDK::FTransform* CalculateShotHook(void** arg0, uintptr_t arg1, uintptr_t arg2);
	}

	namespace GetPlayerViewpoint {
		using GetPlayerViewpointParams = void(*)(void* this_, SDK::FVector* Location, SDK::FRotator* Rotation);
		inline GetPlayerViewpointParams GetPlayerViewpointOriginal = nullptr;

		void GetPlayerViewpoint(void* this_, SDK::FVector* Location, SDK::FRotator* Rotation);

		inline void* PlayerControllerHooked = nullptr;

		inline Hooks::VFTHook* Hook = nullptr;
	}

	namespace GetViewpoint {
		using GetViewpointParams = void(*)(void* this_, SDK::FMinimalViewInfo* OutViewInfo, SDK::EStereoscopicPass StereoPass);
		inline GetViewpointParams GetViewpointOriginal = nullptr;

		void GetViewpoint(void* this_, SDK::FMinimalViewInfo* OutViewInfo, SDK::EStereoscopicPass StereoPass);

		inline void* LocalPlayerHooked = nullptr;

		inline Hooks::VFTHook* Hook = nullptr;
	}



	// Functions

	void Init();
	void Tick();
}