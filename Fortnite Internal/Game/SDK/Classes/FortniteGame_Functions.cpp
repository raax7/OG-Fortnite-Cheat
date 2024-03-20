#include "FortniteGame_Classes.h"

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

SDK::UClass* SDK::AFortPawn::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("FortPawn")));

	return Clss;
}

SDK::AFortAthenaVehicle* SDK::AFortPawn::GetVehicle() {
	if (SDK::IsValidPointer(this) == false) return nullptr;
	return VehicleStateLocal()->GetVehicle();
}

SDK::UClass* SDK::AFortPlayerController::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("FortPlayerController")));

	return Clss;
}

SDK::UClass* SDK::UFortLocalPlayer::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("FortLocalPlayer")));

	return Clss;
}

SDK::UClass* SDK::ABuildingWeakSpot::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("BuildingWeakSpot")));

	return Clss;
}