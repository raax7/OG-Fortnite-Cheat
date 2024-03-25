#pragma once
#include <Windows.h>

namespace SDK {
	namespace Cached {
		inline bool UsingCalculateShot = false;

		namespace VFT {
			inline uintptr_t DrawTransition;
			inline uintptr_t ProcessEvent;
			inline uintptr_t GetPlayerViewpoint;
			inline uintptr_t GetViewpoint;
			inline uintptr_t GetWeaponStats;
		}

		namespace Offsets {
			namespace World {
				inline uintptr_t GameState = -0x1;
			}

			namespace Canvas {
				inline uintptr_t SizeX = -0x1;
				inline uintptr_t SizeY = -0x1;
			}

			namespace Character {
				inline uintptr_t Mesh = -0x1;
			}

			namespace Pawn {
				inline uintptr_t PlayerState = -0x1;
			}

			namespace FortPawn {
				inline uintptr_t bIsDying = -0x1;
				inline uintptr_t CurrentWeapon = -0x1;
			}

			namespace FortPlayerPawn {
				inline uintptr_t VehicleStateLocal = -0x1;
			}

			namespace FortPlayerPawnAthena {
				inline uintptr_t bADSWhileNotOnGround = -0x1;
			}

			namespace Font {
				inline uintptr_t LegacyFontSize = -0x1;
			}

			namespace Engine {
				inline uintptr_t GameViewport = -0x1;
				inline uintptr_t WireframeMaterial = -0x1;
			}

			namespace GameViewportClient {
				inline uintptr_t World = -0x1;
				inline uintptr_t GameInstance = -0x1; // DO NOT DELETE! Required for getting PostRender VFT index
			}

			namespace GameInstance {
				inline uintptr_t LocalPlayers = -0x1;
			}

			namespace Player {
				inline uintptr_t PlayerController = -0x1;
			}

			namespace PlayerController {
				inline uintptr_t AcknowledgedPawn = -0x1;
				inline uintptr_t PlayerCameraManager = -0x1;
			}

			namespace HUD {
				inline uintptr_t Canvas = -0x1;
			}

			namespace HitResult {
				inline uintptr_t TraceStart = -0x1;
				inline uintptr_t Distance = -0x1;
			}

			namespace FortItemEntry {
				inline uintptr_t ItemDefinition = -0x1;
			}

			namespace FortPickup {
				inline uintptr_t PrimaryPickupItemEntry = -0x1;
			}

			namespace FortWeapon {
				inline uintptr_t WeaponData = -0x1;

				inline uintptr_t LastFireTime = -0x1;
				inline uintptr_t LastFireTimeVerified = -0x1;

				inline uintptr_t bIgnoreTryToFireSlotCooldownRestriction = -0x1;
			}

			namespace FortItemDefinition {
				inline uintptr_t DisplayName = -0x1;
				inline uintptr_t Tier = -0x1;
			}

			namespace Actor {
				inline uintptr_t RootComponent = -0x1;
			}

			namespace SceneComponent {
				inline uintptr_t RelativeLocation = -0x1;
			}

			namespace FortPlayerStateAthena {
				inline uintptr_t TeamIndex = -0x1;
			}

			namespace BuildingWeakSpot {
				inline uintptr_t WeakSpotInfoBitField = -0x1;
			}

			namespace MinimalViewInfo {
				inline uintptr_t Location = -0x1;
				inline uintptr_t Rotation = -0x1;
			}

			namespace FortMeleeWeaponStats {
				inline uintptr_t SwingPlaySpeed = -0x1;
			}

			namespace FortRangedWeaponStats {
				inline uintptr_t Spread = -0x1;
				inline uintptr_t SpreadDownsights = -0x1;
				inline uintptr_t StandingStillSpreadMultiplier = -0x1;
				inline uintptr_t AthenaCrouchingSpreadMultiplier = -0x1;
				inline uintptr_t AthenaJumpingFallingSpreadMultiplier = -0x1;
				inline uintptr_t AthenaSprintingSpreadMultiplier = -0x1;
				inline uintptr_t MinSpeedForSpreadMultiplier = -0x1;
				inline uintptr_t MaxSpeedForSpreadMultiplier = -0x1;

				inline uintptr_t BulletsPerCartridge = -0x1;
				inline uintptr_t ReloadTime = -0x1;
				inline uintptr_t RecoilVert = -0x1;
				inline uintptr_t RecoilHoriz = -0x1;
			}

			namespace FortAthenaAntelopeVehicle {
				inline uintptr_t FortAntelopeVehicleConfigs = -0x1;
			}

			namespace FortAthenaJackalVehicle {
				inline uintptr_t BoostTimers = -0x1;
			}

			namespace FortAthenaDoghouseVehicle {
				inline uintptr_t BoostAction = -0x1;
			}

			namespace FortAntelopeVehicleConfigs {
				inline uintptr_t BoostAccumulationRate = -0x1;
				inline uintptr_t BoostExpenseRate = -0x1;
			}

			namespace VehiclePawnState {
				inline uintptr_t Vehicle = -0x1;
			}

			namespace FortRechargingActionTimer {
				inline uintptr_t ChargeRate = -0x1;
				inline uintptr_t ActiveExpenseRate = -0x1;
				inline uintptr_t PassiveExpenseRate = -0x1;
				inline uintptr_t Charge = -0x1;
			}

			namespace BuildingActor {
				inline uintptr_t TeamIndex = -0x1;
			}

			namespace FortPlayerController {
				inline uintptr_t bBuildFree = -0x1;
				inline uintptr_t bInfiniteAmmo = -0x1;
				inline uintptr_t TargetedBuilding = -0x1;
			}

			namespace FortGameStateAthena {
				inline uintptr_t DefaultGliderRedeployCanRedeploy = -0x1;
			}
		}

		namespace Functions {
			namespace SceneComponent {
				inline void* SetPhysicsLinearVelocity = nullptr;
			}

			namespace PrimitiveComponent {
				inline void* SetMaterial = nullptr;
			}

			namespace MeshComponent {
				inline void* GetMaterials = nullptr;
			}

			namespace Actor {
				inline void* K2_TeleportTo = nullptr;
				inline void* K2_SetActorRotation = nullptr;
				inline void* K2_SetActorLocation = nullptr;
				inline void* SetActorEnableCollision = nullptr;
			}

			namespace Pawn {
				inline void* GetMovementComponent = nullptr;
			}

			namespace MovementComponent {
				inline void* StopMovementImmediately = nullptr;
			}

			namespace Canvas {
				inline void* K2_DrawLine = nullptr;
				inline void* K2_DrawText = nullptr;
				inline void* K2_TextSize = nullptr;
				inline void* K2_Project = nullptr;
				inline void* K2_DrawBox = nullptr;
			}

			namespace KismetSystemLibrary {
				inline void* GetEngineVersion = nullptr;
				inline void* LineTraceSingle = nullptr;
			}

			namespace KismetMaterialLibrary {
				inline void* CreateDynamicMaterialInstance = nullptr;
			}

			namespace KismetMathLibrary {
				inline void* FindLookAtRotation = nullptr;
				inline void* GetForwardVector = nullptr;
				inline void* GetRightVector = nullptr;
				inline void* FMod = nullptr;
			}

			namespace GameplayStatics {
				inline void* GetAllActorsOfClass = nullptr;
			}

			namespace PlayerCameraManager {
				inline void* GetCameraLocation = nullptr;
				inline void* GetCameraRotation = nullptr;
				inline void* GetFOVAngle = nullptr;
			}

			namespace PlayerController {
				inline void* WasInputKeyJustReleased = nullptr;
				inline void* WasInputKeyJustPressed = nullptr;
				inline void* IsInputKeyDown = nullptr;
				inline void* ClientSetRotation = nullptr;
				inline void* SetControlRotation = nullptr;
				inline void* GetMousePosition = nullptr;
			}

			namespace PlayerState {
				inline void* GetPlayerName = nullptr;
			}

			namespace SkinnedMeshComponent {
				inline void* GetBoneName = nullptr;
				inline void* GetSocketLocation = nullptr;
			}

			namespace FortWeapon {
				inline void* IsProjectileWeapon = nullptr;
				inline void* GetProjectileSpeed = nullptr;
			}

			namespace FortPlayerPawn {
				inline void* ServerHandlePickup = nullptr;
			}

			namespace BuildingActor {
				inline void* SetTeam = nullptr;
			}

			inline uintptr_t AppendString = 0x0;
			inline uintptr_t FNameConstructor = 0x0;
			inline uintptr_t LineTraceSingle = 0x0;
			inline uintptr_t CalculateShot = 0x0;
			inline uintptr_t RaycastMulti = 0x0;
		}

		namespace Masks {
			namespace FortPawn{
				inline uintptr_t bIsDying = -0x1;
			}

			namespace FortPlayerController {
				inline uintptr_t bBuildFree = -0x1;
				inline uintptr_t bInfiniteAmmo = -0x1;
			}
		}
	}

	bool IsValidPointer(void* Address);
	uintptr_t GetBaseAddress();

	void Init();
}