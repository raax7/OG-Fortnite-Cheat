#include "Hooks.h"

#include "../Game/SDK/SDK.h"
#include "../Game/SDK/Classes/Engine_Classes.h"
#include "../Game/SDK/Classes/FortniteGame_Classes.h"

#include "../External-Libs/LazyImporter.h"

#include "../Utilities/Logger.h"

#include "../Configs/Config.h"

template <typename T>
Hooks::VFTHook::VFTHook(void** VFT, const uintptr_t VFTIndex, T& Original, void* Hook) {
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Create VFTHook called")));

	DWORD OldProtection{};
	LI_FN(VirtualProtect).safe()(&VFT[VFTIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtection);

	Original = reinterpret_cast<T>(VFT[VFTIndex]);
	VFT[VFTIndex] = Hook;

	LI_FN(VirtualProtect).safe()(&VFT[VFTIndex], sizeof(void*), OldProtection, &OldProtection);

	this->VFT = VFT;
	this->VFTIndex = VFTIndex;
	this->Original = Original;

	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Hooked VFTIndex: ")) + std::to_string(VFTIndex));
}
Hooks::VFTHook::~VFTHook() {
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Destroy VFTHook called")));

	if (!VFT || !Original) {
		DEBUG_LOG(LOG_INFO, std::string(skCrypt("Failed to destroy hook! VFT or Original is nullptr")));
		return;
	}

	DWORD OldProtection{};
	LI_FN(VirtualProtect).safe()(VFT[VFTIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtection);

	VFT[VFTIndex] = Original;

	LI_FN(VirtualProtect).safe()(VFT[VFTIndex], sizeof(void*), OldProtection, &OldProtection);

	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Unhooked VFTIndex: ")) + std::to_string(VFTIndex));
}

void Hooks::Init() {
	if (SDK::Cached::VFT::PostRender) {
		PostRender::Hook = new Hooks::VFTHook(
			SDK::GetEngine()->GameViewport()->Vft,
			SDK::Cached::VFT::PostRender,
			Hooks::PostRender::PostRenderOriginal,
			Hooks::PostRender::PostRender);
	}
}
void Hooks::Tick() {
	if (Config::Aimbot::SilentAim && SDK::Cached::VFT::GetPlayerViewpoint != 0x0 && SDK::Cached::VFT::GetViewpoint != 0x0) {
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
}