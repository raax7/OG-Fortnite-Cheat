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
#include "../../../Utilities/Logger.h"

void Actors::FortPawn::Tick() {
	bool SeenTarget = false;

	// move somwhere better later this is gay here
	if (SDK::GetLocalPawn() == nullptr) LocalPawnCache.TeamIndex = INT_FAST8_MAX;

	std::vector<Actors::Caches::FortPawnCache> CachedPlayersLocal = Actors::FortPawn::CachedPlayers;

	for (auto it = CachedPlayersLocal.begin(); it != CachedPlayersLocal.end(); ++it) {
		Actors::Caches::FortPawnCache& CurrentPlayer = *it;

		SDK::AActor*					Actor				= CurrentPlayer.FortPawn;								if (!Actor) continue;
		SDK::AFortPawn*					FortPawn			= reinterpret_cast<SDK::AFortPawn*>(Actor);				if (!FortPawn) continue;
		SDK::AFortPlayerState*			FortPlayerState		= FortPawn->PlayerState();								//if (!FortPlayerState) continue;
		SDK::ACharacter*				Character			= static_cast<SDK::ACharacter*>((SDK::APawn*)FortPawn);	if (!Character) continue;
		CurrentPlayer.Mesh									= Character->Mesh();									if (!CurrentPlayer.Mesh) continue;

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
		// If this returns false, the player isn't on the screen and only 5 of the bones were WorldToScreen'd
		CurrentPlayer.IsBoneRegister2DPopulated = Features::FortPawnHelper::PopulateBones(CurrentPlayer);
		Features::FortPawnHelper::PopulateVisibilities(CurrentPlayer);

		//
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

		if (CurrentPlayer.IsBoneRegister2DPopulated) {
			//if (FortPawn == Objects::target.Actor) {
			//	Colour = SDK::FLinearColor(0.9f, 0.5f, 0.1f, 1.f);
			//}

			float Top		= FLT_MAX;
			float Bottom	= FLT_MIN;
			float Left		= FLT_MAX;
			float Right		= FLT_MIN;

			for (int i2 = 0; i2 < CurrentPlayer.BonePositions2D.size(); i2++) {
				if (i2 == Features::FortPawnHelper::Bone::None) continue;

				if (CurrentPlayer.BonePositions2D[i2].X && CurrentPlayer.BonePositions2D[i2].Y) {
					SDK::FVector2D BonePos = CurrentPlayer.BonePositions2D[i2];

					if (!(BonePos.X > 0) && !(BonePos.X < Game::ScreenWidth)) continue;
					if (!(BonePos.Y > 0) && !(BonePos.Y < Game::ScreenHeight)) continue;

					if (BonePos.Y < Top)
						Top = BonePos.Y;
					if (BonePos.Y > Bottom)
						Bottom = BonePos.Y;
					if (BonePos.X < Left)
						Left = BonePos.X;
					if (BonePos.X > Right)
						Right = BonePos.X;
				}
			}

			// Magic numbers for good box size
			float LeftRightOffset = (Right - Left) * 0.36f;
			float TopBottomOffset = (Bottom - Top) * 0.22f;

			SDK::FVector2D bottomLeft(Left - LeftRightOffset, Bottom + (TopBottomOffset * 0.75f));
			SDK::FVector2D topRight(Right + LeftRightOffset, Top - TopBottomOffset);

			float MaxDistance = 150.0f;
			float MinFontSize = 10.0f;
			float MaxFontSize = 20.0f;
			float FontSize = MaxFontSize - (MaxFontSize - MinFontSize) * (CurrentPlayer.DistanceFromLocalPawn / MaxDistance);
			FontSize = (FontSize < MinFontSize) ? MinFontSize : ((FontSize > MaxFontSize) ? MaxFontSize : FontSize);
			
			bool BoneVisible = false;

			for (const auto& Pair : Features::FortPawnHelper::Bone::SkeletonBonePairs) {
				int BoneIDs[2] = { (int)Pair.first, (int)Pair.second };
				SDK::FVector2D ScreenPos[2];

				bool BoneVisibleToPlayer = false;
				bool IsValidPair = true;

				for (int i = 0; i < 2; ++i) {
					int BoneID = BoneIDs[i];

					if (BoneID < -1 || BoneID >= 99) {
						IsValidPair = false;
						break;
					}

					ScreenPos[i] = CurrentPlayer.BonePositions2D[BoneID];

					if (CurrentPlayer.BoneVisibilityStates[BoneID]) {
						BoneVisibleToPlayer = true;
					}

					// Validate screen positions
					if (!Math::IsOnScreen(ScreenPos[i])) {
						IsValidPair = false;
						break;
					}
				}

				if (!IsValidPair) {
					continue;
				}

				BoneVisible = true;

				if (Config::Visuals::Players::Enabled && Config::Visuals::Players::Skeleton) {
					Drawing::Line(
						SDK::FVector2D(ScreenPos[0].X, ScreenPos[0].Y),
						SDK::FVector2D(ScreenPos[1].X, ScreenPos[1].Y),
						1,
						BoneVisibleToPlayer ? SDK::FLinearColor(0.0f, 1.f, 1.f, 1.0f) : SDK::FLinearColor(1.0f, 0.f, 0.f, 1.0f),
						false
					);
				}
			}

			SDK::FLinearColor Colour = SDK::FLinearColor(1.f, 1.f, 1.f, 1.f);
			if (CurrentPlayer.IsAnyBoneVisible) {
				Colour = SDK::FLinearColor(1.f, 0.f, 0.f, 1.f);
			}

			if (BoneVisible){
				if (Config::Visuals::Players::Enabled) {
					if (Config::Visuals::Players::Box) {
						if (CurrentPlayer.DistanceFromLocalPawn > 50) {
							Drawing::CorneredRect(SDK::FVector2D(bottomLeft.X, topRight.Y), SDK::FVector2D((topRight.X - bottomLeft.X), (bottomLeft.Y - topRight.Y)), 1, Colour, false);
						}
						else {
							Drawing::CorneredRect(SDK::FVector2D(bottomLeft.X, topRight.Y), SDK::FVector2D((topRight.X - bottomLeft.X), (bottomLeft.Y - topRight.Y)), 2, Colour, false);
						}
					}

					if (Config::Visuals::Players::Name) {
						SDK::FVector2D PlayerNameTextSize = Drawing::TextSize(CurrentPlayer.PlayerName.ToWString().c_str(), FontSize);
						SDK::FVector2D PlayerNameTextPos = SDK::FVector2D(topRight.X - ((topRight.X / 2) - (bottomLeft.X / 2)), topRight.Y - PlayerNameTextSize.Y - 1);

						Drawing::Text(CurrentPlayer.PlayerName.ToWString().c_str(), PlayerNameTextPos, FontSize, Colour, true, false, true);
					}
				}
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