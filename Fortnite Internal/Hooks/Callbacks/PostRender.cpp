#include "../Hooks.h"

#include "../../Game/Actors/Actors.h"
#include "../../Game/Game.h"

#include "../../Utilities/Math.h"
#include "../../Configs/Config.h"
#include "../../Utilities/Logger.h"

void Hooks::PostRender::PostRender(uintptr_t this_, uintptr_t Canvas) {
	if (!Canvas) {
		return PostRenderOriginal(this_, Canvas);
		//return spoof_call<void>(PostRenderOriginal, this_, Canvas);
	}

#ifdef _ENGINE
	Game::CurrentFrame++;
#endif
	Game::CurrentCanvas = Canvas;
	Game::ScreenWidth = reinterpret_cast<SDK::UCanvas*>(Canvas)->SizeX();
	Game::ScreenHeight = reinterpret_cast<SDK::UCanvas*>(Canvas)->SizeY();

	// Clamp the FOV to fix target issues on extreme FOV's. This does make it innacurate on FOV's above 120, but this doesn't really matter
	Game::PixelsPerDegree = Game::ScreenWidth / Math::RadiansToDegrees((2 * tan(0.5f * Math::DegreesToRadians(Math::Clamp(Actors::MainCamera.FOV, 0, 120)))));

	Hooks::Tick();

	Actors::Tick();
	Actors::UpdateCaches();

#ifdef _ENGINE
	Game::DrawCallback();
	Game::MenuCallback();
#else
	if (RaaxDx::Initalized == false) {
		DEBUG_LOG(LOG_INFO, skCrypt("Initiating DirectX hooks").decrypt());

		if (RaaxDx::Init() == RaaxDx::Status::Success && RaaxDx::Hook() == RaaxDx::Status::Success) {
			DEBUG_LOG(LOG_INFO, skCrypt("Hooked DirectX").decrypt());
		}
		else {
			THROW_ERROR(skCrypt("Failed to hook DirectX").decrypt(), true);
		}
	}
#endif

	return PostRenderOriginal(this_, Canvas);
	//return spoof_call<void>(PostRenderOriginal, this_, Canvas);
}