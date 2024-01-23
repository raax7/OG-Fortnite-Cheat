#ifndef HOOKS_H
#define HOOKS_H

#include <Windows.h>
#include "../SDK/Classes/Engine_classes.h"

namespace Hooks {
	class Hook {
	private:
		const char* FunctionName;
		void* VFT;
		uintptr_t VFTIndex;
		void* Original;
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
		Hook(const char* FunctionName, void* VFT, const uintptr_t VFTIndex, T& Original, void* Hook) {
			DEBUG_LOG(skCrypt("Hooking - ").decrypt() + std::string(FunctionName));

			DWORD OldProtection{};
			VirtualProtect(reinterpret_cast<void**>(VFT)[VFTIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtection);

			Original = reinterpret_cast<T>(reinterpret_cast<void**>(VFT)[VFTIndex]);
			reinterpret_cast<void**>(VFT)[VFTIndex] = Hook;

			VirtualProtect(reinterpret_cast<void**>(VFT)[VFTIndex], sizeof(void*), OldProtection, &OldProtection);

			this->FunctionName = FunctionName;
			this->VFT = const_cast<void*>(VFT);
			this->VFTIndex = VFTIndex;
			this->Original = Original;
		}

		/*
		* @brief Revert the hook back to the original function
		*/
		~Hook() {
			DEBUG_LOG(skCrypt("Unhooking - ").decrypt() + std::string(FunctionName));

			if (!VFT || !Original) {
				DEBUG_LOG(skCrypt("Failed to revert hook! VFT or Original is nullptr. (").decrypt() + std::string(FunctionName) + skCrypt(")").decrypt());
				return;
			}

			DWORD OldProtection{};
			LI_FN(VirtualProtect, &reinterpret_cast<void**>(VFT)[VFTIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtection).safe();

			reinterpret_cast<void**>(VFT)[VFTIndex] = Original;

			LI_FN(VirtualProtect, &reinterpret_cast<void**>(VFT)[VFTIndex], sizeof(void*), OldProtection, &OldProtection).safe();

			VFT = nullptr;
			VFTIndex = 0;
			Original = nullptr;
		}
	};

	namespace PostRender {
		using PostRenderParams = void(*)(uintptr_t this_, uintptr_t Canvas);
		inline PostRenderParams PostRenderOriginal = nullptr;

		void PostRender(uintptr_t this_, uintptr_t Canvas);

		inline Hooks::Hook* Hook;
	}

	inline void Init();
}

inline void Hooks::Init() {
	PostRender::Hook = new Hooks::Hook(
		skCrypt("PostRender").decrypt(),
		*(void***)(SDK::GetEngine()->GameViewport()),
		SDK::Cached::VFT::PostRender,
		Hooks::PostRender::PostRenderOriginal,
		Hooks::PostRender::PostRender);
}

#endif