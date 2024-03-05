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

	class UFortWeaponItemDefinition : public UObject {
	public:

	};
	class UFortWeaponMeleeItemDefinition : public UFortWeaponItemDefinition {
	public:
		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class UFortItemDefinition : public UObject {
	public:
		// VALUES

		FText DisplayName() {
			if (!this) return FText{};
			return *(FText*)((uintptr_t)this + SDK::Cached::Offsets::FortItemDefinition::DisplayName);
		}

		EFortItemTier Tier() {
			if (!this) return EFortItemTier{};
			return *(EFortItemTier*)((uintptr_t)this + SDK::Cached::Offsets::FortItemDefinition::Tier);
		}

		//EFortItemType Type() {
		//	if (!SDK::IsValidPointer((uintptr_t)this)) return EFortItemType{};
		//	return *(EFortItemType*)((uintptr_t)this + SDK::Cached::Offsets::FortItemDefinition::ItemType);
		//}
	};
	class FFortItemEntry : public UObject {
	public:
		// VALUES

		UFortItemDefinition* ItemDefinition() {
			if (!this) return nullptr;
			return (UFortItemDefinition*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortItemEntry::ItemDefinition));
		}
	};
	class AFortPickup : public AActor {
	public:
		// VALUES

		FFortItemEntry PrimaryPickupItemEntry() {
			if (!this) return FFortItemEntry{};
			return *(FFortItemEntry*)((uintptr_t)this + SDK::Cached::Offsets::FortPickup::PrimaryPickupItemEntry);
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class AFortWeapon : public AActor {
	public:
		// VALUES

		UFortWeaponItemDefinition* WeaponData() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (UFortWeaponItemDefinition*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortWeapon::WeaponData));
		}



		// FUNCTIONS

		FFortBaseWeaponStats* WeaponStats();



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
			if (!SDK::IsValidPointer((uintptr_t)this)) return uint8{};
			return *(uint8*)((uintptr_t)this + SDK::Cached::Offsets::FortPlayerStateAthena::TeamIndex);
		}
	};
	class AFortPawn : public APawn {
	public:
		// VALUES

		AFortPlayerState* PlayerState() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (AFortPlayerState*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Pawn::PlayerState));
		}

		AFortWeapon* CurrentWeapon() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (AFortWeapon*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortPawn::CurrentWeapon));
		}

		bool IsDying() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return false;

			if (SDK::Cached::Masks::FortPawn::bIsDying) {
				uint8 BitField = *(uint8*)((uintptr_t)this + SDK::Cached::Offsets::FortPawn::bIsDying);
				return BitField & SDK::Cached::Masks::FortPawn::bIsDying;
			}
			else {
				return *(bool*)((uintptr_t)this + SDK::Cached::Offsets::FortPawn::bIsDying);
			}
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class ABuildingWeakSpot : public AActor {
	public:
		// VALUES

		uint8 GetWeakSpotInfo() {
			if (!this) return 0;
			return *(uint8*)((uintptr_t)this + SDK::Cached::Offsets::BuildingWeakSpot::WeakSpotInfoBitField);
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
}