#ifndef FORTNITEGAME_CLASSES_H
#define FORTNITEGAME_CLASSES_H

#include "../SDK.h"
#include "../../../Configs/Config.h"
#include "Engine_classes.h"
#include "Basic.h"

namespace SDK {
	class UFortItemDefinition : public UObject {
	public:
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
		UFortItemDefinition* ItemDefinition() {
			//if (!this) return nullptr;
			return (UFortItemDefinition*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::FortItemEntry::ItemDefinition));
		}
	};

	class AFortPickup : public AActor {
	public:
		FFortItemEntry PrimaryPickupItemEntry() {
			if (!this) return FFortItemEntry{};
			return *(FFortItemEntry*)((uintptr_t)this + SDK::Cached::Offsets::FortPickup::PrimaryPickupItemEntry);
		}



		static UClass* StaticClass()
		{
			static class UClass* Clss = nullptr;

			if (!Clss)
				Clss = SDK::UObject::FindClass("Class FortniteGame.FortPickup");

			return Clss;
		}
	};

	class AFortPlayerState : public AActor {
	public:
		//FString GetPlayerName() {
		//	if (!SDK::IsValidPointer((uintptr_t)this)) return FString{};
		//
		//	struct {
		//		FString return_value;
		//	} params{};
		//
		//	this->ProcessEvent(SDK::Cached::Functions::GetPlayerName, &params);
		//
		//	return params.return_value;
		//}
		//uint8 TeamIndex() {
		//	if (!SDK::IsValidPointer((uintptr_t)this)) return uint8{};
		//	return *(uint8*)((uintptr_t)this + SDK::Cached::Offsets::TeamIndex);
		//}
	};

	class AFortPawn : public APawn {
	public:
		AFortPlayerState* PlayerState() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (AFortPlayerState*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Pawn::PlayerState));
		}
		//AFortWeapon* CurrentWeapon() {
		//	if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
		//	return (AFortWeapon*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::CurrentWeapon));
		//}



		static UClass* StaticClass()
		{
			static class UClass* Clss = nullptr;

			if (!Clss)
				Clss = UObject::FindClassFast(skCrypt("FortPawn").decrypt());

			return Clss;
		}
	};
}

#endif