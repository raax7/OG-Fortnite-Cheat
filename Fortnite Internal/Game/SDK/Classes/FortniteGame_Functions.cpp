#include "FortniteGame_Classes.h"

SDK::UClass* SDK::UFortWeaponMeleeItemDefinition::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(skCrypt("Class FortniteGame.FortWeaponMeleeItemDefinition").decrypt());

	return Clss;
}

SDK::UClass* SDK::AFortPickup::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(skCrypt("Class FortniteGame.FortPickup").decrypt());

	return Clss;
}

SDK::FFortBaseWeaponStats* SDK::AFortWeapon::WeaponStats() {
	if (!SDK::IsValidPointer(this)) return nullptr;
	auto VFT = *reinterpret_cast<void***>(this);

	if (VFT != nullptr && VFT[SDK::Cached::VFT::GetWeaponStats] != nullptr) {
		return reinterpret_cast<FFortBaseWeaponStats * (*)(void*)>(VFT[SDK::Cached::VFT::GetWeaponStats])(this);
	}
}

bool SDK::AFortWeapon::IsProjectileWeapon() {
	if (!SDK::IsValidPointer(this)) return false;

	struct {
		bool return_value;
	} params_IsProjectileWeapon{};

	this->ProcessEvent(SDK::Cached::Functions::FortWeapon::IsProjectileWeapon, &params_IsProjectileWeapon);

	return params_IsProjectileWeapon.return_value;
}

float SDK::AFortWeapon::GetProjectileSpeed(float ChargePercent) {
	if (!SDK::IsValidPointer(this)) return false;

	struct {
		float ChargePercent;

		float return_value;
	} params_GetProjectileSpeed{};

	this->ProcessEvent(SDK::Cached::Functions::FortWeapon::GetProjectileSpeed, &params_GetProjectileSpeed);

	return params_GetProjectileSpeed.return_value;
}

bool SDK::AFortWeapon::IsPickaxe() {
	if (!SDK::IsValidPointer(this)) return false;

	UFortWeaponItemDefinition* WeaponData = this->WeaponData();
	if (!SDK::IsValidPointer(this)) return false;

	return WeaponData->IsA(UFortWeaponMeleeItemDefinition::StaticClass());
}

SDK::UClass* SDK::AFortWeapon::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(skCrypt("Class FortniteGame.FortWeapon").decrypt());

	return Clss;
}

SDK::UClass* SDK::AFortWeaponRanged::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(skCrypt("Class FortniteGame.FortWeaponRanged").decrypt());

	return Clss;
}

SDK::UClass* SDK::AFortPawn::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(skCrypt("FortPawn").decrypt());

	return Clss;
}

SDK::UClass* SDK::ABuildingWeakSpot::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(skCrypt("BuildingWeakSpot").decrypt());

	return Clss;
}