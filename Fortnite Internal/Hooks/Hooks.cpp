#include "Hooks.h"

#include "../Game/SDK/SDK.h"
#include "../Game/SDK/Classes/Engine_Classes.h"
#include "../Game/SDK/Classes/FortniteGame_Classes.h"

#include "../External-Libs/LazyImporter.h"

#include "../Utilities/Logger.h"
#include "../Utilities/Error.h"

#include "../Configs/Config.h"
#include "../External-Libs/minhook/include/MinHook.h"

template <typename T>
Hooks::VFTHook::VFTHook(void** VFT, const uintptr_t VFTIndex, T& Original, void* Hook) {
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Create VFTHook called")));

	DWORD OldProtection = 0;

	BOOL ProtectSucceeded = LI_FN(VirtualProtect).safe()(&VFT[VFTIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtection);
	if (ProtectSucceeded == FALSE) {
		DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to hook VFTIndex: ")) + std::to_string(VFTIndex));
		return;
	}

	Original = reinterpret_cast<T>(VFT[VFTIndex]);
	VFT[VFTIndex] = Hook;

	ProtectSucceeded = LI_FN(VirtualProtect).safe()(&VFT[VFTIndex], sizeof(void*), OldProtection, &OldProtection);
	if (ProtectSucceeded == FALSE) {
		DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to revert protection on destructor VFTIndex: ")) + std::to_string(VFTIndex));
		return;
	}

	this->VFT = VFT;
	this->VFTIndex = VFTIndex;
	this->Original = Original;

	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Hooked VFTIndex: ")) + std::to_string(VFTIndex));
}
Hooks::VFTHook::~VFTHook() {
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Destroy VFTHook called")));

	if (VFT == nullptr || Original == nullptr) {
		DEBUG_LOG(LOG_INFO, std::string(skCrypt("Failed to destroy hook! VFT or Original is nullptr")));
		return;
	}

	DWORD OldProtection = 0;

	BOOL ProtectSucceeded = LI_FN(VirtualProtect).safe()(VFT[VFTIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtection);
	if (ProtectSucceeded == FALSE) {
		DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to revert protection on destructor VFTIndex: ")) + std::to_string(VFTIndex));
		return;
	}

	VFT[VFTIndex] = Original;

	ProtectSucceeded = LI_FN(VirtualProtect).safe()(VFT[VFTIndex], sizeof(void*), OldProtection, &OldProtection);
	if (ProtectSucceeded == FALSE) {
		DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to revert protection on destructor VFTIndex: ")) + std::to_string(VFTIndex));
		return;
	}

	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Unhooked VFTIndex: ")) + std::to_string(VFTIndex));
}

void Hooks::Init() {
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Initializing Hooks...")));
	
	MH_STATUS InitStats = MH_Initialize();
	if (InitStats != MH_OK && InitStats != MH_ERROR_ALREADY_INITIALIZED) {
		THROW_ERROR(std::string(skCrypt("Failed to init MinHook!")), true);
	}

	if (SDK::Cached::VFT::DrawTransition && SDK::IsValidPointer(SDK::GetEngine()->GameViewport())) {
		DrawTransition::Hook = new Hooks::VFTHook(
			SDK::GetEngine()->GameViewport()->Vft,
			SDK::Cached::VFT::DrawTransition,
			Hooks::DrawTransition::DrawTransitionOriginal,
			Hooks::DrawTransition::DrawTransition);
	}
	else {
		THROW_ERROR(std::string(skCrypt("Failed to hook DrawTransition!")), true);
	}

	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Hooks Initialized!")));
}
void Hooks::Tick() {
	if (Config::Aimbot::SilentAim && SDK::Cached::VFT::GetPlayerViewpoint && SDK::Cached::VFT::GetViewpoint) {
		SDK::APlayerController* PlayerController = SDK::GetLocalController();
		if ((Hooks::GetPlayerViewpoint::Hook == nullptr || (Hooks::GetPlayerViewpoint::PlayerControllerHooked != PlayerController && PlayerController))
			&& PlayerController->IsA(SDK::AFortPlayerController::StaticClass()) && SDK::GetLocalPawn()) {
			if (Hooks::GetPlayerViewpoint::Hook) delete Hooks::GetPlayerViewpoint::Hook;

			Hooks::GetPlayerViewpoint::Hook = new Hooks::VFTHook(
				*(void***)(PlayerController),
				SDK::Cached::VFT::GetPlayerViewpoint,
				Hooks::GetPlayerViewpoint::GetPlayerViewpointOriginal,
				Hooks::GetPlayerViewpoint::GetPlayerViewpoint);

			Hooks::GetPlayerViewpoint::PlayerControllerHooked = PlayerController;
		}

		SDK::ULocalPlayer* LocalPlayer = SDK::GetLocalPlayer();
		if ((Hooks::GetViewpoint::Hook == nullptr || (Hooks::GetViewpoint::LocalPlayerHooked != LocalPlayer && LocalPlayer))
			&& LocalPlayer->IsA(SDK::UFortLocalPlayer::StaticClass()) && SDK::GetLocalPawn()) {
			if (Hooks::GetViewpoint::Hook) delete Hooks::GetViewpoint::Hook;

			Hooks::GetViewpoint::Hook = new Hooks::VFTHook(
				*(void***)(LocalPlayer),
				SDK::Cached::VFT::GetViewpoint,
				Hooks::GetViewpoint::GetViewpointOriginal,
				Hooks::GetViewpoint::GetViewpoint);

			Hooks::GetViewpoint::LocalPlayerHooked = LocalPlayer;
		}
	}
	else {
		if (Hooks::GetPlayerViewpoint::Hook) {
			delete Hooks::GetPlayerViewpoint::Hook;
			Hooks::GetPlayerViewpoint::Hook = nullptr;
			Hooks::GetPlayerViewpoint::PlayerControllerHooked = nullptr;
		}

		if (Hooks::GetViewpoint::Hook) {
			delete Hooks::GetViewpoint::Hook;
			Hooks::GetViewpoint::Hook = nullptr;
			Hooks::GetViewpoint::LocalPlayerHooked = nullptr;
		}
	}

	if (SDK::Cached::Functions::CalculateShot) {
		if (Config::Aimbot::BulletTP && Hooks::CalculateShot::Hooked == false) {
			Hooks::CalculateShot::Hooked = true;

			MH_STATUS CreateCalculateShotHook = MH_CreateHook((void*)(SDK::Cached::Functions::CalculateShot + SDK::GetBaseAddress()), &Hooks::CalculateShot::CalculateShot, (void**)&Hooks::CalculateShot::CalculateShotOriginal);
			if (CreateCalculateShotHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to hook CalculateShot! Create Status: ")) + std::to_string(CreateCalculateShotHook));
			}

			MH_STATUS EnableCalculateShotHook = MH_EnableHook((void*)(SDK::Cached::Functions::CalculateShot + SDK::GetBaseAddress()));
			if (EnableCalculateShotHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to hook CalculateShot! Enable Status: ")) + std::to_string(EnableCalculateShotHook));
			}

			DEBUG_LOG(LOG_INFO, std::string(skCrypt("Hooked CalculateShot!")));
		}
		else if (Config::Aimbot::BulletTP == false && Hooks::CalculateShot::Hooked) {
			Hooks::CalculateShot::Hooked = false;

			MH_STATUS DisableCalculateShotHook = MH_DisableHook((void*)(SDK::Cached::Functions::CalculateShot + SDK::GetBaseAddress()));
			if (DisableCalculateShotHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to unhook CalculateShot! Disable Status: ")) + std::to_string(DisableCalculateShotHook));
			}

			MH_STATUS RemoveCalculateShotHook = MH_RemoveHook((void*)(SDK::Cached::Functions::CalculateShot + SDK::GetBaseAddress()));
			if (RemoveCalculateShotHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to unhook CalculateShot! Remove Status: ")) + std::to_string(RemoveCalculateShotHook));
			}

			Hooks::CalculateShot::CalculateShotOriginal = nullptr;

			DEBUG_LOG(LOG_INFO, std::string(skCrypt("Unhooked CalculateShot!")));
		}
	}
	if (SDK::Cached::Functions::RaycastMulti) {
		if (Config::Aimbot::BulletTPV2 && Hooks::RaycastMulti::Hooked == false) {
			Hooks::RaycastMulti::Hooked = true;

			MH_STATUS CreateRaycastMultiHook = MH_CreateHook((void*)(SDK::Cached::Functions::RaycastMulti + SDK::GetBaseAddress()), &Hooks::RaycastMulti::RaycastMulti, (void**)&Hooks::RaycastMulti::RaycastMultiOriginal);
			if (CreateRaycastMultiHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to hook RaycastMulti! Create Status: ")) + std::to_string(CreateRaycastMultiHook));
			}

			MH_STATUS EnableRaycastMultiHook = MH_EnableHook((void*)(SDK::Cached::Functions::RaycastMulti + SDK::GetBaseAddress()));
			if (EnableRaycastMultiHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to hook RaycastMulti! Enable Status: ")) + std::to_string(EnableRaycastMultiHook));
			}

			DEBUG_LOG(LOG_INFO, std::string(skCrypt("Hooked RaycastMulti!")));
		}
		else if (Config::Aimbot::BulletTPV2 == false && Hooks::RaycastMulti::Hooked) {
			Hooks::RaycastMulti::Hooked = false;

			MH_STATUS DisableRaycastMultiHook = MH_DisableHook((void*)(SDK::Cached::Functions::RaycastMulti + SDK::GetBaseAddress()));
			if (DisableRaycastMultiHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to unhook RaycastMulti! Disable Status: ")) + std::to_string(DisableRaycastMultiHook));
			}

			MH_STATUS RemoveRaycastMultiHook = MH_RemoveHook((void*)(SDK::Cached::Functions::RaycastMulti + SDK::GetBaseAddress()));
			if (RemoveRaycastMultiHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to unhook RaycastMulti! Remove Status: ")) + std::to_string(RemoveRaycastMultiHook));
			}

			Hooks::RaycastMulti::RaycastMultiOriginal = nullptr;

			DEBUG_LOG(LOG_INFO, std::string(skCrypt("Unhooked RaycastMulti!")));
		}
	}
	if (SDK::Cached::Functions::EditSelectRelease) {
		if (Config::Exploits::Player::EditOnRelease && Hooks::EditSelectRelease::Hooked == false) {
			Hooks::EditSelectRelease::Hooked = true;

			MH_STATUS CreateEditSelectReleaseHook = MH_CreateHook((void*)(SDK::Cached::Functions::EditSelectRelease + SDK::GetBaseAddress()), &Hooks::EditSelectRelease::EditSelectRelease, (void**)&Hooks::EditSelectRelease::EditSelectReleaseOriginal);
			if (CreateEditSelectReleaseHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to hook EditSelectRelease! Create Status: ")) + std::to_string(CreateEditSelectReleaseHook));
			}

			MH_STATUS EnableEditSelectReleaseHook = MH_EnableHook((void*)(SDK::Cached::Functions::EditSelectRelease + SDK::GetBaseAddress()));
			if (EnableEditSelectReleaseHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to hook EditSelectRelease! Enable Status: ")) + std::to_string(EnableEditSelectReleaseHook));
			}

			DEBUG_LOG(LOG_INFO, std::string(skCrypt("Hooked EditSelectRelease!")));
		}
		else if (Config::Exploits::Player::EditOnRelease == false && Hooks::EditSelectRelease::Hooked) {
			Hooks::EditSelectRelease::Hooked = false;

			MH_STATUS DisableEditSelectReleaseHook = MH_DisableHook((void*)(SDK::Cached::Functions::EditSelectRelease + SDK::GetBaseAddress()));
			if (DisableEditSelectReleaseHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to unhook EditSelectRelease! Disable Status: ")) + std::to_string(DisableEditSelectReleaseHook));
			}

			MH_STATUS RemoveEditSelectReleaseHook = MH_RemoveHook((void*)(SDK::Cached::Functions::EditSelectRelease + SDK::GetBaseAddress()));
			if (RemoveEditSelectReleaseHook != MH_OK) {
				DEBUG_LOG(LOG_ERROR, std::string(skCrypt("Failed to unhook EditSelectRelease! Remove Status: ")) + std::to_string(RemoveEditSelectReleaseHook));
			}

			Hooks::EditSelectRelease::EditSelectReleaseOriginal = nullptr;

			DEBUG_LOG(LOG_INFO, std::string(skCrypt("Unhooked EditSelectRelease!")));
		}
	}
}