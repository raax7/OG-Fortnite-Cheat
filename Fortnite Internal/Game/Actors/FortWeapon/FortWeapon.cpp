#include "../Actors.h"
#include "../../SDK/SDK.h"
#include "../../SDK/Classes/Engine_classes.h"
#include "../../SDK/Classes/FortniteGame_classes.h"
#include "../../Hooks/Hooks.h"

using paramss = void(*)(uintptr_t this_);
inline paramss hookedOriginal = nullptr;

void hooked(uintptr_t this_) {
	std::string test = "CALLED! - ";

	DEBUG_LOG(test);
	return;
}

void Actors::FortWeapon::Tick(uintptr_t Canvas_) {
	if (!SDK::IsValidPointer(Canvas_)) return;
	SDK::UCanvas* Canvas = reinterpret_cast<SDK::UCanvas*>(Canvas_);

	// Player Cache (to avoid calling GetAllActorsOfClass every tick)
	static SDK::TArray<SDK::AActor*> CachedWeapons;
	{
		auto currentTime = std::chrono::steady_clock::now();
		double elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - lastTime).count();

		if (elapsedTime >= intervalSeconds) {
			lastTime = currentTime;

			CachedWeapons = SDK::UGameplayStatics::StaticClass()->GetAllActorsOfClass(SDK::GetWorld(), SDK::AFortPickup::StaticClass());
		}
	}

	for (int i = 0; i < CachedWeapons.Num(); i++) {
		if (!CachedWeapons.IsValidIndex(i)) continue;

		SDK::AActor* Actor = CachedWeapons[i];												if (!Actor) continue;

		SDK::FVector RootPosition = Actor->GetRootComponent()->GetPosition();
		if (RootPosition.Distance(SDK::GetLocalController()->AcknowledgedPawn()->GetRootComponent()->GetPosition()) / 100 >= Config::Visuals::Weapons::WeaponMaxDistance) {
			continue;
		}

		SDK::FVector2D Project = Canvas->K2_Project(RootPosition);
		if (!Project.X && !Project.Y) continue;

		SDK::AFortPickup* FortPickup = reinterpret_cast<SDK::AFortPickup*>(Actor);			if (!FortPickup) continue;

		SDK::FFortItemEntry FortItemEntry = FortPickup->PrimaryPickupItemEntry();			if (!FortItemEntry.ItemDefinition()) continue;
		SDK::UFortItemDefinition* FortItemDefinition = FortItemEntry.ItemDefinition();		if (!FortItemDefinition) continue;

		SDK::FText WeaponName = FortItemDefinition->DisplayName();

		if (WeaponName.Data) {
			EFortItemTier Rarity = FortItemDefinition->Tier();

			void** VFT = *(void***)(FortPickup);

			//Hooks::PlaceHook<paramss>(VFT, 0xFF, hookedOriginal, hooked);
			//DEBUG_LOG("org1: " + std::to_string(reinterpret_cast<uintptr_t>(hookedOriginal) - SDK::GetBaseAddress()));
			//DEBUG_LOG("org2: " + std::to_string(*reinterpret_cast<uintptr_t*>(hookedOriginal)));


			//Actor->Vft[0x2464];

			//if (static_cast<int>(Rarity) < Settings::Visuals::Weapons::MinimumRarity) {
			//	continue;
			//}

			SDK::FLinearColor WeaponColor(0.74f, 0.74f, 0.71f, 1.0f);

			switch (Rarity) {
			case EFortItemTier::I:
				WeaponColor = SDK::FLinearColor(0.74f, 0.74f, 0.71f, 1.0f); // Set color for Common rarity
				break;
			case EFortItemTier::II:
				WeaponColor = SDK::FLinearColor(0.12f, 0.87f, 0.11f, 1.0f); // Set color for Uncommon rarity
				break;
			case EFortItemTier::III:
				WeaponColor = SDK::FLinearColor(0.29f, 0.33f, 0.95f, 1.0f); // Set color for Rare rarity
				break;
			case EFortItemTier::IV:
				WeaponColor = SDK::FLinearColor(0.65f, 0.27f, 0.82f, 1.0f); // Set color for Epic rarity
				break;
			case EFortItemTier::V:
				WeaponColor = SDK::FLinearColor(0.95f, 0.40f, 0.07f, 1.0f); // Set color for Legendary rarity
				break;
			case EFortItemTier::VI:
				WeaponColor = SDK::FLinearColor(0.98f, 0.85f, 0.29f, 1.0f); // Set color for Mythic rarity
				break;
			case EFortItemTier::VII:
				WeaponColor = SDK::FLinearColor(0.47f, 1.0f, 0.96f, 1.0f); // Set color for Transcendent rarity
				break;
			default:
				WeaponColor = SDK::FLinearColor(0.74f, 0.74f, 0.71f, 1.0f); // Set color for Default rarity
				break;
			}

			SDK::FString WeaponNameFString = WeaponName.Data->Name;

			Canvas->K2_DrawText(WeaponNameFString, Project, 10.f, WeaponColor, true, true, true);
		}
	}
}