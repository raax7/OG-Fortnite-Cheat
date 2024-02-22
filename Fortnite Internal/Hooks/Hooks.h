#pragma once
#include <Windows.h>

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

	namespace PostRender {
		using PostRenderParams = void(*)(uintptr_t this_, uintptr_t Canvas);
		inline PostRenderParams PostRenderOriginal = nullptr;

		void PostRender(uintptr_t this_, uintptr_t Canvas);

		inline Hooks::VFTHook* Hook = nullptr;
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