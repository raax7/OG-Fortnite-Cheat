#include "../Actors.h"

#include "../../SDK/Classes/Engine_classes.h"
#include "../../SDK/Classes/FortniteGame_classes.h"

#include "../../../Drawing/Drawing.h"
#include "../../../Configs/Config.h"
#include "../../Input/Input.h"

#include <algorithm>

void Actors::FortWeapon::Tick() {
	for (int i = 0; i < CachedWeapons.Num(); i++) {
		if (Config::Visuals::Weapons::Enabled) {
			if (!CachedWeapons.IsValidIndex(i)) continue;

			SDK::AActor* Actor = CachedWeapons[i];												if (!Actor) continue;

			SDK::FVector RootPosition = Actor->GetRootComponent()->GetPosition();
			if (RootPosition.Distance(SDK::GetLocalController()->AcknowledgedPawn()->GetRootComponent()->GetPosition()) / 100 >= Config::Visuals::Weapons::WeaponMaxDistance) {
				continue;
			}

			SDK::FVector2D Project = SDK::Project(RootPosition);
			if (!Project.X && !Project.Y) continue;

			SDK::AFortPickup* FortPickup = reinterpret_cast<SDK::AFortPickup*>(Actor);			if (!FortPickup) continue;



		
			SDK::FFortItemEntry* FortItemEntry = FortPickup->PrimaryPickupItemEntry();			if (!FortItemEntry) continue;
			SDK::UFortItemDefinition* FortItemDefinition = FortItemEntry->ItemDefinition();		if (!FortItemDefinition) continue;

			SDK::FText WeaponName = FortItemDefinition->DisplayName();

			if (WeaponName.Data && WeaponName.Data->Name && WeaponName.Data->Length > 0) {
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

				DEBUG_LOG(LOG_INFO, "WeaponName: ", WeaponName.ToString());

				Drawing::Text(WeaponName.ToString().c_str(), Project, 16.f, WeaponColor, true, true, true);
			}
		}
	}

	if (Config::Exploits::Pickup::AutoPickup || Input::WasKeyJustPressed((Input::KeyName)Config::Exploits::Pickup::PickupAllKey)) {
		std::chrono::steady_clock::time_point CurrentTime = std::chrono::steady_clock::now();
		if (Config::Exploits::Pickup::AutoPickup) {
			double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(CurrentTime - LastAutoPickupTime).count();

			if (ElapsedTime < (Config::Exploits::Pickup::AutoPickupDelaySecs)) {
				return;
			}
		}

		LastAutoPickupTime = CurrentTime;

		struct OrderedWeaponsType {
			SDK::AActor* Actor;
			float DistanceFromLocal;
		};

		std::vector<OrderedWeaponsType> OrderedWeapons;
		for (int i = 0; i < CachedWeapons.Num(); i++) {
			if (!CachedWeapons.IsValidIndex(i)) continue;

			SDK::AActor* Actor = CachedWeapons[i];										if (!Actor) continue;

			SDK::FVector RootPosition = Actor->GetRootComponent()->GetPosition();
			float DistanceFromLocal = RootPosition.Distance(Actors::LocalPawnCache.Position);
			if (DistanceFromLocal / 100 >= Config::Exploits::Pickup::MaxDistance) {
				continue;
			}

			OrderedWeaponsType OrderedWeapon;
			OrderedWeapon.Actor = Actor;
			OrderedWeapon.DistanceFromLocal = DistanceFromLocal / 100;

			OrderedWeapons.push_back(OrderedWeapon);
		}
		
		if (Config::Exploits::Pickup::PrioritizeFarthestWeapons) {
			// Order so that the farthest weapons are first
			std::sort(OrderedWeapons.begin(), OrderedWeapons.end(), [](const OrderedWeaponsType& a, const OrderedWeaponsType& b) {
				return a.DistanceFromLocal > b.DistanceFromLocal;
			});
		}
		else {
			// Order so that the closest weapons are first
			std::sort(OrderedWeapons.begin(), OrderedWeapons.end(), [](const OrderedWeaponsType& a, const OrderedWeaponsType& b) {
				return a.DistanceFromLocal < b.DistanceFromLocal;
			});
		}



		int ItemsPickedUp = 0;

		for (auto OrderedWeapon : OrderedWeapons) {
			SDK::AActor* Actor = OrderedWeapon.Actor;									if (!Actor) continue;
			SDK::AFortPickup* FortPickup = reinterpret_cast<SDK::AFortPickup*>(Actor);	if (!FortPickup) continue;

			reinterpret_cast<SDK::AFortPawn*>(SDK::GetLocalPawn())->ServerHandlePickup(FortPickup, 0.1f, SDK::FVector(), false);

			ItemsPickedUp++;

			if (ItemsPickedUp >= Config::Exploits::Pickup::MaxItems) {
				break;
			}
		}
	}
}