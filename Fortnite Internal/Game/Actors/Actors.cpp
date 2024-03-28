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
		if (SDK::GetLocalController()->AcknowledgedPawn()->RootComponent() == nullptr) {
			LocalPawnCache.Position = SDK::GetLocalController()->PlayerCameraManager()->GetCameraLocation();
			LocalPawnCache.TeamIndex = INT_FAST8_MAX;
		}
	}

	// Update the current FOV size
	{
		switch (MainTarget.GlobalInfo.Type) {
		case Features::Aimbot::Target::TargetType::ClosePlayer:
			CurrentFOVSizeDegrees = Config::Aimbot::CloseAim::FOV;
			CurrentFOVSizePixels = CurrentFOVSizeDegrees * Game::PixelsPerDegree;
			break;
		case Features::Aimbot::Target::TargetType::Weakspot:
			CurrentFOVSizeDegrees = Config::Aimbot::Weakspot::FOV;
			CurrentFOVSizePixels = CurrentFOVSizeDegrees * Game::PixelsPerDegree;
			break;
		default:
			CurrentFOVSizeDegrees = Config::Aimbot::Standard::FOV;
			CurrentFOVSizePixels = CurrentFOVSizeDegrees * Game::PixelsPerDegree;
			break;
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
						Drawing::Line(SDK::FVector2D(Game::ScreenCenterX, Game::ScreenCenterY), SDK::FVector2D(AimLineEnd.X, AimLineEnd.Y), 1.f, SDK::FLinearColor(1.f, 0.f, 0.f, 1.f), true);
					}
					else {
						Drawing::Line(SDK::FVector2D(Game::ScreenCenterX, Game::ScreenCenterY), SDK::FVector2D(AimLineEnd.X, AimLineEnd.Y), 1.f, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
					}
				}
			}
		}
	}

	// Draw FOV circle
	{
		if (SDK::GetLocalController()->AcknowledgedPawn()) {
			if (Config::Aimbot::ShowFOV && Config::Aimbot::Enabled) {
				// Only do high segment count for ImGui (since we don't have overhead of ProcessEvent)
#ifdef _IMGUI
				const int Segments = 128;
#else
				const int Segments = 32;
#endif

				Drawing::Circle(SDK::FVector2D(Game::ScreenCenterX, Game::ScreenCenterY), Actors::CurrentFOVSizePixels , Segments, MainTarget.GlobalInfo.Type == Features::Aimbot::Target::TargetType::Weakspot ? SDK::FLinearColor(1.f, 0.f, 0.f, 1.f) : SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
			}
		}
	}
}
void Actors::UpdateCaches() {
	// Player Cache
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(Game::CurrentTime - FortPawn::LastCacheTime).count();

		if (ElapsedTime >= FortPawn::IntervalSeconds) {
			FortPawn::LastCacheTime = Game::CurrentTime;

			SDK::TArray<SDK::AActor*> ReturnArray = SDK::UGameplayStatics::GetAllActorsOfClass(SDK::GetWorld(), SDK::AFortPawn::StaticClass());

			std::vector<Actors::Caches::FortPawnCache> TempCache;

			for (int i = 0; i < ReturnArray.Num(); i++) {
				Actors::Caches::FortPawnCache FortPawnCache{};

				FortPawnCache.FortPawn = SDK::Cast<SDK::AFortPawn, true>(ReturnArray[i]);
				SDK::APlayerState* PlayerState = FortPawnCache.FortPawn->PlayerState();
				if (SDK::IsValidPointer(PlayerState)) {
					FortPawnCache.PlayerName = PlayerState->GetPlayerName();
					FortPawnCache.TeamIndex = SDK::Cast<SDK::AFortPlayerState>(PlayerState)->TeamIndex();
				}

				FortPawnCache.BonePositions3D.resize(Features::FortPawnHelper::Bone::BONEID_MAX);
				FortPawnCache.BonePositions2D.resize(Features::FortPawnHelper::Bone::BONEID_MAX);
				FortPawnCache.BoneVisibilityStates.resize(Features::FortPawnHelper::Bone::BONEID_MAX);

				TempCache.push_back(FortPawnCache);
			}

			FortPawn::CachedPlayers = TempCache;
		}
	}

	// Weapon Cache
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(Game::CurrentTime - FortPickup::LastCacheTime).count();

		if (ElapsedTime >= FortPickup::IntervalSeconds) {
			FortPickup::LastCacheTime = Game::CurrentTime;

			FortPickup::CachedWeapons = SDK::UGameplayStatics::GetAllActorsOfClass(SDK::GetWorld(), SDK::AFortPickup::StaticClass());
		}
	}

	// Weakspot Cache
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(Game::CurrentTime - BuildingWeakSpot::LastCacheTime).count();

		if (ElapsedTime >= BuildingWeakSpot::IntervalSeconds) {
			BuildingWeakSpot::LastCacheTime = Game::CurrentTime;

			BuildingWeakSpot::CachedBuildingWeakSpot = SDK::UGameplayStatics::GetAllActorsOfClass(SDK::GetWorld(), SDK::ABuildingWeakSpot::StaticClass());
		}
	}

	// FortAthenaVehicle Cache
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(Game::CurrentTime - FortAthenaVehicle::LastCacheTime).count();

		if (ElapsedTime >= FortAthenaVehicle::IntervalSeconds) {
			FortAthenaVehicle::LastCacheTime = Game::CurrentTime;

			FortAthenaVehicle::CachedVehicles = SDK::UGameplayStatics::GetAllActorsOfClass(SDK::GetWorld(), SDK::AFortAthenaVehicle::StaticClass());
		}
	}
}