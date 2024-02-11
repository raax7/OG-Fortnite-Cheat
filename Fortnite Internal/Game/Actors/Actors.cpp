#include "Actors.h"
#include "../../Drawing/Drawing.h"
#include "../SDK/Classes/FortniteGame_Classes.h"

void Actors::Tick() {
	if (!SDK::GetLocalCanvas()) return;

	// Adjust FOV for screen resolution
	Config::Aimbot::CloseAim::RealFOV = static_cast<float>(Config::Aimbot::CloseAim::FOV) * (static_cast<float>(SDK::GetLocalCanvas()->SizeY()) / 1080.0f);
	Config::Aimbot::Standard::RealFOV = static_cast<float>(Config::Aimbot::Standard::FOV) * (static_cast<float>(SDK::GetLocalCanvas()->SizeY()) / 1080.0f);
	Config::Aimbot::Weakspot::RealFOV = static_cast<float>(Config::Aimbot::Weakspot::FOV) * (static_cast<float>(SDK::GetLocalCanvas()->SizeY()) / 1080.0f);
	Config::Aimbot::TriggerBot::RealFOV = static_cast<float>(Config::Aimbot::TriggerBot::FOV) * (static_cast<float>(SDK::GetLocalCanvas()->SizeY()) / 1080.0f);

	// Update FPS scale
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastAimbotFrameTime).count();
		lastAimbotFrameTime = currentTime;

		float targetFrameTime = 1.0f / 60.0f; // Targeting 60 FPS frame time

		FPSScale = targetFrameTime / deltaTime;
	}

	// Update Camera and AimbotCamera
	{
		SDK::APlayerCameraManager* CameraManager = SDK::GetLocalController()->PlayerCameraManager();
		if (SDK::IsValidPointer((uintptr_t)CameraManager)) {
			RealCamera.Position = CameraManager->GetCameraLocation();
			RealCamera.Rotation = CameraManager->GetCameraRotation();
			RealCamera.FOV = CameraManager->GetFOVAngle();

			AimbotCamera.Position = RealCamera.Position;
			AimbotCamera.FOV = RealCamera.FOV;

			if (Config::Aimbot::SilentAim) {
				if (!Config::Aimbot::UseAimKeyForSilent || Config::Aimbot::UseAimKeyForSilent && target.LocalInfo.IsTargeting) {
					AimbotCamera.Rotation = target.LocalInfo.TargetRotationWithSmooth;

					// Account for if the players actual rotation is closer than the silent aim rotation (so silent aim can't throw for you)
					float SilentPitchDistance = AimbotCamera.Rotation.GetPitchDistance(target.LocalInfo.TargetRotation);
					float SilentYawDistance = AimbotCamera.Rotation.GetYawDistance(target.LocalInfo.TargetRotation);

					float RealPitchDistance = RealCamera.Rotation.GetPitchDistance(target.LocalInfo.TargetRotation);
					float RealYawDistance = RealCamera.Rotation.GetYawDistance(target.LocalInfo.TargetRotation);

					if (RealPitchDistance < SilentPitchDistance) {
						AimbotCamera.Rotation.Pitch = RealCamera.Rotation.Pitch;
					}
					if (RealYawDistance < SilentYawDistance) {
						AimbotCamera.Rotation.Yaw = RealCamera.Rotation.Yaw;
					}
				}
			}
			else {
				AimbotCamera.Rotation = RealCamera.Rotation;
			}
		}
	}

	// Draw the aim line
	{
		if (SDK::GetLocalController()->AcknowledgedPawn()) {
			if (Config::Aimbot::ShowAimLine && Config::Aimbot::Enabled) {
				if (target.GlobalInfo.TargetActor) {
					if (target.GlobalInfo.Type == Features::Aimbot::Target::TargetType::Weakspot) {
						Drawing::Line(SDK::FVector2D(Game::ScreenWidth / 2, Game::ScreenHeight / 2), SDK::FVector2D(target.GlobalInfo.TargetBonePosition2D.X, target.GlobalInfo.TargetBonePosition2D.Y), 1.f, SDK::FLinearColor(1.f, 0.f, 0.f, 1.f), true);
					}
					else {
						Drawing::Line(SDK::FVector2D(Game::ScreenWidth / 2, Game::ScreenHeight / 2), SDK::FVector2D(target.GlobalInfo.TargetBonePosition2D.X, target.GlobalInfo.TargetBonePosition2D.Y), 1.f, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
					}
				}
			}
		}
	}

	// Draw FOV circle
	{
		if (SDK::GetLocalController()->AcknowledgedPawn()) {
			if (Config::Aimbot::ShowFOV && Config::Aimbot::Enabled) {
				if (target.GlobalInfo.Type == Features::Aimbot::Target::TargetType::ClosePlayer) {
					Drawing::Circle(SDK::FVector2D(SDK::GetLocalCanvas()->SizeX() / 2, SDK::GetLocalCanvas()->SizeY() / 2), Config::Aimbot::CloseAim::RealFOV, 32, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
				}
				else if (target.GlobalInfo.Type == Features::Aimbot::Target::TargetType::Weakspot) {
					Drawing::Circle(SDK::FVector2D(SDK::GetLocalCanvas()->SizeX() / 2, SDK::GetLocalCanvas()->SizeY() / 2), Config::Aimbot::Weakspot::RealFOV, 32, SDK::FLinearColor(1.f, 0.f, 0.f, 1.f), true);
				}
				else {
					Drawing::Circle(SDK::FVector2D(SDK::GetLocalCanvas()->SizeX() / 2, SDK::GetLocalCanvas()->SizeY() / 2), Config::Aimbot::Standard::RealFOV, 32, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
				}
			}

			if (Config::Aimbot::TriggerBot::ShowFOV && Config::Aimbot::TriggerBot::Enabled) {
				Drawing::Circle(SDK::FVector2D(SDK::GetLocalCanvas()->SizeX() / 2, SDK::GetLocalCanvas()->SizeY() / 2), Config::Aimbot::TriggerBot::RealFOV, 32, SDK::FLinearColor(0.75f, 0.25f, 0.75f, 1.f), true);
			}
		}
	}

	// Update local player
	{
		if (!SDK::GetLocalController()->AcknowledgedPawn()->GetRootComponent()) {
			localPlayer.Position = SDK::GetLocalController()->PlayerCameraManager()->GetCameraLocation();
			localPlayer.TeamIndex = -1;
		}
	}
}

void Actors::UpdateCaches() {
	if (!SDK::GetLocalCanvas()) return;

	auto CurrentTime = std::chrono::steady_clock::now();

	// Player Cache (to avoid calling GetAllActorsOfClass every tick)
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(CurrentTime - FortPawn::LastTime).count();

		if (ElapsedTime >= FortPawn::IntervalSeconds) {
			FortPawn::LastTime = CurrentTime;

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

				TempCache.push_back(FortPawnCache);
			}

			FortPawn::CachedPlayers = TempCache;
		}
	}

	// Weapon Cache (to avoid calling GetAllActorsOfClass every tick)
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(CurrentTime - FortWeapon::LastTime).count();

		if (ElapsedTime >= FortWeapon::IntervalSeconds) {
			FortWeapon::LastTime = CurrentTime;

			FortWeapon::CachedWeapons = SDK::UGameplayStatics::StaticClass()->GetAllActorsOfClass(SDK::GetWorld(), SDK::AFortPickup::StaticClass());
		}
	}

	// Weakspot Cache (to avoid calling GetAllActorsOfClass every tick)
	{
		double ElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(CurrentTime - BuildingWeakSpot::LastTime).count();

		if (ElapsedTime >= BuildingWeakSpot::IntervalSeconds) {
			BuildingWeakSpot::LastTime = CurrentTime;

			BuildingWeakSpot::CachedBuildingWeakSpot = SDK::UGameplayStatics::StaticClass()->GetAllActorsOfClass(SDK::GetWorld(), SDK::ABuildingWeakSpot::StaticClass());
		}
	}
}