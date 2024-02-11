#pragma once
#include "../SDK.h"
#include "Engine_classes.h"
#include "FortniteGame_structs.h"
#include "Basic.h"

namespace SDK {
	class UFortWeaponItemDefinition : public UObject {
	public:

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

		static UClass* StaticClass()
		{
			static class UClass* Clss = nullptr;

			if (!Clss)
				Clss = SDK::UObject::FindClass(skCrypt("Class FortniteGame.FortPickup").decrypt());

			return Clss;
		}
	};

	class AFortWeapon : public AActor {
	public:
		// VALUES

		UFortWeaponItemDefinition* WeaponData() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (UFortWeaponItemDefinition*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortWeapon::WeaponData));
		}

		bool IsPickaxe() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return false;
			// add shit here
		}



		// FUNCTIONS

		FFortBaseWeaponStats* WeaponStats() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			auto VFT = *reinterpret_cast<void***>(this);

			// The VFT index has only been 0xD0 across all versions ive checked, so no need to auto get
			if (VFT != nullptr && VFT[0xD0] != nullptr) {
				return reinterpret_cast<FFortBaseWeaponStats*(*)(void*)>(VFT[0xD0])(this);
			}
		}
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



		// STATIC FUNCTIONS

		static UClass* StaticClass()
		{
			static class UClass* Clss = nullptr;

			if (!Clss)
				Clss = UObject::FindClassFast(skCrypt("FortPawn").decrypt());

			return Clss;
		}
	};

	class ABuildingWeakSpot : public AActor {
	public:
		// VALUES

		uint8_t GetWeakSpotInfo() {
			if (!this) return 0;
			return *(uint8_t*)((uintptr_t)this + SDK::Cached::Offsets::BuildingWeakSpot::WeakSpotInfoBitField);
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass()
		{
			static class UClass* Clss = nullptr;

			if (!Clss)
				Clss = UObject::FindClassFast(skCrypt("BuildingWeakSpot").decrypt());

			return Clss;
		}
	};
}