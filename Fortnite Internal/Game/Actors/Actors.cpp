#include "Actors.h"

#include "../SDK/Classes/FortniteGame_Classes.h"

#include "../Features/Aimbot/Target.h"
#include "../Features/FortPawnHelper/Bone.h"

#include "../../Drawing/Drawing.h"
#include "../../Configs/Config.h"

#include "../Game.h"

void Actors::Tick() {
	// Update FPS scale
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - LastAimbotFrameTime).count();
		LastAimbotFrameTime = currentTime;

		float targetFrameTime = 1.0f / 60.0f; // Targeting 60 FPS frame time

		FPSScale = targetFrameTime / deltaTime;
	}

	// Update Camera and AimbotCamera
	{
		SDK::APlayerCameraManager* CameraManager = SDK::GetLocalController()->PlayerCameraManager();
		if (SDK::IsValidPointer(CameraManager)) {
			MainCamera.Position = CameraManager->GetCameraLocation();
			MainCamera.Rotation = CameraManager->GetCameraRotation();
			MainCamera.FOV = CameraManager->GetFOVAngle();

			AimbotCamera.Position = MainCamera.Position;
			AimbotCamera.FOV = MainCamera.FOV;

			if (Config::Aimbot::SilentAim) {
				if (Config::Aimbot::UseAimKeyForSilent == false || Config::Aimbot::UseAimKeyForSilent && MainTarget.LocalInfo.IsTargeting) {
					AimbotCamera.Rotation = MainTarget.LocalInfo.TargetRotationWithSmooth;

					// Account for if the players actual rotation is closer than the silent aim rotation (so silent aim can't throw for you)
					float SilentPitchDistance = AimbotCamera.Rotation.GetPitchDistance(MainTarget.LocalInfo.TargetRotation);
					float SilentYawDistance = AimbotCamera.Rotation.GetYawDistance(MainTarget.LocalInfo.TargetRotation);

					float RealPitchDistance = MainCamera.Rotation.GetPitchDistance(MainTarget.LocalInfo.TargetRotation);
					float RealYawDistance = MainCamera.Rotation.GetYawDistance(MainTarget.LocalInfo.TargetRotation);

					if (std::abs(RealPitchDistance) < std::abs(SilentPitchDistance)) {
						AimbotCamera.Rotation.Pitch = MainCamera.Rotation.Pitch;
					}
					if (std::abs(RealYawDistance) < std::abs(SilentYawDistance)) {
						AimbotCamera.Rotation.Yaw = MainCamera.Rotation.Yaw;
					}
				}
			}
			else {
				AimbotCamera.Rotation = MainCamera.Rotation;
			}
		}
	}

	// Update local player
	{
		if (SDK::GetLocalController()->AcknowledgedPawn()->GetRootComponent() == nullptr) {
			LocalPawnCache.Position = SDK::GetLocalController()->PlayerCameraManager()->GetCameraLocation();
			LocalPawnCache.TeamIndex = INT_FAST8_MAX;
		}
	}
}
void Actors::Draw() {
	// Draw the aim line
	{
		if (SDK::GetLocalController()->AcknowledgedPawn()) {
			if (Config::Aimbot::ShowAimLine && Config::Aimbot::Enabled) {
				if (MainTarget.GlobalInfo.TargetActor) {
					// If the target is behind us, we need to flip the aim line to make it look correct (K2_Project is weird with things behind us)
					SDK::FVector AimLineEnd = SDK::Project3D(MainTarget.GlobalInfo.TargetBonePosition);

					if (MainTarget.GlobalInfo.Type == Features::Aimbot::Target::TargetType::Weakspot) {
						Drawing::Line(SDK::FVector2D(Game::ScreenWidth / 2.f, Game::ScreenHeight / 2.f), SDK::FVector2D(AimLineEnd.X, AimLineEnd.Y), 1.f, SDK::FLinearColor(1.f, 0.f, 0.f, 1.f), true);
					}
					else {
						Drawing::Line(SDK::FVector2D(Game::ScreenWidth / 2.f, Game::ScreenHeight / 2.f), SDK::FVector2D(AimLineEnd.X, AimLineEnd.Y), 1.f, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
					}
				}
			}
		}
	}

	// Draw FOV circle
	{
		if (SDK::GetLocalController()->AcknowledgedPawn()) {
			if (Config::Aimbot::ShowFOV && Config::Aimbot::Enabled) {
				switch (MainTarget.GlobalInfo.Type) {
				case Features::Aimbot::Target::TargetType::ClosePlayer:
					Drawing::Circle(SDK::FVector2D(Game::ScreenWidth / 2.f, Game::ScreenHeight / 2.f), Config::Aimbot::CloseAim::FOV * Game::PixelsPerDegree, 32, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
					break;
				case Features::Aimbot::Target::TargetType::Weakspot:
					Drawing::Circle(SDK::FVector2D(Game::ScreenWidth / 2.f, Game::ScreenHeight / 2.f), Config::Aimbot::Weakspot::FOV * Game::PixelsPerDegree, 32, SDK::FLinearColor(1.f, 0.f, 0.f, 1.f), true);
					break;
				default:
					Drawing::Circle(SDK::FVector2D(Game::ScreenWidth / 2.f, Game::ScreenHeight / 2.f), Config::Aimbot::Standard::FOV * Game::PixelsPerDegree, 32, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
					break;
				}
			}

			if (Config::Aimbot::TriggerBot::ShowFOV && Config::Aimbot::TriggerBot::Enabled) {
				Drawing::Circle(SDK::FVector2D((float)Game::ScreenWidth / 2.f, (float)Game::ScreenHeight / 2.f), (float)Config::Aimbot::TriggerBot::FOV * (float)Game::PixelsPerDegree, 32, SDK::FLinearColor(0.75f, 0.25f, 0.75f, 1.f), true);
			}
		}
	}
}
void Actors::UpdateCaches() {
	auto CurrentTime = std::chrono::steady_clock::now();

	// Player Cache (to avoid calling GetAllActorsOfClass every tick)
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(CurrentTime - FortPawn::LastCacheTime).count();

		if (ElapsedTime >= FortPawn::IntervalSeconds) {
			FortPawn::LastCacheTime = CurrentTime;

			SDK::TArray<SDK::AActor*> ReturnArray = SDK::UGameplayStatics::StaticClass()->GetAllActorsOfClass(SDK::GetWorld(), SDK::AFortPawn::StaticClass());

			std::vector<Actors::Caches::FortPawnCache> TempCache;

			for (int i = 0; i < ReturnArray.Num(); i++) {
				Actors::Caches::FortPawnCache FortPawnCache{};

				FortPawnCache.FortPawn = static_cast<SDK::AFortPawn*>(ReturnArray[i]);
				SDK::APlayerState* PlayerState = FortPawnCache.FortPawn->PlayerState();
				if (PlayerState) {
					FortPawnCache.PlayerName = PlayerState->GetPlayerName();
					FortPawnCache.TeamIndex = static_cast<SDK::AFortPlayerState*>(PlayerState)->TeamIndex();
				}

				FortPawnCache.BonePositions3D.resize(Features::FortPawnHelper::Bone::BONEID_MAX);
				FortPawnCache.BonePositions2D.resize(Features::FortPawnHelper::Bone::BONEID_MAX);
				FortPawnCache.BoneVisibilityStates.resize(Features::FortPawnHelper::Bone::BONEID_MAX);

				TempCache.push_back(FortPawnCache);
			}

			FortPawn::CachedPlayers = TempCache;
		}
	}

	// Weapon Cache (to avoid calling GetAllActorsOfClass every tick)
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(CurrentTime - FortWeapon::LastCacheTime).count();

		if (ElapsedTime >= FortWeapon::IntervalSeconds) {
			FortWeapon::LastCacheTime = CurrentTime;

			FortWeapon::CachedWeapons = SDK::UGameplayStatics::StaticClass()->GetAllActorsOfClass(SDK::GetWorld(), SDK::AFortPickup::StaticClass());
		}
	}

	// Weakspot Cache (to avoid calling GetAllActorsOfClass every tick)
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(CurrentTime - BuildingWeakSpot::LastCacheTime).count();

		if (ElapsedTime >= BuildingWeakSpot::IntervalSeconds) {
			BuildingWeakSpot::LastCacheTime = CurrentTime;

			BuildingWeakSpot::CachedBuildingWeakSpot = SDK::UGameplayStatics::StaticClass()->GetAllActorsOfClass(SDK::GetWorld(), SDK::ABuildingWeakSpot::StaticClass());
		}
	}
}