#pragma once
#include <Windows.h>
#include "../Game/SDK/Classes/FortniteGame_classes.h"

namespace Hooks {
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
		VFTHook(void** VFT, const uintptr_t VFTIndex, T& Original, void* Hook) {
			DEBUG_LOG(skCrypt("Create VFTHook called").decrypt());

			DWORD OldProtection{};
			LI_FN(VirtualProtect, &VFT[VFTIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtection).safe();

			Original = reinterpret_cast<T>(VFT[VFTIndex]);
			VFT[VFTIndex] = Hook;

			LI_FN(VirtualProtect, &VFT[VFTIndex], sizeof(void*), OldProtection, &OldProtection).safe();

			this->VFT = VFT;
			this->VFTIndex = VFTIndex;
			this->Original = Original;
		}

		/*
		* @brief Revert the VFT hook back to the original function
		*/
		~VFTHook() {
			DEBUG_LOG(skCrypt("Destroy VFTHook called").decrypt());

			if (!VFT || !Original) {
				DEBUG_LOG(skCrypt("Failed to destroy hook! VFT or Original is nullptr").decrypt());
				return;
			}

			DWORD OldProtection{};
			LI_FN(VirtualProtect, &VFT[VFTIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtection).safe();

			VFT[VFTIndex] = Original;

			LI_FN(VirtualProtect, &VFT[VFTIndex], sizeof(void*), OldProtection, &OldProtection).safe();
		}
	};

	// TEST PROCESS EVENT
	namespace ProcessEvent {
		using ProcessEventParams = void(*)(void* this_, void* Function, void* Params);
		inline ProcessEventParams ProcessEventOriginal = nullptr;

		void ProcessEvent(void* this_, void* Function, void* Params);

		inline Hooks::VFTHook* Hook;
	}

	namespace PostRender {
		using PostRenderParams = void(*)(uintptr_t this_, uintptr_t Canvas);
		inline PostRenderParams PostRenderOriginal = nullptr;

		void PostRender(uintptr_t this_, uintptr_t Canvas);

		inline Hooks::VFTHook* Hook;
	}

	namespace GetPlayerViewpoint {
		using GetPlayerViewpointParams = void(*)(void* this_, SDK::FVector* Location, SDK::FRotator* Rotation);
		inline GetPlayerViewpointParams GetPlayerViewpointOriginal = nullptr;

		void GetPlayerViewpoint(void* this_, SDK::FVector* Location, SDK::FRotator* Rotation);

		inline SDK::APlayerController* PlayerControllerHooked = nullptr;

		inline Hooks::VFTHook* Hook;
	}

	namespace GetViewpoint {
		using GetViewpointParams = void(*)(void* this_, SDK::FMinimalViewInfo* OutViewInfo, SDK::EStereoscopicPass StereoPass);
		inline GetViewpointParams GetViewpointOriginal = nullptr;

		void GetViewpoint(void* this_, SDK::FMinimalViewInfo* OutViewInfo, SDK::EStereoscopicPass StereoPass);

		inline SDK::ULocalPlayer* LocalPlayerHooked = nullptr;

		inline Hooks::VFTHook* Hook;
	}



	namespace ReloadSpeed {
		using ReloadSpeedParams = float(*)(void*);
		inline ReloadSpeedParams ReloadSpeedOriginal = nullptr;

		float ReloadSpeed(void*);

		inline SDK::AFortWeapon* WeaponHooked = nullptr;

		inline Hooks::VFTHook* Hook;
	}

	namespace FiringRate {
		using FiringRateParams = float(*)(void*);
		inline FiringRateParams FiringRateOriginal = nullptr;

		float FiringRate(void*);

		inline SDK::AFortWeapon* WeaponHooked = nullptr;

		inline Hooks::VFTHook* Hook;
	}

	namespace SilentGun {
		using SilentGunParams = void(*)(void*);
		inline SilentGunParams SilentGunOriginal1 = nullptr;
		inline SilentGunParams SilentGunOriginal2 = nullptr;
		inline SilentGunParams SilentGunOriginal3 = nullptr;

		void SilentGun1(void*);
		void SilentGun2(void*);
		void SilentGun3(void*);

		inline SDK::AFortWeapon* WeaponHooked1 = nullptr;
		inline SDK::AFortWeapon* WeaponHooked2 = nullptr;
		inline SDK::AFortWeapon* WeaponHooked3 = nullptr;

		inline Hooks::VFTHook* Hook1;
		inline Hooks::VFTHook* Hook2;
		inline Hooks::VFTHook* Hook3;
	}

	void Init();
	void Tick();
}