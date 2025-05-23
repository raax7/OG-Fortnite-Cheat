#include "FortniteGame_Classes.h"

SDK::UClass* SDK::AFortAthenaVehicle::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(std::string(skCrypt("Class FortniteGame.FortAthenaVehicle")));

	return Clss;
}

SDK::UClass* SDK::AFortAthenaAntelopeVehicle::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(std::string(skCrypt("Class FortniteGame.FortAthenaAntelopeVehicle")));

	return Clss;
}

SDK::UClass* SDK::AFortAthenaJackalVehicle::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(std::string(skCrypt("Class FortniteGame.FortAthenaJackalVehicle")));

	return Clss;
}

SDK::UClass* SDK::AFortAthenaDoghouseVehicle::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(std::string(skCrypt("Class FortniteGame.FortAthenaDoghouseVehicle")));

	return Clss;
}

SDK::FLinearColor SDK::UFortItemDefinition::GetRarityColor() {
	if (SDK::IsValidPointer(this) == false) return FLinearColor(1.f, 1.f, 1.f, 1.f);

	EFortItemTier Tier = this->Tier();

	switch (Tier) {
		// Common
	case SDK::EFortItemTier::I:
		return SDK::FLinearColor(0.74f, 0.74f, 0.71f, 1.0f);

		// Uncommon
	case SDK::EFortItemTier::II:
		return SDK::FLinearColor(0.12f, 0.87f, 0.11f, 1.0f);

		// Rare
	case SDK::EFortItemTier::III:
		return SDK::FLinearColor(0.29f, 0.33f, 0.95f, 1.0f);

		// Epic
	case SDK::EFortItemTier::IV:
		return SDK::FLinearColor(0.65f, 0.27f, 0.82f, 1.0f);

		// Legendary
	case SDK::EFortItemTier::V:
		return SDK::FLinearColor(0.95f, 0.40f, 0.07f, 1.0f);

		// Mythic
	case SDK::EFortItemTier::VI:
		return SDK::FLinearColor(0.98f, 0.85f, 0.29f, 1.0f);

		// Transcendent
	case SDK::EFortItemTier::VII:
		return SDK::FLinearColor(0.47f, 1.0f, 0.96f, 1.0f);

		// Default
	default:
		return SDK::FLinearColor(0.74f, 0.74f, 0.71f, 1.0f);
	}

	return SDK::FLinearColor(1.f, 1.f, 1.f, 1.f);
}

SDK::UClass* SDK::UFortWeaponMeleeItemDefinition::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(std::string(skCrypt("Class FortniteGame.FortWeaponMeleeItemDefinition")));

	return Clss;
}

void SDK::AFortPawn::ServerHandlePickup(class AFortPickup* Pickup, float InFlyTime, const struct FVector& InStartDirection, bool bPlayPickupSound) {
	if (SDK::IsValidPointer(this) == false) return;

	struct {
		class AFortPickup* Pickup;
		float InFlyTime;
		struct FVector InStartDirection;
		bool bPlayPickupSound;
		uint8 Pad_2B6C[0x7];
	} params_ServerHandlePickup{};

	params_ServerHandlePickup.Pickup = Pickup;
	params_ServerHandlePickup.InFlyTime = InFlyTime;
	params_ServerHandlePickup.InStartDirection = InStartDirection;
	params_ServerHandlePickup.bPlayPickupSound = bPlayPickupSound;

	this->ProcessEvent(SDK::Cached::Functions::FortPlayerPawn::ServerHandlePickup, &params_ServerHandlePickup);
}

SDK::UClass* SDK::AB_Pickups_Parent_C::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast("B_Pickups_Parent_C");

	return Clss;
}

SDK::UClass* SDK::AFortPickup::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(std::string(skCrypt("Class FortniteGame.FortPickup")));

	return Clss;
}

SDK::FFortBaseWeaponStats* SDK::AFortWeapon::WeaponStats() {
	if (SDK::IsValidPointer(this) == false) return nullptr;
	auto VFT = *reinterpret_cast<void***>(this);

	if (VFT != nullptr && VFT[SDK::Cached::VFT::GetWeaponStats] != nullptr) {
		return reinterpret_cast<FFortBaseWeaponStats*(*)(void*)>(VFT[SDK::Cached::VFT::GetWeaponStats])(this);
	}
}

bool SDK::AFortWeapon::IsProjectileWeapon() {
	if (SDK::IsValidPointer(this) == false) return false;

	struct {
		bool return_value;
	} params_IsProjectileWeapon{};

	this->ProcessEvent(SDK::Cached::Functions::FortWeapon::IsProjectileWeapon, &params_IsProjectileWeapon);

	return params_IsProjectileWeapon.return_value;
}

float SDK::AFortWeapon::GetProjectileSpeed(float ChargePercent) {
	if (SDK::IsValidPointer(this) == false) return false;

	struct {
		float ChargePercent;

		float return_value;
	} params_GetProjectileSpeed{};

	this->ProcessEvent(SDK::Cached::Functions::FortWeapon::GetProjectileSpeed, &params_GetProjectileSpeed);

	return params_GetProjectileSpeed.return_value;
}

int32 SDK::AFortWeapon::GetBulletsPerClip() {
	if (SDK::IsValidPointer(this) == false) return 0;

	struct {
		int32 return_value;
	} params_GetBulletsPerClip{};

	this->ProcessEvent(SDK::Cached::Functions::FortWeapon::GetBulletsPerClip, &params_GetBulletsPerClip);

	return params_GetBulletsPerClip.return_value;
}

bool SDK::AFortWeapon::IsPickaxe() {
	if (SDK::IsValidPointer(this) == false) return false;

	UFortWeaponItemDefinition* WeaponData = this->WeaponData();
	if (SDK::IsValidPointer(this) == false) return false;

	return WeaponData->IsA(UFortWeaponMeleeItemDefinition::StaticClass());
}

SDK::UClass* SDK::AFortWeapon::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(std::string(skCrypt("Class FortniteGame.FortWeapon")));

	return Clss;
}

SDK::UClass* SDK::AFortWeaponRanged::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(std::string(skCrypt("Class FortniteGame.FortWeaponRanged")));

	return Clss;
}

SDK::UClass* SDK::AFortPlayerState::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(std::string(skCrypt("Class FortniteGame.FortPlayerState")));

	return Clss;
}

SDK::UClass* SDK::AFortPlayerStateZone::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(std::string(skCrypt("Class FortniteGame.FortPlayerStateZone")));

	return Clss;
}

SDK::UClass* SDK::AFortPawn::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("FortPawn")));

	return Clss;
}

SDK::AFortAthenaVehicle* SDK::AFortPlayerPawn::GetVehicle() {
	if (SDK::IsValidPointer(this) == false) return nullptr;
	return VehicleStateLocal()->Vehicle();
}

std::vector<SDK::USkeletalMeshComponentBudgeted*> SDK::AFortPlayerPawn::GetCharacterPartSkeletalMeshComponents() {
	std::vector<SDK::USkeletalMeshComponentBudgeted*> Components{};

	if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortPlayerPawn::CharacterPartSkeletalMeshComponents == -0x1) return Components;

	// There should never be more than 8 character parts
	for (int i = 0; i < 8; i++) {
		USkeletalMeshComponentBudgeted* Mesh = *(USkeletalMeshComponentBudgeted**)((uintptr_t)this + SDK::Cached::Offsets::FortPlayerPawn::CharacterPartSkeletalMeshComponents + (i * 8));
		if (SDK::IsValidPointer(Mesh) == false || Mesh->IsA(USkeletalMeshComponentBudgeted::StaticClass()) == false) continue;

		Components.push_back(Mesh);
	}

	return Components;
}

SDK::UClass* SDK::AFortPlayerPawnAthena::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("FortPlayerPawnAthena")));

	return Clss;
}

SDK::UClass* SDK::AFortPlayerController::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("FortPlayerController")));

	return Clss;
}

void SDK::AFortPlayerController::Fire() {
	if (SDK::IsValidPointer(this) == false || SDK::Cached::Functions::Fire == 0x0) return;

	if (Fire1 == nullptr) {
		Fire1 = reinterpret_cast<FireParams>(SDK::GetBaseAddress() + SDK::Cached::Functions::Fire);
	}

	Fire1(this);
}

void SDK::AFortPlayerController::CompleteBuildingEditInteraction() {
	if (SDK::IsValidPointer(this) == false || SDK::Cached::Functions::CompleteBuildingEditInteraction == 0x0) return;

	if (CompleteBuildingEditInteraction1 == nullptr) {
		CompleteBuildingEditInteraction1 = reinterpret_cast<CompleteBuildingEditInteractionParams>(SDK::GetBaseAddress() + SDK::Cached::Functions::CompleteBuildingEditInteraction);
	}

	CompleteBuildingEditInteraction1(this);
}

SDK::UClass* SDK::UFortLocalPlayer::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("FortLocalPlayer")));

	return Clss;
}

SDK::UClass* SDK::AFortGameStateAthena::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("FortGameStateAthena")));

	return Clss;
}

SDK::UClass* SDK::ABuildingPlayerPrimitivePreview::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("BuildingPlayerPrimitivePreview")));

	return Clss;
}

SDK::UClass* SDK::ABuildingWeakSpot::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("BuildingWeakSpot")));

	return Clss;
}