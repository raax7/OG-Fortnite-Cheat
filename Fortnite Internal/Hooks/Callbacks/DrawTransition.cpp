#include "../Hooks.h"

#include "../../Game/Actors/Actors.h"
#include "../../Game/Game.h"

#include "../../Drawing/Drawing.h"

#include "../../Utilities/Logger.h"
#include "../../Utilities/Math.h"

#include <algorithm>

void Hooks::DrawTransition::DrawTransition(uintptr_t this_, uintptr_t Canvas) {
	if (Canvas == 0x0) {
		return DrawTransitionOriginal(this_, Canvas);
		//return spoof_call<void>(PostRenderOriginal, this_, Canvas);
	}

	Game::CurrentFrame++;
	Game::CurrentTime = std::chrono::steady_clock::now();

	Game::CurrentCanvas = Canvas;
	Game::ScreenWidth = reinterpret_cast<SDK::UCanvas*>(Canvas)->SizeX();
	Game::ScreenHeight = reinterpret_cast<SDK::UCanvas*>(Canvas)->SizeY();

	Game::ScreenCenterX = Game::ScreenWidth / 2.f;
	Game::ScreenCenterY = Game::ScreenHeight / 2.f;

	// Clamp the FOV to fix target issues on extreme FOV's. This does make it inaccurate on FOV's above 120, but this doesn't really matter
	Game::PixelsPerDegree = Game::ScreenWidth / Math::RadiansToDegrees((2 * tan(0.5f * Math::DegreesToRadians(std::clamp(Actors::MainCamera.FOV, 0.f, 120.f)))));

	Hooks::Tick();

	Actors::Tick();
	Actors::UpdateCaches();

	Features::Tick();

	Game::DrawCallback();

#ifdef _IMGUI
	Drawing::SwapBuffers();

	if (RaaxDx::Initalized == false) {
		DEBUG_LOG(LOG_INFO, std::string(skCrypt("Initiating DirectX hooks")));

		RaaxDx::Status InitStatus = RaaxDx::Init();
		DEBUG_LOG(LOG_INFO, std::string(skCrypt("RaaxDx Init Status: ")) + std::to_string((int)InitStatus));

		RaaxDx::Status HookStatus = RaaxDx::Hook();
		DEBUG_LOG(LOG_INFO, std::string(skCrypt("RaaxDx Hook Status: ")) + std::to_string((int)HookStatus));
	}
#else
	Game::MenuCallback();
#endif

	return DrawTransitionOriginal(this_, Canvas);
	//return spoof_call<void>(PostRenderOriginal, this_, Canvas);
}