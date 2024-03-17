#pragma once
#include <Windows.h>

namespace SDK {
	namespace Cached {
		namespace VFT {
			inline uintptr_t PostRender;
			inline uintptr_t ProcessEvent;
			inline uintptr_t GetPlayerViewpoint;
			inline uintptr_t GetViewpoint;
			inline uintptr_t GetWeaponStats;
		}

		namespace Offsets {
			namespace Canvas {
				inline uintptr_t SizeX;
				inline uintptr_t SizeY;
			}

			namespace Character {
				inline uintptr_t Mesh;
			}

			namespace Pawn {
				inline uintptr_t PlayerState;
			}

			namespace FortPawn {
				inline uintptr_t bIsDying;
				inline uintptr_t CurrentWeapon;
				inline uintptr_t VehicleStateLocal;
			}

			namespace Font {
				inline uintptr_t LegacyFontSize;
			}

			namespace Engine {
				inline uintptr_t GameViewport;
			}

			namespace GameViewportClient {
				inline uintptr_t World;
				inline uintptr_t GameInstance; // DO NOT DELETE! Required for getting PostRender VFT index
			}

			namespace GameInstance {
				inline uintptr_t LocalPlayers;
			}

			namespace Player {
				inline uintptr_t PlayerController;
			}

			namespace PlayerController {
				inline uintptr_t AcknowledgedPawn;
				inline uintptr_t PlayerCameraManager;
			}

			namespace HUD {
				inline uintptr_t Canvas;
			}

			namespace FortItemEntry {
				inline uintptr_t ItemDefinition;
			}

			namespace FortPickup {
				inline uintptr_t PrimaryPickupItemEntry;
			}

			namespace FortWeapon {
				inline uintptr_t WeaponData;

				inline uintptr_t LastFireTime;
				inline uintptr_t LastFireTimeVerified;
			}

			namespace FortItemDefinition {
				inline uintptr_t DisplayName;
				inline uintptr_t Tier;
			}

			namespace Actor {
				inline uintptr_t RootComponent;
			}

			namespace SceneComponent {
				inline uintptr_t RelativeLocation;
			}

			namespace FortPlayerStateAthena {
				inline uintptr_t TeamIndex;
			}

			namespace BuildingWeakSpot {
				inline uintptr_t WeakSpotInfoBitField;
			}

			namespace MinimalViewInfo {
				inline uintptr_t Location;
				inline uintptr_t Rotation;
			}

			namespace FortMeleeWeaponStats {
				inline uintptr_t SwingPlaySpeed;
			}

			namespace FortRangedWeaponStats {
				inline uintptr_t Spread;
				inline uintptr_t SpreadDownsights;
				inline uintptr_t StandingStillSpreadMultiplier;
				inline uintptr_t AthenaCrouchingSpreadMultiplier;
				inline uintptr_t AthenaJumpingFallingSpreadMultiplier;
				inline uintptr_t AthenaSprintingSpreadMultiplier;
				inline uintptr_t MinSpeedForSpreadMultiplier;
				inline uintptr_t MaxSpeedForSpreadMultiplier;

				inline uintptr_t BulletsPerCartridge;
				inline uintptr_t ReloadTime;
				inline uintptr_t RecoilVert;
				inline uintptr_t RecoilHoriz;
			}

			namespace FortAthenaAntelopeVehicle {
				inline uintptr_t FortAntelopeVehicleConfigs;
			}

			namespace FortAthenaJackalVehicle {
				inline uintptr_t BoostTimers;
			}

			namespace FortAthenaDoghouseVehicle {
				inline uintptr_t BoostAction;
			}

			namespace FortAntelopeVehicleConfigs {
				inline uintptr_t BoostAccumulationRate;
				inline uintptr_t BoostExpenseRate;
			}

			namespace VehiclePawnState {
				inline uintptr_t Vehicle;
			}

			namespace FortRechargingActionTimer {
				inline uintptr_t ChargeRate;
				inline uintptr_t ActiveExpenseRate;
				inline uintptr_t PassiveExpenseRate;
				inline uintptr_t Charge;
			}

			namespace BuildingActor {
				inline uintptr_t TeamIndex;
			}

			namespace FortPlayerController {
				inline uintptr_t bBuildFree;
				inline uintptr_t bInfiniteAmmo;
				inline uintptr_t TargetedBuilding;
			}
		}

		namespace Functions {
			namespace SceneComponent {
				inline void* SetPhysicsLinearVelocity;
			}

			namespace Actor {
				inline void* K2_TeleportTo;
				inline void* K2_SetActorRotation;
				inline void* K2_SetActorLocation;
				inline void* SetActorEnableCollision;
			}

			namespace Pawn {
				inline void* GetMovementComponent;
			}

			namespace MovementComponent {
				inline void* StopMovementImmediately;
			}

			namespace Canvas {
				inline void* K2_DrawLine;
				inline void* K2_DrawText;
				inline void* K2_TextSize;
				inline void* K2_Project;
			}

			namespace KismetSystemLibrary {
				inline void* GetEngineVersion;
				inline void* LineTraceSingle;
			}

			namespace KismetMathLibrary {
				inline void* FindLookAtRotation;
				inline void* GetForwardVector;
				inline void* GetRightVector;
			}

			namespace GameplayStatics {
				inline void* GetAllActorsOfClass;
			}

			namespace PlayerCameraManager {
				inline void* GetCameraLocation;
				inline void* GetCameraRotation;
				inline void* GetFOVAngle;
			}

			namespace PlayerController {
				inline void* WasInputKeyJustReleased;
				inline void* WasInputKeyJustPressed;
				inline void* IsInputKeyDown;
				inline void* ClientSetRotation;
				inline void* SetControlRotation;
				inline void* GetMousePosition;
			}

			namespace PlayerState {
				inline void* GetPlayerName;
			}

			namespace SkinnedMeshComponent {
				inline void* GetBoneName;
				inline void* GetSocketLocation;
			}

			namespace FortWeapon {
				inline void* IsProjectileWeapon;
				inline void* GetProjectileSpeed;
			}

			namespace FortPlayerPawn {
				inline void* ServerHandlePickup;
			}

			namespace BuildingActor {
				inline void* SetTeam;
			}

			inline uintptr_t AppendString = 0x0;
			inline uintptr_t FNameConstructor = 0x0;
			inline uintptr_t LineTraceSingle = 0x0;
		}

		namespace Masks {
			namespace FortPawn{
				inline uintptr_t bIsDying;
			}

			namespace FortPlayerController {
				inline uintptr_t bBuildFree;
				inline uintptr_t bInfiniteAmmo;
			}
		}
	}

	bool IsValidPointer(void* Address);
	uintptr_t GetBaseAddress();

	void Init();
}