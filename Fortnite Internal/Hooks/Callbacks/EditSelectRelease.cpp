#include "../Hooks.h"

#include "../../Game/SDK/Classes/FortniteGame_Classes.h"

void Hooks::EditSelectRelease::EditSelectRelease(void* this_) {
	EditSelectReleaseOriginal(this_);

	SDK::Cast<SDK::AFortPlayerController>(SDK::GetLocalController())->CompleteBuildingEditInteraction();
}