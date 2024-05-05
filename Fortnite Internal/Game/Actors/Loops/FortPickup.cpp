#include "../Actors.h"

#include "../../SDK/Classes/Engine_classes.h"
#include "../../SDK/Classes/FortniteGame_classes.h"

#include "../../Features/Visuals/Chams.h"

#include "../../../Drawing/Drawing.h"

#include "../../../Configs/Config.h"

#include "../../Input/Input.h"

#include "../../../Utilities/Math.h"

#include "../../Game.h"

#include <algorithm>

void Actors::FortPickup::Tick()
{
    for (int i = 0; i < CachedWeapons.Num(); i++)
    {
        Features::Visuals::ChamManagerFortPickup::Manager->Tick(CachedWeapons[i]);

        if (Config::Visuals::Weapons::Enabled)
        {
            if (!CachedWeapons.IsValidIndex(i)) continue;

            SDK::AActor* Actor = CachedWeapons[i];												if (SDK::IsValidPointer(Actor) == false) continue;

            SDK::FVector RootPosition = Actor->RootComponent()->RelativeLocation();
            if (RootPosition.Distance(Actors::LocalPawnCache.Position) / 100 > Config::Visuals::Weapons::MaxDistance)
            {
                continue;
            }

            SDK::FVector2D Project = SDK::Project(RootPosition);
            if (Math::IsOnScreen(Project) == false) continue;

            SDK::AFortPickup* FortPickup = SDK::Cast<SDK::AFortPickup>(Actor);					if (SDK::IsValidPointer(FortPickup) == false) continue;

            SDK::FFortItemEntry* FortItemEntry = FortPickup->PrimaryPickupItemEntry();			if (SDK::IsValidPointer(FortItemEntry) == false) continue;
            SDK::UFortItemDefinition* FortItemDefinition = FortItemEntry->ItemDefinition();		if (SDK::IsValidPointer(FortItemDefinition) == false) continue;

            SDK::FText WeaponName = FortItemDefinition->DisplayName();

            if (WeaponName.Data && WeaponName.Data->Name && WeaponName.Data->Length > 0)
            {
                SDK::EFortItemTier Rarity = FortItemDefinition->Tier();

                SDK::FLinearColor WeaponColor = FortItemDefinition->GetRarityColor();

                Drawing::Text(WeaponName.ToString().c_str(), Project, 16.f, WeaponColor, true, true, true);
            }
        }
    }

    if (Config::Exploits::Pickup::AutoPickup || Input::WasKeyJustPressed((Input::KeyName)Config::Exploits::Pickup::PickupAllKey))
    {
        if (Config::Exploits::Pickup::AutoPickup)
        {
            double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(Game::CurrentTime - LastAutoPickupTime).count();

            if (ElapsedTime < (Config::Exploits::Pickup::AutoPickupDelaySecs))
            {
                return;
            }
        }

        LastAutoPickupTime = Game::CurrentTime;

        struct OrderedWeaponsType
        {
            SDK::AActor* Actor;
            float DistanceFromLocal;
        };

        std::vector<OrderedWeaponsType> OrderedWeapons;
        for (int i = 0; i < CachedWeapons.Num(); i++)
        {
            if (!CachedWeapons.IsValidIndex(i)) continue;

            SDK::AActor* Actor = CachedWeapons[i];										if (SDK::IsValidPointer(Actor) == false) continue;

            SDK::FVector RootPosition = Actor->RootComponent()->RelativeLocation();
            float DistanceFromLocal = RootPosition.Distance(Actors::LocalPawnCache.Position);
            if (DistanceFromLocal / 100 >= Config::Exploits::Pickup::MaxDistance)
            {
                continue;
            }

            OrderedWeaponsType OrderedWeapon;
            OrderedWeapon.Actor = Actor;
            OrderedWeapon.DistanceFromLocal = DistanceFromLocal / 100;

            OrderedWeapons.push_back(OrderedWeapon);
        }

        if (Config::Exploits::Pickup::PrioritizeFarthestWeapons)
        {
            // Order so that the farthest weapons are first
            std::sort(OrderedWeapons.begin(), OrderedWeapons.end(), [](const OrderedWeaponsType& a, const OrderedWeaponsType& b) {
                return a.DistanceFromLocal > b.DistanceFromLocal;
                });
        }
        else
        {
            // Order so that the closest weapons are first
            std::sort(OrderedWeapons.begin(), OrderedWeapons.end(), [](const OrderedWeaponsType& a, const OrderedWeaponsType& b) {
                return a.DistanceFromLocal < b.DistanceFromLocal;
                });
        }



        int ItemsPickedUp = 0;

        for (auto OrderedWeapon : OrderedWeapons)
        {
            SDK::AActor* Actor = OrderedWeapon.Actor;									if (SDK::IsValidPointer(Actor) == false) continue;
            SDK::AFortPickup* FortPickup = reinterpret_cast<SDK::AFortPickup*>(Actor);	if (SDK::IsValidPointer(FortPickup) == false) continue;

            SDK::Cast<SDK::AFortPawn>(SDK::GetLocalPawn())->ServerHandlePickup(FortPickup, 0.1f, SDK::FVector(), false);

            ItemsPickedUp++;

            if (ItemsPickedUp >= Config::Exploits::Pickup::MaxItems)
            {
                break;
            }
        }
    }
}