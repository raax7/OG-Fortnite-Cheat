#include "FortniteGame_Classes.h"

SDK::UClass* SDK::AFortPickup::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = SDK::UObject::FindClass(skCrypt("Class FortniteGame.FortPickup").decrypt());

	return Clss;
}

SDK::FFortBaseWeaponStats* SDK::AFortWeapon::WeaponStats() {
	if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
	auto VFT = *reinterpret_cast<void***>(this);

	// The VFT index has only been 0xD0 across all versions ive checked, so no need to get dynamically
	if (VFT != nullptr && VFT[0xD0] != nullptr) {
		return reinterpret_cast<FFortBaseWeaponStats * (*)(void*)>(VFT[0xD0])(this);
	}
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