#include "../Actors.h"

#include "../../SDK/Classes/Engine_classes.h"

#include "../../../Drawing/Drawing.h"
#include "../../../Configs/Config.h"

#include "../../Features/FortPawnHelper/Bone.h"
#include "../../Features/FortPawnHelper/FortPawnHelper.h"
#include "../../Features/Aimbot/Aimbot.h"

#include "../../../Utilities/Math.h"
#include "../../Game.h"
#include "../../../Utilities/Logger.h"

void Actors::FortPawn::Tick() {
	if (!SDK::GetLocalCanvas()) return;

	bool SeenTarget = false;

	// move somwhere better later this is gay here
	if (!SDK::GetLocalController()->AcknowledgedPawn()) LocalPawnCache.TeamIndex = INT_FAST8_MAX;

	for (auto it = CachedPlayers.begin(); it != CachedPlayers.end(); ++it) {
		Actors::Caches::FortPawnCache& CurrentPlayer = *it;

		SDK::AActor*					Actor				= CurrentPlayer.FortPawn;								if (!Actor) continue;
		SDK::AFortPawn*					FortPawn			= reinterpret_cast<SDK::AFortPawn*>(Actor);				if (!FortPawn) continue;
		SDK::AFortPlayerState*			FortPlayerState		= FortPawn->PlayerState();								//if (!FortPlayerState) continue;
		SDK::ACharacter*				Character			= static_cast<SDK::ACharacter*>((SDK::APawn*)FortPawn);	if (!Character) continue;
		CurrentPlayer.Mesh									= Character->Mesh();									if (!CurrentPlayer.Mesh) continue;

		if (FortPawn == SDK::GetLocalPawn()) {
			SDK::FVector Bone = CurrentPlayer.Mesh->GetBonePosition(0);
			LocalPawnCache.Position = Bone;
			LocalPawnCache.TeamIndex = CurrentPlayer.TeamIndex;

			continue;
		}

		if (CurrentPlayer.TeamIndex == LocalPawnCache.TeamIndex) continue;
		if (CurrentPlayer.FortPawn->IsDying()) continue;

		bool DidPopulate2D = Features::FortPawnHelper::PopulateBones(CurrentPlayer);
		Features::FortPawnHelper::PopulateVisibilities(CurrentPlayer);

		CurrentPlayer.IsOnScreen = false;

		for (int i = 0; i < CurrentPlayer.BoneRegister2D.size(); i++) {
			if (CurrentPlayer.BoneRegister2D[i] == 0 && CurrentPlayer.BoneRegister2D[i] == 0) continue;

			if (Math::IsOnScreen(CurrentPlayer.BoneRegister2D[i])) {
				CurrentPlayer.IsOnScreen = true;
			}
		}

		CurrentPlayer.DistanceFromLocal = LocalPawnCache.Position.Distance(CurrentPlayer.BoneRegister[Features::FortPawnHelper::Bone::Root]) / 100.f;

		// Hardcoded max distance, should move to bone population for optimisation
		if (CurrentPlayer.DistanceFromLocal > 500.f) continue;

		// Update any bone visibility
		CurrentPlayer.AnyBoneVisible = false;
		for (int i = 0; i < CurrentPlayer.BoneVisibilities.size(); i++) {
			if (CurrentPlayer.BoneVisibilities[i]) {
				CurrentPlayer.AnyBoneVisible = true;
				break;
			}
		}

		if (DidPopulate2D) {
			//if (FortPawn == Objects::target.Actor) {
			//	Colour = SDK::FLinearColor(0.9f, 0.5f, 0.1f, 1.f);
			//}

			float Top		= FLT_MAX;
			float Bottom	= FLT_MIN;
			float Left		= FLT_MAX;
			float Right		= FLT_MIN;

			for (int i2 = 0; i2 < CurrentPlayer.BoneRegister2D.size(); i2++) {
				if (i2 == Features::FortPawnHelper::Bone::None) continue;

				if (CurrentPlayer.BoneRegister2D[i2].X && CurrentPlayer.BoneRegister2D[i2].Y) {
					SDK::FVector2D BonePos = CurrentPlayer.BoneRegister2D[i2];

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
			float FontSize = MaxFontSize - (MaxFontSize - MinFontSize) * (CurrentPlayer.DistanceFromLocal / MaxDistance);
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

					ScreenPos[i] = CurrentPlayer.BoneRegister2D[BoneID];

					if (CurrentPlayer.BoneVisibilities[BoneID]) {
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

				if (Config::Visuals::Players::Skeleton) {
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
			if (CurrentPlayer.AnyBoneVisible) {
				Colour = SDK::FLinearColor(1.f, 0.f, 0.f, 1.f);
			}

			if (BoneVisible){
				if (Config::Visuals::Players::Enabled) {
					if (Config::Visuals::Players::Box) {
						if (CurrentPlayer.DistanceFromLocal > 50) {
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
		if (Config::Aimbot::Enabled && SDK::GetLocalController()->AcknowledgedPawn()) {
			if (CurrentPlayer.AnyBoneVisible && (!MainTarget.LocalInfo.IsTargeting || !MainTarget.GlobalInfo.TargetActor)) {
				Features::Aimbot::Target PotentialNewTarget{};

				Features::Aimbot::PlayerTarget::UpdateTargetInfo(PotentialNewTarget, CurrentPlayer, AimbotCamera);
				MainTarget.SetTarget(PotentialNewTarget);
			}

			if (MainTarget.GlobalInfo.TargetActor == FortPawn) {
				if (CurrentPlayer.AnyBoneVisible == false) {
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