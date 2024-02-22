#include "../Actors.h"

#include "../../SDK/Classes/Engine_classes.h"
#include "../../SDK/Classes/FortniteGame_classes.h"

#include "../../../Drawing/Drawing.h"
#include "../../../Configs/Config.h"

void Actors::FortWeapon::Tick() {
	if (!Config::Visuals::Weapons::Enabled) return;

	for (int i = 0; i < CachedWeapons.Num(); i++) {
		if (!CachedWeapons.IsValidIndex(i)) continue;

		SDK::AActor* Actor = CachedWeapons[i];												if (!Actor) continue;

		SDK::FVector RootPosition = Actor->GetRootComponent()->GetPosition();
		if (RootPosition.Distance(SDK::GetLocalController()->AcknowledgedPawn()->GetRootComponent()->GetPosition()) / 100 >= Config::Visuals::Weapons::WeaponMaxDistance) {
			continue;
		}

		SDK::FVector2D Project = SDK::Project(RootPosition);
		if (!Project.X && !Project.Y) continue;

		SDK::AFortPickup* FortPickup = reinterpret_cast<SDK::AFortPickup*>(Actor);			if (!FortPickup) continue;

		SDK::FFortItemEntry FortItemEntry = FortPickup->PrimaryPickupItemEntry();			if (!FortItemEntry.ItemDefinition()) continue;
		SDK::UFortItemDefinition* FortItemDefinition = FortItemEntry.ItemDefinition();		if (!FortItemDefinition) continue;

		SDK::FText WeaponName = FortItemDefinition->DisplayName();

		if (WeaponName.Data) {
			SDK::EFortItemTier Rarity = FortItemDefinition->Tier();

			SDK::FLinearColor WeaponColor(0.74f, 0.74f, 0.71f, 1.0f);

			switch (Rarity) {
			case SDK::EFortItemTier::I:
				WeaponColor = SDK::FLinearColor(0.74f, 0.74f, 0.71f, 1.0f); // Set color for Common rarity
				break;
			case SDK::EFortItemTier::II:
				WeaponColor = SDK::FLinearColor(0.12f, 0.87f, 0.11f, 1.0f); // Set color for Uncommon rarity
				break;
			case SDK::EFortItemTier::III:
				WeaponColor = SDK::FLinearColor(0.29f, 0.33f, 0.95f, 1.0f); // Set color for Rare rarity
				break;
			case SDK::EFortItemTier::IV:
				WeaponColor = SDK::FLinearColor(0.65f, 0.27f, 0.82f, 1.0f); // Set color for Epic rarity
				break;
			case SDK::EFortItemTier::V:
				WeaponColor = SDK::FLinearColor(0.95f, 0.40f, 0.07f, 1.0f); // Set color for Legendary rarity
				break;
			case SDK::EFortItemTier::VI:
				WeaponColor = SDK::FLinearColor(0.98f, 0.85f, 0.29f, 1.0f); // Set color for Mythic rarity
				break;
			case SDK::EFortItemTier::VII:
				WeaponColor = SDK::FLinearColor(0.47f, 1.0f, 0.96f, 1.0f); // Set color for Transcendent rarity
				break;
			default:
				WeaponColor = SDK::FLinearColor(0.74f, 0.74f, 0.71f, 1.0f); // Set color for Default rarity
				break;
			}

			Drawing::Text(WeaponName.Data->Name, Project, 10.f, WeaponColor, true, true, true);
		}
	}
}