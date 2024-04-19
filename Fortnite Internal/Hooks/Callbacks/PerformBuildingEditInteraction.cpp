#include "../Hooks.h"

#include "../../Game/SDK/Classes/FortniteGame_Classes.h"

void Hooks::PerformBuildingEditInteraction::PerformBuildingEditInteraction(void* this_) {
	// If the building is a preview, don't edit it
	if (SDK::Cast<SDK::AFortPlayerController>(SDK::GetLocalController())->TargetedBuilding()->IsA(SDK::ABuildingPlayerPrimitivePreview::StaticClass())) {
		return;
	}

	PerformBuildingEditInteractionOriginal(this_);
}