#include "../Actors.h"

#include "../../SDK/Classes/Engine_classes.h"

#include "../../Game.h"

#include "../../../Drawing/Drawing.h"
#include "../../../Configs/Config.h"

#include "../../Features/FortPawnHelper/Bone.h"
#include "../../Features/FortPawnHelper/FortPawnHelper.h"
#include "../../Features/Aimbot/Aimbot.h"
#include "../../Features/Exploits/Vehicle.h"
#include "../../Features/Exploits/Weapon.h"

#include "../../../Utilities/Math.h"

void Actors::FortPawn::Tick() {
	bool SeenTarget = false;

	// move somewhere better later this is gay here
	if (SDK::GetLocalPawn() == nullptr) LocalPawnCache.TeamIndex = INT_FAST8_MAX;

	std::vector<Actors::Caches::FortPawnCache> CachedPlayersLocal = Actors::FortPawn::CachedPlayers;

	for (auto it = CachedPlayersLocal.begin(); it != CachedPlayersLocal.end(); ++it) {
		Actors::Caches::FortPawnCache& CurrentPlayer = *it;

		SDK::AActor*					Actor				= CurrentPlayer.FortPawn;								if (SDK::IsValidPointer(Actor) == false) continue;
		SDK::AFortPawn*					FortPawn			= reinterpret_cast<SDK::AFortPawn*>(Actor);				if (SDK::IsValidPointer(FortPawn) == false) continue;
		SDK::AFortPlayerState*			FortPlayerState		= FortPawn->PlayerState();								//if (SDK::IsValidPointer(FortPlayerState) == false) continue;
		SDK::ACharacter*				Character			= static_cast<SDK::ACharacter*>((SDK::APawn*)FortPawn);	if (SDK::IsValidPointer(Character) == false) continue;
		CurrentPlayer.Mesh									= Character->Mesh();									if (SDK::IsValidPointer(CurrentPlayer.Mesh) == false) continue;

		// LocalPawn caching and exploit ticks
		if (FortPawn == SDK::GetLocalPawn()) {
			LocalPawnCache.Position = CurrentPlayer.Mesh->GetBonePosition(Features::FortPawnHelper::Bone::Head);
			LocalPawnCache.TeamIndex = CurrentPlayer.TeamIndex;

			Features::Exploits::Vehicle::Tick();
			Features::Exploits::Weapon::Tick(FortPawn->CurrentWeapon());

			if (SDK::GetLocalController()) {
				if (Config::Exploits::Player::InfiniteBuilds) {
					reinterpret_cast<SDK::AFortPlayerController*>(SDK::GetLocalController())->SetbBuildFree(true, &Config::Exploits::Player::InfiniteBuilds);
				}

				if (Config::Exploits::Player::InfiniteAmmo) {
					reinterpret_cast<SDK::AFortPlayerController*>(SDK::GetLocalController())->SetbInfiniteAmmo(true, &Config::Exploits::Player::InfiniteAmmo);
				}
			}

			if (Config::Exploits::Player::EditEnemyBuilds) {
				SDK::ABuildingActor* TargetedBuilding = reinterpret_cast<SDK::AFortPlayerController*>(SDK::GetLocalController())->TargetedBuilding();
				if (TargetedBuilding) {
					TargetedBuilding->SetTeamIndex(LocalPawnCache.TeamIndex, & Config::Exploits::Player::EditEnemyBuilds);
				}
			}

			continue;
		}

		// Player state validation
		if (CurrentPlayer.TeamIndex == LocalPawnCache.TeamIndex) continue;
		if (CurrentPlayer.FortPawn->IsDying()) continue;

		// Bone positions and visibility caching
		// If this returns false, the player isn't on the screen and only 5 of the bones were WorldToScreened
		CurrentPlayer.IsBoneRegister2DPopulated = Features::FortPawnHelper::PopulateBones(CurrentPlayer);
		Features::FortPawnHelper::PopulateVisibilities(CurrentPlayer);

		// Update IsPlayerVisibleOnScreen based on if any of the bones 2D positions are on the screen
		CurrentPlayer.IsPlayerVisibleOnScreen = false;
		for (int i = 0; i < CurrentPlayer.BonePositions2D.size(); i++) {
			if (CurrentPlayer.BonePositions2D[i] == SDK::FVector2D()) continue;

			if (Math::IsOnScreen(CurrentPlayer.BonePositions2D[i])) {
				CurrentPlayer.IsPlayerVisibleOnScreen = true;
			}
		}
		
		CurrentPlayer.DistanceFromLocalPawn = LocalPawnCache.Position.Distance(CurrentPlayer.BonePositions3D[Features::FortPawnHelper::Bone::Root]) / 100.f;

		// Hardcoded max distance, should move to bone population for optimisation
		if (CurrentPlayer.DistanceFromLocalPawn > 500.f) continue;

		// Update any bone visibility
		CurrentPlayer.IsAnyBoneVisible = false;
		for (int i = 0; i < CurrentPlayer.BoneVisibilityStates.size(); i++) {
			if (CurrentPlayer.BoneVisibilityStates[i]) {
				CurrentPlayer.IsAnyBoneVisible = true;
				break;
			}
		}

		// Visuals
		if (CurrentPlayer.IsPlayerVisibleOnScreen) {
			SDK::FVector2D BottomLeft, TopRight;
			Features::FortPawnHelper::PopulateBoundCorners(CurrentPlayer, BottomLeft, TopRight);

			float FontSize = Math::CalculateInterpolatedValue(CurrentPlayer.DistanceFromLocalPawn, 150.f, 10.f, 20.f);
			float PrimaryThicknessMultiplier = Math::CalculateInterpolatedValue(75.f, CurrentPlayer.DistanceFromLocalPawn, 1.f, 3.f);
			float SecondaryThicknessMultiplier = Math::CalculateInterpolatedValue(75.f, CurrentPlayer.DistanceFromLocalPawn, 1.f, 2.f);

			float PrimaryThickness = 1.f * PrimaryThicknessMultiplier;
			float SecondaryThickness = 1.f * SecondaryThicknessMultiplier;

			SDK::FLinearColor Color = SDK::FLinearColor(1.f, 1.f, 1.f, 1.f);
			if (CurrentPlayer.IsAnyBoneVisible) {
				Color = SDK::FLinearColor(1.f, 0.f, 0.f, 1.f);
			}

			if (CurrentPlayer.IsPlayerVisibleOnScreen){
				if (Config::Visuals::Players::Enabled) {
					if (Config::Visuals::Players::Skeleton) {
						for (const auto& Pair : Features::FortPawnHelper::Bone::SkeletonBonePairs) {
							int BoneIDs[2] = { (int)Pair.first, (int)Pair.second };
							SDK::FVector2D ScreenPos[2];

							bool BoneVisibleToPlayer = false;

							if (Math::IsOnScreen(ScreenPos[0]) == false && Math::IsOnScreen(ScreenPos[1]) == false) {
								continue;
							}

							for (int i = 0; i <= 1; ++i) {
								int BoneID = BoneIDs[i];

								if (BoneID <= Features::FortPawnHelper::Bone::None || BoneID >= Features::FortPawnHelper::Bone::BONEID_MAX) {
									break;
								}

								ScreenPos[i] = CurrentPlayer.BonePositions2D[BoneID];

								if (CurrentPlayer.BoneVisibilityStates[BoneID]) {
									BoneVisibleToPlayer = true;
								}
							}

							Drawing::Line(
								SDK::FVector2D(ScreenPos[0].X, ScreenPos[0].Y),
								SDK::FVector2D(ScreenPos[1].X, ScreenPos[1].Y),
								SecondaryThicknessMultiplier,
								BoneVisibleToPlayer ? SDK::FLinearColor(0.0f, 1.f, 1.f, 1.0f) : SDK::FLinearColor(1.0f, 0.f, 0.f, 1.0f),
								false
							);
						}
					}

					if (Config::Visuals::Players::Box) {
						Drawing::CorneredRect(BottomLeft, SDK::FVector2D(TopRight - BottomLeft), PrimaryThickness, Color, true);
					}

					if (Config::Visuals::Players::Name) {
						SDK::FVector2D PlayerNameTextSize = Drawing::TextSize(CurrentPlayer.PlayerName.ToWString().c_str(), FontSize);
						SDK::FVector2D PlayerNameTextPos = SDK::FVector2D(TopRight.X - ((TopRight.X / 2) - (BottomLeft.X / 2)), TopRight.Y - PlayerNameTextSize.Y - 1);

						Drawing::Text(CurrentPlayer.PlayerName.ToWString().c_str(), PlayerNameTextPos, FontSize, Color, true, false, true);
					}
				}
			}
		}
		else if (Config::Visuals::Players::Enabled) {
			if (Config::Visuals::Players::OffScreenIndicators::Enabled) {
				SDK::FVector pos = CurrentPlayer.BonePositions3D[Features::FortPawnHelper::Bone::Head]/* + SDK::FVector(0, 0, 75.f)*/; // make the arrows point more towards the body, but also not move when they crouch/uncrouch
				SDK::FVector screen = SDK::Project3D(pos);

				const float radius = (Config::Visuals::Players::OffScreenIndicators::CopyAimbotFOV ? 250 - Config::Visuals::Players::OffScreenIndicators::Height - 3.f : Config::Visuals::Players::OffScreenIndicators::FOV);

				SDK::FVector2D toPlayer = { screen.X - Game::ScreenWidth / 2, screen.Y - Game::ScreenHeight / 2 };

				toPlayer = toPlayer / std::sqrt(toPlayer.X * toPlayer.X + toPlayer.Y * toPlayer.Y);


				const float angle = std::atan2(toPlayer.Y, toPlayer.X);
				const float xOffset = std::cos(angle) * radius;
				const float yOffset = std::sin(angle) * radius;

				const SDK::FVector2D Point = { Game::ScreenWidth / 2 + xOffset, Game::ScreenHeight / 2 + yOffset };

				const SDK::FVector2D p3 = { Point.X + toPlayer.X * Config::Visuals::Players::OffScreenIndicators::Height, Point.Y + toPlayer.Y * Config::Visuals::Players::OffScreenIndicators::Height };

				toPlayer = toPlayer * Config::Visuals::Players::OffScreenIndicators::Size;
				const SDK::FVector2D p1 = { Point.X - toPlayer.Y, Point.Y + toPlayer.X };
				const SDK::FVector2D p2 = { Point.X + toPlayer.Y, Point.Y - toPlayer.X };

				const SDK::FLinearColor col = CurrentPlayer.IsAnyBoneVisible ? SDK::FLinearColor(1.f, 1.f, 1.f, 1.f) : SDK::FLinearColor(1.f, 1.f, 1.f, 1.f);

				Drawing::Triangle(p1, p2, p3, 1.f, col, true, true);
			}
		}

		// Aimbot
		if (Config::Aimbot::Enabled && SDK::GetLocalPawn()) {
			if ((CurrentPlayer.IsAnyBoneVisible || Config::Aimbot::VisibleCheck == false) && ((MainTarget.LocalInfo.IsTargeting == false || Config::Aimbot::StickyAim == false) || MainTarget.GlobalInfo.TargetActor == nullptr)) {
				Features::Aimbot::Target PotentialNewTarget{};

				Features::Aimbot::PlayerTarget::UpdateTargetInfo(PotentialNewTarget, CurrentPlayer, MainCamera, AimbotCamera);
				MainTarget.SetTarget(PotentialNewTarget);
			}

			if (MainTarget.GlobalInfo.TargetActor == FortPawn) {
				if (CurrentPlayer.IsAnyBoneVisible == false && Config::Aimbot::VisibleCheck == true) {
					MainTarget.ResetTarget();
				}
				else {
					SeenTarget = true;

					Features::Aimbot::PlayerTarget::UpdateTargetInfo(MainTarget, CurrentPlayer, MainCamera, AimbotCamera, FPSScale);

					Features::Aimbot::AimbotTarget(MainTarget);
				}
			}
		}
	}

	if (MainTarget.GlobalInfo.Type == Features::Aimbot::Target::TargetType::ClosePlayer
		|| MainTarget.GlobalInfo.Type == Features::Aimbot::Target::TargetType::FarPlayer) {
		MainTarget.TargetTick(SeenTarget);
	}
}