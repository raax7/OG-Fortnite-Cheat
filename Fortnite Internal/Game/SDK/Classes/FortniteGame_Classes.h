#pragma once
#include "../SDK.h"
#include "FortniteGame_Structs.h"

#include "Engine_Classes.h"

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

namespace SDK {
	// Classes

	class UFortAntelopeVehicleConfigs : public UObject {
	public:
		// VALUES

		void SetBoostAccumulationRate(float Value, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostAccumulationRate == -0x1) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostAccumulationRate), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostAccumulationRate) = Value;
		}

		void SetBoostExpenseRate(float Value, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostExpenseRate == -0x1) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostExpenseRate), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostExpenseRate) = Value;
		}
	};

	class AFortAthenaVehicle : public APawn {
	public:
		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};

	class AFortAthenaJackalVehicle : public AFortAthenaVehicle {
	public:
		// VALUES

		TArray<struct FFortRechargingActionTimer> BoostTimers() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortAthenaJackalVehicle::BoostTimers == -0x1) return TArray<struct FFortRechargingActionTimer>{};
			return *(TArray<struct FFortRechargingActionTimer>*)((uintptr_t)this + SDK::Cached::Offsets::FortAthenaJackalVehicle::BoostTimers);
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};

	class AFortAthenaDoghouseVehicle : public AFortAthenaVehicle {
	public:
		// VALUES

		FFortRechargingActionTimer* BoostAction() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortAthenaDoghouseVehicle::BoostAction == -0x1) return nullptr;
			return (FFortRechargingActionTimer*)((uintptr_t)this + SDK::Cached::Offsets::FortAthenaDoghouseVehicle::BoostAction);
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};

	class AFortAthenaAntelopeVehicle : public AFortAthenaVehicle {
	public:
		// VALUES

		UFortAntelopeVehicleConfigs* FortAntelopeVehicleConfigs() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortAthenaAntelopeVehicle::FortAntelopeVehicleConfigs == -0x1) return nullptr;
			return (UFortAntelopeVehicleConfigs*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortAthenaAntelopeVehicle::FortAntelopeVehicleConfigs));
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};

	class UFortItemDefinition : public UObject {
	public:
		// VALUES

		FText DisplayName() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortItemDefinition::DisplayName == -0x1) return FText();
			return *(FText*)((uintptr_t)this + SDK::Cached::Offsets::FortItemDefinition::DisplayName);
		}

		EFortItemTier Tier() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortItemDefinition::Tier == -0x1) return EFortItemTier();
			return *(EFortItemTier*)((uintptr_t)this + SDK::Cached::Offsets::FortItemDefinition::Tier);
		}



		// CUSTOM FUNCTIONS

		FLinearColor GetRarityColor();
	};
	class UFortWeaponItemDefinition : public UFortItemDefinition {
	public:

	};
	class UFortWeaponMeleeItemDefinition : public UFortWeaponItemDefinition {
	public:
		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class FFortItemEntry : public UObject {
	public:
		// VALUES

		UFortItemDefinition* ItemDefinition() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortItemEntry::ItemDefinition == -0x1) return nullptr;
			return (UFortItemDefinition*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortItemEntry::ItemDefinition));
		}
	};
	class AFortPickup : public AActor {
	public:
		// VALUES

		FFortItemEntry* PrimaryPickupItemEntry() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortPickup::PrimaryPickupItemEntry == -0x1) return nullptr;
			return (FFortItemEntry*)((uintptr_t)this + SDK::Cached::Offsets::FortPickup::PrimaryPickupItemEntry);
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class AFortWeapon : public AActor {
	public:
		// VALUES

		UFortWeaponItemDefinition* WeaponData() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortWeapon::WeaponData == -0x1) return nullptr;
			return (UFortWeaponItemDefinition*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortWeapon::WeaponData));
		}

		float LastFireTime() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortWeapon::LastFireTime == -0x1) return 0.f;
			return *(float*)((uintptr_t)this + SDK::Cached::Offsets::FortWeapon::LastFireTime);
		}

		float LastFireTimeVerified() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortWeapon::LastFireTimeVerified == -0x1) return 0.f;
			return *(float*)((uintptr_t)this + SDK::Cached::Offsets::FortWeapon::LastFireTimeVerified);
		}

		void SetLastFireTime(float NewLastFireTime) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortWeapon::LastFireTime == -0x1) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortWeapon::LastFireTime) = NewLastFireTime;
		}

		void SetbIgnoreTryToFireSlotCooldownRestriction(bool NewValue, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortWeapon::bIgnoreTryToFireSlotCooldownRestriction == -0x1) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<bool>((bool*)((uintptr_t)this + SDK::Cached::Offsets::FortWeapon::bIgnoreTryToFireSlotCooldownRestriction), AutoRevertFeature);
			}

			*(bool*)((uintptr_t)this + SDK::Cached::Offsets::FortWeapon::bIgnoreTryToFireSlotCooldownRestriction) = NewValue;
		}



		// FUNCTIONS

		FFortBaseWeaponStats* WeaponStats();

		bool IsProjectileWeapon();

		float GetProjectileSpeed(float ChargePercent);



		// STATIC FUNCTIONS

		static UClass* StaticClass();




		// CUSTOM FUNCTIONS

		bool IsPickaxe();
	};
	class AFortWeaponRanged : public AFortWeapon {
	public:
		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class AFortPlayerState : public APlayerState {
	public:
		// VALUES

		uint8 TeamIndex() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortPlayerStateAthena::TeamIndex == -0x1) return 0;
			return *(uint8*)((uintptr_t)this + SDK::Cached::Offsets::FortPlayerStateAthena::TeamIndex);
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class AFortPlayerStateZone : public APlayerState {
	public:
		// VALUES

		AFortPlayerStateZone* SpectatingTarget() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortPlayerStateZone::SpectatingTarget == -0x1) return nullptr;
			return (AFortPlayerStateZone*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortPlayerStateZone::SpectatingTarget));
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class AFortPawn : public ACharacter {
	public:
		// VALUES

		AFortWeapon* CurrentWeapon() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Pawn::PlayerState == -0x1) return nullptr;
			return (AFortWeapon*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortPawn::CurrentWeapon));
		}

		bool IsDying() {
			if (SDK::IsValidPointer(this) == false || (SDK::Cached::Offsets::FortPawn::bIsDying == -0x1 && SDK::Cached::Masks::FortPawn::bIsDying == -0x1)) return false;

			if (SDK::Cached::Masks::FortPawn::bIsDying != -0x1) {
				uint8 BitField = *(uint8*)((uintptr_t)this + SDK::Cached::Offsets::FortPawn::bIsDying);
				return BitField & SDK::Cached::Masks::FortPawn::bIsDying;
			}
			else {
				return *(bool*)((uintptr_t)this + SDK::Cached::Offsets::FortPawn::bIsDying);
			}
		}



		// FUNCTIONS

		void ServerHandlePickup(class AFortPickup* Pickup, float InFlyTime, const struct FVector& InStartDirection, bool bPlayPickupSound);



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class AFortPlayerPawn : public AFortPawn {
	public:
		// VALUES

		FVehiclePawnState* VehicleStateLocal() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortPlayerPawn::VehicleStateLocal == -0x1) return nullptr;
			return (FVehiclePawnState*)((uintptr_t)this + SDK::Cached::Offsets::FortPlayerPawn::VehicleStateLocal);
		}

		FZiplinePawnState* ZiplineState() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortPlayerPawn::ZiplineState == -0x1) return nullptr;
			return (FZiplinePawnState*)((uintptr_t)this + SDK::Cached::Offsets::FortPlayerPawn::ZiplineState);
		}



		// CUSTOM FUNCTIONS

		AFortAthenaVehicle* GetVehicle();

		std::vector<USkeletalMeshComponentBudgeted*> GetCharacterPartSkeletalMeshComponents();
	};
	class AFortPlayerPawnAthena : public AFortPlayerPawn {
	public:
		// VALUES

		void SetbADSWhileNotOnGround(bool NewValue, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::FortPlayerPawnAthena::bADSWhileNotOnGround == -0x1) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<bool>((bool*)((uintptr_t)this + SDK::Cached::Offsets::FortPlayerPawnAthena::bADSWhileNotOnGround), AutoRevertFeature);
			}

			*(bool*)((uintptr_t)this + SDK::Cached::Offsets::FortPlayerPawnAthena::bADSWhileNotOnGround) = NewValue;
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class ABuildingActor : public AActor {
	public:
		// VALUES

		uint8 TeamIndex() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::BuildingActor::TeamIndex == -0x1) return 0;
			return *(uint8*)((uintptr_t)this + SDK::Cached::Offsets::BuildingActor::TeamIndex);
		}

		void SetTeamIndex(uint8 NewTeamIndex, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::BuildingActor::TeamIndex == -0x1) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<uint8>((uint8*)((uintptr_t)this + SDK::Cached::Offsets::BuildingActor::TeamIndex), AutoRevertFeature);
			}

			*(uint8*)((uintptr_t)this + SDK::Cached::Offsets::BuildingActor::TeamIndex) = NewTeamIndex;
		}
	};
	class AFortPlayerController : public APlayerController {
	public:
		// VALUES

		ABuildingActor* TargetedBuilding() {
			if (SDK::IsValidPointer(this) == false) return nullptr;
			return (ABuildingActor*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortPlayerController::TargetedBuilding));
		}
	
	

		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class UFortLocalPlayer : public ULocalPlayer {
	public:
		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class AFortGameStateAthena : public AGameState {
	public:
		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class ABuildingWeakSpot : public AActor {
	public:
		// VALUES

		bool bHit() {
			if (SDK::IsValidPointer(this) == false || (SDK::Cached::Offsets::BuildingWeakSpot::bHit == -0x1 && SDK::Cached::Masks::BuildingWeakSpot::bHit == -0x1)) return false;

			if (SDK::Cached::Masks::BuildingWeakSpot::bHit != -0x1) {
				uint8 BitField = *(uint8*)((uintptr_t)this + SDK::Cached::Offsets::BuildingWeakSpot::bHit);
				return BitField & SDK::Cached::Masks::BuildingWeakSpot::bHit;
			}
			else {
				return *(bool*)((uintptr_t)this + SDK::Cached::Offsets::BuildingWeakSpot::bHit);
			}
		}

		bool bFadeOut() {
			if (SDK::IsValidPointer(this) == false || (SDK::Cached::Offsets::BuildingWeakSpot::bFadeOut == -0x1 && SDK::Cached::Masks::BuildingWeakSpot::bFadeOut == -0x1)) return false;

			if (SDK::Cached::Masks::BuildingWeakSpot::bFadeOut != -0x1) {
				uint8 BitField = *(uint8*)((uintptr_t)this + SDK::Cached::Offsets::BuildingWeakSpot::bFadeOut);
				return BitField & SDK::Cached::Masks::BuildingWeakSpot::bFadeOut;
			}
			else {
				return *(bool*)((uintptr_t)this + SDK::Cached::Offsets::BuildingWeakSpot::bFadeOut);
			}
		}

		bool bActive() {
			if (SDK::IsValidPointer(this) == false || (SDK::Cached::Offsets::BuildingWeakSpot::bActive == -0x1 && SDK::Cached::Masks::BuildingWeakSpot::bActive == -0x1)) return false;

			if (SDK::Cached::Masks::BuildingWeakSpot::bActive != -0x1) {
				uint8 BitField = *(uint8*)((uintptr_t)this + SDK::Cached::Offsets::BuildingWeakSpot::bActive);
				return BitField & SDK::Cached::Masks::BuildingWeakSpot::bActive;
			}
			else {
				return *(bool*)((uintptr_t)this + SDK::Cached::Offsets::BuildingWeakSpot::bActive);
			}
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
}