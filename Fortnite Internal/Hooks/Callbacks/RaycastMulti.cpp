#include "../Hooks.h"

#include "../../Game/Features/Aimbot/Aimbot.h"

bool Hooks::RaycastMulti::RaycastMulti(SDK::UWorld* World, SDK::TArray<SDK::FHitResult>& OutHits, const SDK::FVector Start, const SDK::FVector End, SDK::ECollisionChannel TraceChannel, const struct FCollisionQueryParams& Params, const struct FCollisionResponseParams& ResponseParams, const struct FCollisionObjectQueryParams& ObjectParams) {
	bool Original = RaycastMultiOriginal(World, OutHits, Start, End, TraceChannel, Params, ResponseParams, ObjectParams);

	Features::Aimbot::RaycastMultiCallback(World, OutHits, TraceChannel);

	return Original;
}