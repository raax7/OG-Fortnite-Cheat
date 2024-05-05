#include "../Actors.h"

#include "../../SDK/Classes/Engine_classes.h"

#include "../../Game.h"

#include "../../../Configs/Config.h"
#include "../../../Drawing/Drawing.h"

#include "../../Features/Aimbot/Aimbot.h"
#include "../../Features/Exploits/Player.h"
#include "../../Features/Exploits/Vehicle.h"
#include "../../Features/Exploits/Weapon.h"
#include "../../Features/FortPawnHelper/Bone.h"
#include "../../Features/FortPawnHelper/FortPawnHelper.h"
#include "../../Features/Visuals/Chams.h"

#include "../../../Utilities/Math.h"

void Actors::FortPawn::Tick()
{
    bool SeenTarget = false;

    // move somewhere better later this is gay here
    if (SDK::GetLocalPawn() == nullptr) LocalPawnCache.TeamIndex = INT_FAST8_MAX;

    std::vector<Actors::Caches::FortPawnCache> CachedPlayersLocal = Actors::FortPawn::CachedPlayers;

    for (auto it = CachedPlayersLocal.begin(); it != CachedPlayersLocal.end(); ++it)
    {
        Actors::Caches::FortPawnCache& CurrentPlayer = *it;

        SDK::AFortPawn* FortPawn = CurrentPlayer.FortPawn;										if (SDK::IsValidPointer(FortPawn) == false) continue;
        SDK::AFortPlayerState* FortPlayerState = SDK::Cast<SDK::AFortPlayerState>(FortPawn->PlayerState());	//if (SDK::IsValidPointer(FortPlayerState) == false) continue;
        CurrentPlayer.Mesh = FortPawn->Mesh();												if (SDK::IsValidPointer(CurrentPlayer.Mesh) == false) continue;

        // LocalPawn caching and exploit ticks
        if (FortPawn == SDK::GetLocalPawn())
        {
            LocalPawnCache.Position = CurrentPlayer.Mesh->GetBonePosition(Features::FortPawnHelper::Bone::Head);
            LocalPawnCache.TeamIndex = CurrentPlayer.TeamIndex;

            Features::Exploits::Vehicle::Tick(SDK::Cast<SDK::AFortPlayerPawnAthena>(FortPawn));
            Features::Exploits::Weapon::Tick(FortPawn->CurrentWeapon());
            Features::Exploits::Player::Tick(SDK::Cast<SDK::AFortPlayerPawnAthena>(FortPawn), SDK::Cast<SDK::AFortPlayerController>(SDK::GetLocalController()));

            // Apply chams (if enabled and ShowLocal is enabled)
            Features::Visuals::ChamManagerFortPawn::Manager->Tick(FortPawn);

            continue;
        }

        // Player state validation
        if (CurrentPlayer.TeamIndex == LocalPawnCache.TeamIndex) continue;
        if (CurrentPlayer.FortPawn->IsDying()) continue;
        SDK::AFortPlayerStateZone* SpectatingTarget = SDK::Cast<SDK::AFortPlayerStateZone>(SDK::GetLocalPawn()->PlayerState())->SpectatingTarget();
        if (SpectatingTarget == SDK::Cast<SDK::AFortPlayerStateZone>(FortPlayerState) && (SDK::IsValidPointer(SpectatingTarget) && SDK::IsValidPointer(FortPlayerState))) continue;

        // Apply chams (if enabled)
        Features::Visuals::ChamManagerFortPawn::Manager->Tick(FortPawn);

        // Kill all players by teleporting them infront of the local pawn allowing free shots
        if (Config::Exploits::Player::KillAll)
        {
            SDK::FVector ForwardVector = SDK::UKismetMathLibrary::GetForwardVector(SDK::FRotator(0.f, Actors::MainCamera.Rotation.Yaw, Actors::MainCamera.Rotation.Roll));

            FortPawn->K2_SetActorLocation(Actors::MainCamera.Position + (ForwardVector * 350.f), false, nullptr, true);
        }

        // Bone positions and visibility caching
        // If this returns false, the player isn't on the screen and only 5 of the bones were WorldToScreened
        CurrentPlayer.IsBoneRegister2DPopulated = Features::FortPawnHelper::PopulateBones(CurrentPlayer);
        Features::FortPawnHelper::PopulateVisibilities(CurrentPlayer);

        // Update IsPlayerVisibleOnScreen based on if any of the bones 2D positions are on the screen
        CurrentPlayer.IsPlayerVisibleOnScreen = false;
        for (int i = 0; i < CurrentPlayer.BonePositions2D.size(); i++)
        {
            if (CurrentPlayer.BonePositions2D[i] == SDK::FVector2D()) continue;

            if (Math::IsOnScreen(CurrentPlayer.BonePositions2D[i]))
            {
                CurrentPlayer.IsPlayerVisibleOnScreen = true;
            }
        }

        CurrentPlayer.DistanceFromLocalPawn = LocalPawnCache.Position.Distance(CurrentPlayer.BonePositions3D[Features::FortPawnHelper::Bone::Root]) / 100.f;

        // Hardcoded max distance, should move to bone population for optimisation
        if (CurrentPlayer.DistanceFromLocalPawn > 500.f) continue;

        // Update any bone visibility
        CurrentPlayer.IsAnyBoneVisible = false;
        for (int i = 0; i < CurrentPlayer.BoneVisibilityStates.size(); i++)
        {
            if (CurrentPlayer.BoneVisibilityStates[i])
            {
                CurrentPlayer.IsAnyBoneVisible = true;
                break;
            }
        }

        // Update the current weapon and magazine ammo count
        SDK::AFortWeapon* CurrentWeapon = FortPawn->CurrentWeapon();
        if (CurrentWeapon != CurrentPlayer.Weapon)
        {
            CurrentPlayer.Weapon = CurrentWeapon;
            CurrentPlayer.BulletsPerClip = CurrentWeapon->GetBulletsPerClip();
        }

        // Visuals
        if (CurrentPlayer.IsPlayerVisibleOnScreen)
        {
            SDK::FVector2D TopLeft, BottomRight;
            Features::FortPawnHelper::PopulateBoundCorners(CurrentPlayer, TopLeft, BottomRight);

            float FontSize = Math::CalculateInterpolatedValue(150.f, CurrentPlayer.DistanceFromLocalPawn, 12.f, 20.f);
            float PrimaryThicknessMultiplier = Math::CalculateInterpolatedValue(75.f, CurrentPlayer.DistanceFromLocalPawn, 1.f, 3.f);
            float SecondaryThicknessMultiplier = Math::CalculateInterpolatedValue(75.f, CurrentPlayer.DistanceFromLocalPawn, 1.f, 2.f);

            float PrimaryThickness = 1.f * PrimaryThicknessMultiplier;
            float SecondaryThickness = 1.f * SecondaryThicknessMultiplier;

            SDK::FLinearColor PrimaryColor = SDK::FLinearColor(1.f, 1.f, 1.f, 1.f);
            if (CurrentPlayer.IsAnyBoneVisible)
            {
                PrimaryColor = SDK::FLinearColor(1.f, 0.f, 0.f, 1.f);
            }

            SDK::FLinearColor SecondaryColor = SDK::FLinearColor(1.0f, 0.f, 0.f, 1.0f);
            if (CurrentPlayer.IsAnyBoneVisible)
            {
                SecondaryColor = SDK::FLinearColor(0.0f, 1.f, 1.f, 1.0f);
            }

            if (CurrentPlayer.IsPlayerVisibleOnScreen)
            {
                if (Config::Visuals::Players::Enabled)
                {
                    if (Config::Visuals::Players::Skeleton)
                    {
                        for (const auto& Pair : Features::FortPawnHelper::Bone::SkeletonBonePairs)
                        {
                            int BoneIDs[2] = { (int)Pair.first, (int)Pair.second };
                            SDK::FVector2D ScreenPos[2];

                            bool BoneVisibleToPlayer = false;

                            if (Math::IsOnScreen(ScreenPos[0]) == false && Math::IsOnScreen(ScreenPos[1]) == false)
                            {
                                continue;
                            }

                            for (int i = 0; i <= 1; ++i)
                            {
                                int BoneID = BoneIDs[i];

                                if (BoneID <= Features::FortPawnHelper::Bone::None || BoneID >= Features::FortPawnHelper::Bone::BONEID_MAX)
                                {
                                    break;
                                }

                                ScreenPos[i] = CurrentPlayer.BonePositions2D[BoneID];

                                if (CurrentPlayer.BoneVisibilityStates[BoneID])
                                {
                                    BoneVisibleToPlayer = true;
                                }
                            }

                            Drawing::Line(
                                SDK::FVector2D(ScreenPos[0].X, ScreenPos[0].Y),
                                SDK::FVector2D(ScreenPos[1].X, ScreenPos[1].Y),
                                SecondaryThicknessMultiplier,
                                Config::Visuals::Players::IndividualBoneVisibilities ? BoneVisibleToPlayer ? SDK::FLinearColor(0.0f, 1.f, 1.f, 1.0f) : SDK::FLinearColor(1.0f, 0.f, 0.f, 1.0f) : SecondaryColor,
                                false
                            );
                        }
                    }

                    if (Config::Visuals::Players::Box)
                    {
                        switch (Config::Visuals::Players::BoxType)
                        {
                        case ConfigTypes::BoxType::Full3D:
                            // ADD THIS LATER
                            break;
                        case ConfigTypes::BoxType::Cornered3D:
                            // ADD THIS LATER
                            break;
                        case ConfigTypes::BoxType::Full2D:
                            Drawing::Rect(TopLeft, SDK::FVector2D(BottomRight - TopLeft), PrimaryThickness, PrimaryColor, true);
                            break;
                        case ConfigTypes::BoxType::Cornered2D:
                            Drawing::CorneredRect(TopLeft, SDK::FVector2D(BottomRight - TopLeft), PrimaryThickness, PrimaryColor, true);
                            break;
                        }
                    }

                    if (Config::Visuals::Players::Name)
                    {
                        // Text position at the top of the box
                        SDK::FVector2D PlayerNameTextPos = SDK::FVector2D(TopLeft.X + (BottomRight.X - TopLeft.X) / 2, TopLeft.Y - FontSize - 2);

                        Drawing::Text(CurrentPlayer.PlayerName.ToString().c_str(), PlayerNameTextPos, FontSize, PrimaryColor, true, false, true);
                    }

                    if (Config::Visuals::Players::Distance)
                    {
                        // Text position at the bottom of the box
                        SDK::FVector2D PlayerDistanceTextPos = SDK::FVector2D(TopLeft.X + (BottomRight.X - TopLeft.X) / 2, BottomRight.Y + 2);

                        // Cast to int to remove decimal places
                        std::string DistanceString = skCrypt("[ ").decrypt() + std::to_string((int)CurrentPlayer.DistanceFromLocalPawn) + skCrypt(" m ]").decrypt();
                        Drawing::Text(DistanceString.c_str(), PlayerDistanceTextPos, FontSize, PrimaryColor, true, false, true);
                    }

                    if (Config::Visuals::Players::CurrentWeapon)
                    {
                        if (CurrentWeapon)
                        {
                            std::string WeaponDisplayName = CurrentWeapon->WeaponData()->DisplayName().ToString();
                            int CurrentAmmoCount = CurrentWeapon->AmmoCount();

                            // Add the ammo count to the weapon display name (only if the clip isn't infinite)
                            if (CurrentPlayer.BulletsPerClip != 0)
                            {
                                WeaponDisplayName += std::string(skCrypt(" [ ")) + std::to_string(CurrentAmmoCount) + std::string(skCrypt(" / ")) + std::to_string(CurrentPlayer.BulletsPerClip) + std::string(skCrypt(" ]"));
                            }

                            SDK::FVector2D WeaponTextPos = SDK::FVector2D(TopLeft.X + (BottomRight.X - TopLeft.X) / 2, BottomRight.Y);
                            WeaponTextPos.Y += Config::Visuals::Players::Distance ? FontSize + 1 : 0;

                            Drawing::Text(WeaponDisplayName.c_str(), WeaponTextPos, FontSize, CurrentWeapon->WeaponData()->GetRarityColor(), true, false, true);
                        }
                    }
                }
            }
        }
        else if (Config::Visuals::Players::Enabled)
        {
            if (Config::Visuals::Players::OffScreenIndicators::Enabled)
            {
                SDK::FVector2D TipPoint, BasePoint1, BasePoint2;

                // Calculate offscreen indicator triangle points
                {
                    // TO-DO: Cache this so we don't have to get the head pos twice per tick
                    const SDK::FVector HeadPosition = CurrentPlayer.BonePositions3D[Features::FortPawnHelper::Bone::Head];
                    SDK::FVector HeadPosition2D = SDK::Project3D(HeadPosition);

                    // Calculate the indicator's position
                    float Radius = Config::Visuals::Players::OffScreenIndicators::CopyAimbotFOV ? Actors::CurrentFOVSizePixels : Config::Visuals::Players::OffScreenIndicators::FOV * Game::PixelsPerDegree;
                    SDK::FVector2D DirectionToPlayer = SDK::FVector2D(HeadPosition2D.X - Game::ScreenCenterX, HeadPosition2D.Y - Game::ScreenCenterY);
                    float Magnitude = std::sqrt(DirectionToPlayer.X * DirectionToPlayer.X + DirectionToPlayer.Y * DirectionToPlayer.Y);
                    DirectionToPlayer = DirectionToPlayer / Magnitude;

                    float Angle = std::atan2(DirectionToPlayer.Y, DirectionToPlayer.X);
                    SDK::FVector2D IndicatorPosition = {
                        Game::ScreenCenterX + std::cos(Angle) * Radius,
                        Game::ScreenCenterY + std::sin(Angle) * Radius
                    };

                    // Calculate the points for the indicator triangle
                    TipPoint = IndicatorPosition + DirectionToPlayer * Config::Visuals::Players::OffScreenIndicators::Height;
                    BasePoint1 = IndicatorPosition + SDK::FVector2D(-DirectionToPlayer.Y, DirectionToPlayer.X) * Config::Visuals::Players::OffScreenIndicators::Width;
                    BasePoint2 = IndicatorPosition - SDK::FVector2D(-DirectionToPlayer.Y, DirectionToPlayer.X) * Config::Visuals::Players::OffScreenIndicators::Width;

                }

                SDK::FLinearColor IndicatorColor = CurrentPlayer.IsAnyBoneVisible ? SDK::FLinearColor(0.2f, 1.f, 0.2f, 1.f) : SDK::FLinearColor(1.f, 0.2f, 0.2f, 1.f);
                Drawing::Triangle(BasePoint1, BasePoint2, TipPoint, 1.f, IndicatorColor, true, true);
            }
        }

        // Aimbot
        if (Config::Aimbot::Enabled && SDK::GetLocalPawn())
        {
            if ((CurrentPlayer.IsAnyBoneVisible || Config::Aimbot::VisibleCheck == false || Config::Aimbot::BulletTP == true || Config::Aimbot::BulletTPV2 == true) && ((MainTarget.LocalInfo.IsTargeting == false || Config::Aimbot::StickyAim == false) || MainTarget.GlobalInfo.TargetActor == nullptr))
            {
                Features::Aimbot::Target PotentialNewTarget{};

                Features::Aimbot::PlayerTarget::UpdateTargetInfo(PotentialNewTarget, CurrentPlayer, MainCamera, AimbotCamera);
                MainTarget.SetTarget(PotentialNewTarget);
            }

            if (MainTarget.GlobalInfo.TargetActor == FortPawn)
            {
                if (CurrentPlayer.IsAnyBoneVisible == false && (Config::Aimbot::VisibleCheck == true && Config::Aimbot::BulletTP == false && Config::Aimbot::BulletTPV2 == false))
                {
                    MainTarget.ResetTarget();
                }
                else
                {
                    SeenTarget = true;

                    Features::Aimbot::PlayerTarget::UpdateTargetInfo(MainTarget, CurrentPlayer, MainCamera, AimbotCamera, FPSScale);
                    Features::Aimbot::AimbotTarget(MainTarget);
                }
            }
        }
    }

    if (MainTarget.GlobalInfo.Type == Features::Aimbot::Target::TargetType::ClosePlayer
        || MainTarget.GlobalInfo.Type == Features::Aimbot::Target::TargetType::FarPlayer)
    {
        MainTarget.TargetTick(SeenTarget);
    }
}