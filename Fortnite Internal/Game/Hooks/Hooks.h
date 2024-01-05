#pragma once
#include <Windows.h>
#include "../SDK/Classes/Engine_classes.h"

namespace Hooks {
	namespace PostRender {
		using PostRenderParams = void(*)(uintptr_t this_, uintptr_t Canvas);
		inline PostRenderParams PostRenderOriginal = nullptr;

		void PostRender(uintptr_t this_, uintptr_t Canvas);

		inline bool Hooked = false;
	}

	inline void Init();
}

inline void Hooks::Init() {
	// post render hook
	void** ViewPortClient_VTable = *(void***)(SDK::GetEngine()->GameViewport());

	DWORD OldProtection;
	LI_FN(VirtualProtect, &ViewPortClient_VTable[SDK::Cached::VFT::PostRender], 8, PAGE_EXECUTE_READWRITE, &OldProtection);

	Hooks::PostRender::PostRenderOriginal = decltype(Hooks::PostRender::PostRenderOriginal)(ViewPortClient_VTable[SDK::Cached::VFT::PostRender]);
	ViewPortClient_VTable[SDK::Cached::VFT::PostRender] = &Hooks::PostRender::PostRender;

	LI_FN(VirtualProtect, &ViewPortClient_VTable[SDK::Cached::VFT::PostRender], 8, OldProtection, &OldProtection);
}