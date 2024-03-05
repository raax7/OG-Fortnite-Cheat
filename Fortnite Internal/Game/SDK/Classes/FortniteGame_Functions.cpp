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
	if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
	auto VFT = *reinterpret_cast<void***>(this);

	if (VFT != nullptr && VFT[SDK::Cached::VFT::GetWeaponStats] != nullptr) {
		return reinterpret_cast<FFortBaseWeaponStats * (*)(void*)>(VFT[SDK::Cached::VFT::GetWeaponStats])(this);
	}
}

bool SDK::AFortWeapon::IsPickaxe() {
	if (!SDK::IsValidPointer((uintptr_t)this)) return false;

	UFortWeaponItemDefinition* WeaponData = this->WeaponData();
	if (!SDK::IsValidPointer((uintptr_t)this)) return false;

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