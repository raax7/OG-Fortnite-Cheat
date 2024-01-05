#include "../Actors.h"
#include "../../SDK/SDK.h"
#include "../../SDK/Classes/Engine_classes.h"

void Actors::FortWeapon::Tick(uintptr_t Canvas_) {
	if (!SDK::IsValidPointer(Canvas_)) return;
	SDK::UCanvas* Canvas = reinterpret_cast<SDK::UCanvas*>(Canvas_);
}