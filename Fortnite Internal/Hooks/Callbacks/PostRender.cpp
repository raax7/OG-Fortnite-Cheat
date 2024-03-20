#include "../Hooks.h"

#include "../../Game/Actors/Actors.h"
#include "../../Game/Game.h"

#include "../../Drawing/Drawing.h"

#include "../../Utilities/Logger.h"
#include "../../Utilities/Math.h"

void Hooks::PostRender::PostRender(uintptr_t this_, uintptr_t Canvas) {
	if (Canvas == NULL) {
		return PostRenderOriginal(this_, Canvas);
		//return spoof_call<void>(PostRenderOriginal, this_, Canvas);
	}

	Game::CurrentFrame++;

	Game::CurrentCanvas = Canvas;
	Game::ScreenWidth = reinterpret_cast<SDK::UCanvas*>(Canvas)->SizeX();
	Game::ScreenHeight = reinterpret_cast<SDK::UCanvas*>(Canvas)->SizeY();

	// Clamp the FOV to fix target issues on extreme FOV's. This does make it innacurate on FOV's above 120, but this doesn't really matter
	Game::PixelsPerDegree = Game::ScreenWidth / Math::RadiansToDegrees((2 * tan(0.5f * Math::DegreesToRadians(Math::Clamp(Actors::MainCamera.FOV, 0, 120)))));

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

	return PostRenderOriginal(this_, Canvas);
	//return spoof_call<void>(PostRenderOriginal, this_, Canvas);
}