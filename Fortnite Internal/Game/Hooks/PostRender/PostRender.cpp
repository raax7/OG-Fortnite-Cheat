#include "../Hooks.h"
#include "../../Actors/Actors.h"
#include "Drawing.h"
#include "Menu.h"

void Hooks::PostRender::PostRender(uintptr_t this_, uintptr_t Canvas) {
	if (!Canvas) {
		return PostRender(this_, Canvas);
	}

	Drawing::TargetCanvas = reinterpret_cast<SDK::UCanvas*>(Canvas);
	Actors::FortPawn::Tick(Canvas);

	RaaxGUI::Tick(Canvas);

	return PostRenderOriginal(this_, Canvas);
}