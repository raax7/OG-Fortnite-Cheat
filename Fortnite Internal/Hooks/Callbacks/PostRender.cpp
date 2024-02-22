#include "../Hooks.h"

#include "../../Game/Actors/Actors.h"
#include "../../Game/Game.h"

void Hooks::PostRender::PostRender(uintptr_t this_, uintptr_t Canvas) {
	if (!Canvas) {
		return PostRender(this_, Canvas);
	}

	Game::CurrentFrame++;
	Game::CurrentCanvas = Canvas;
	Game::ScreenWidth = reinterpret_cast<SDK::UCanvas*>(Canvas)->SizeX();
	Game::ScreenHeight = reinterpret_cast<SDK::UCanvas*>(Canvas)->SizeY();

	Hooks::Tick();

	Actors::Tick();
	Actors::UpdateCaches();

	Game::DrawCallback();

	return PostRenderOriginal(this_, Canvas);
}