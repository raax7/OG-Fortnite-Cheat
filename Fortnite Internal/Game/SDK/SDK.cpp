#include "SDK.h"
#include "SDKInitializer.h"

#include "../../Globals.h"

#include "../../External-Libs/skCrypter.h"

#include "../../Utilities/Logger.h"

#include "Classes/CoreUObject_classes.h"
#include "Classes/Engine_classes.h"

#include "../Input/Input.h"
#include "../Features/FortPawnHelper/Bone.h"
#include "../Features/Visuals/Chams.h"

#include "../../Configs/Config.h"

void SDK::Init() {
	DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("Initializing SDK... (")) + std::to_string(GetBaseAddress()) + std::string(skCrypt(")")));

	// Init Offsets, Functions, and VFT Indexes
	{
		// Init GObjects
		SDKInitializer::InitGObjects();

		// Init Functions
		SDKInitializer::InitAppendString();
		SDKInitializer::InitFNameConstructor();
		SDKInitializer::InitLineTraceSingle();
		SDKInitializer::InitRaycastMulti();
		SDKInitializer::InitFire();

		// Init Class Offsets
		SDK::UProperty::OffsetOffset = 0x44; // Doesn't change (as far as I know)
		SDK::UBoolProperty::ByteMaskOffset = 0x72; // Doesn't change (as far as I know)
		SDK::UClass::DefaultObjectOffset = SDKInitializer::FindDefaultObjectOffset();
		SDK::UClass::CastFlagsOffset = SDKInitializer::FindCastFlagsOffset();
		SDK::UStruct::SuperOffset = SDKInitializer::FindSuperOffset();
		SDK::UStruct::ChildPropertiesOffset = SDKInitializer::FindChildPropertiesOffset();
		SDK::UStruct::ChildrenOffset = SDKInitializer::FindChildrenOffset();
		SDK::UField::NextOffset = SDKInitializer::FindUFieldNextOffset();
		SDK::UFunction::FunctionFlagsOffset = SDKInitializer::FindFunctionFlagsOffset();

		// Init VFT Indexes
		SDKInitializer::InitPEIndex();

		// Check Game Version
		{
			SDK::Cached::Functions::KismetSystemLibrary::GetEngineVersion = SDK::UObject::FindObjectFast(std::string(skCrypt("GetEngineVersion")));

#if SEASON_20_PLUS
			if (GetGameVersion() < 20.00) {
				THROW_ERROR(
					std::string(skCrypt("Unsupported game version! (")) +
					std::to_string(GetGameVersion()) +
					std::string(skCrypt(")\nSeason 20 and onward are the only seasons supported with \"USE_DOUBLES\"!\n\nDisable \"USE_DOUBLES\" in \"Globals.h\" to play on earlier builds")),
					true);
			}
#else
			if (GetGameVersion() < 3.00 || GetGameVersion() >= 20.00) {
				THROW_ERROR(
					std::string(skCrypt("Unsupported game version! (")) +
					std::to_string(GetGameVersion()) +
					std::string(skCrypt(")\nSeason 3 to Season 19 are the only seasons supported without \"USE_DOUBLES\"\n\Enable \"USE_DOUBLES\" in \"Globals.h\" to play on later builds")),
					true);
			}
#endif

			DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("Game Version: ")) + std::to_string(GetGameVersion()));
		}

		// Init functions for EditOnRelease (only for game versions before EditOnRelease)
		if (SDK::GetGameVersion() < 11.00) {
			SDKInitializer::InitEditSelectRelease();
			SDKInitializer::InitCompleteBuildingEditInteraction();
		}

		// Init CalculateShot function offset (requires game version)
		SDKInitializer::InitCalculateShot();

		// Continue initiating VFT Indexes
		SDKInitializer::InitDTIndex();
		SDKInitializer::InitGVIndex();
		SDKInitializer::InitGPVIndex();
	}

	// Init Cached Objects
	{
		std::vector<FunctionSearch> Functions{
			FunctionSearch { std::string(skCrypt("Canvas")),				std::string(skCrypt("K2_DrawLine")),				&SDK::Cached::Functions::Canvas::K2_DrawLine							},
			FunctionSearch { std::string(skCrypt("Canvas")),				std::string(skCrypt("K2_DrawText")),				&SDK::Cached::Functions::Canvas::K2_DrawText							},
			FunctionSearch { std::string(skCrypt("Canvas")),				std::string(skCrypt("K2_TextSize")),				&SDK::Cached::Functions::Canvas::K2_TextSize							},
			FunctionSearch { std::string(skCrypt("Canvas")),				std::string(skCrypt("K2_Project")),					&SDK::Cached::Functions::Canvas::K2_Project								},
			FunctionSearch { std::string(skCrypt("Canvas")),				std::string(skCrypt("K2_DrawBox")),					&SDK::Cached::Functions::Canvas::K2_DrawBox								},
			FunctionSearch { std::string(skCrypt("GameplayStatics")),		std::string(skCrypt("GetAllActorsOfClass")),		&SDK::Cached::Functions::GameplayStatics::GetAllActorsOfClass			},
			FunctionSearch { std::string(skCrypt("PlayerCameraManager")),	std::string(skCrypt("GetCameraLocation")),			&SDK::Cached::Functions::PlayerCameraManager::GetCameraLocation			},
			FunctionSearch { std::string(skCrypt("PlayerCameraManager")),	std::string(skCrypt("GetCameraRotation")),			&SDK::Cached::Functions::PlayerCameraManager::GetCameraRotation			},
			FunctionSearch { std::string(skCrypt("PlayerCameraManager")),	std::string(skCrypt("GetFOVAngle")),				&SDK::Cached::Functions::PlayerCameraManager::GetFOVAngle				},
			FunctionSearch { std::string(skCrypt("PlayerController")),		std::string(skCrypt("IsInputKeyDown")),				&SDK::Cached::Functions::PlayerController::IsInputKeyDown				},
			FunctionSearch { std::string(skCrypt("PlayerController")),		std::string(skCrypt("WasInputKeyJustReleased")),	&SDK::Cached::Functions::PlayerController::WasInputKeyJustReleased		},
			FunctionSearch { std::string(skCrypt("PlayerController")),		std::string(skCrypt("WasInputKeyJustPressed")),		&SDK::Cached::Functions::PlayerController::WasInputKeyJustPressed		},
			FunctionSearch { std::string(skCrypt("PlayerController")),		std::string(skCrypt("GetMousePosition")),			&SDK::Cached::Functions::PlayerController::GetMousePosition				},
			FunctionSearch { std::string(skCrypt("Controller")),			std::string(skCrypt("ClientSetRotation")),			&SDK::Cached::Functions::PlayerController::ClientSetRotation			},
			FunctionSearch { std::string(skCrypt("Controller")),			std::string(skCrypt("SetControlRotation")),			&SDK::Cached::Functions::PlayerController::SetControlRotation			},
			FunctionSearch { std::string(skCrypt("KismetSystemLibrary")),	std::string(skCrypt("LineTraceSingle")),			&SDK::Cached::Functions::KismetSystemLibrary::LineTraceSingle			},
			FunctionSearch { std::string(skCrypt("KismetMaterialLibrary")),	std::string(skCrypt("CreateDynamicMaterialInstance")),&SDK::Cached::Functions::KismetMaterialLibrary::CreateDynamicMaterialInstance},
			FunctionSearch { std::string(skCrypt("KismetMathLibrary")),		std::string(skCrypt("FindLookAtRotation")),			&SDK::Cached::Functions::KismetMathLibrary::FindLookAtRotation			},
			FunctionSearch { std::string(skCrypt("KismetMathLibrary")),		std::string(skCrypt("GetForwardVector")),			&SDK::Cached::Functions::KismetMathLibrary::GetForwardVector			},
			FunctionSearch { std::string(skCrypt("KismetMathLibrary")),		std::string(skCrypt("GetRightVector")),				&SDK::Cached::Functions::KismetMathLibrary::GetRightVector				},
			FunctionSearch { std::string(skCrypt("KismetMathLibrary")),		std::string(skCrypt("FMod")),						&SDK::Cached::Functions::KismetMathLibrary::FMod						},
			FunctionSearch { std::string(skCrypt("PlayerState")),			std::string(skCrypt("GetPlayerName")),				&SDK::Cached::Functions::PlayerState::GetPlayerName						},
			FunctionSearch { std::string(skCrypt("SkinnedMeshComponent")),	std::string(skCrypt("GetBoneName")),				&SDK::Cached::Functions::SkinnedMeshComponent::GetBoneName				},
			FunctionSearch { std::string(skCrypt("SceneComponent")),		std::string(skCrypt("GetSocketLocation")),			&SDK::Cached::Functions::SkinnedMeshComponent::GetSocketLocation		},
			FunctionSearch { std::string(skCrypt("Actor")),					std::string(skCrypt("K2_TeleportTo")),				&SDK::Cached::Functions::Actor::K2_TeleportTo							},
			FunctionSearch { std::string(skCrypt("Actor")),					std::string(skCrypt("K2_SetActorRotation")),		&SDK::Cached::Functions::Actor::K2_SetActorRotation						},
			FunctionSearch { std::string(skCrypt("Actor")),					std::string(skCrypt("K2_SetActorLocation")),		&SDK::Cached::Functions::Actor::K2_SetActorLocation						},
			FunctionSearch { std::string(skCrypt("Actor")),					std::string(skCrypt("SetActorEnableCollision")),	&SDK::Cached::Functions::Actor::SetActorEnableCollision					},
			FunctionSearch { std::string(skCrypt("PrimitiveComponent")),	std::string(skCrypt("SetPhysicsLinearVelocity")),	&SDK::Cached::Functions::SceneComponent::SetPhysicsLinearVelocity		},
			FunctionSearch { std::string(skCrypt("PrimitiveComponent")),	std::string(skCrypt("CreateDynamicMaterialInstance")),&SDK::Cached::Functions::SceneComponent::CreateDynamicMaterialInstance},
			FunctionSearch { std::string(skCrypt("Pawn")),					std::string(skCrypt("GetMovementComponent")),		&SDK::Cached::Functions::Pawn::GetMovementComponent						},
			FunctionSearch { std::string(skCrypt("MovementComponent")),		std::string(skCrypt("StopMovementImmediately")),	&SDK::Cached::Functions::MovementComponent::StopMovementImmediately		},
			FunctionSearch { std::string(skCrypt("FortWeapon")),			std::string(skCrypt("IsProjectileWeapon")),			&SDK::Cached::Functions::FortWeapon::IsProjectileWeapon					},
			FunctionSearch { std::string(skCrypt("FortWeapon")),			std::string(skCrypt("GetProjectileSpeed")),			&SDK::Cached::Functions::FortWeapon::GetProjectileSpeed					},
			FunctionSearch { std::string(skCrypt("FortWeapon")),			std::string(skCrypt("GetBulletsPerClip")),			&SDK::Cached::Functions::FortWeapon::GetBulletsPerClip					},
			FunctionSearch { std::string(skCrypt("FortPlayerPawn")),		std::string(skCrypt("ServerHandlePickup")),			&SDK::Cached::Functions::FortPlayerPawn::ServerHandlePickup				},
			FunctionSearch { std::string(skCrypt("MeshComponent")),			std::string(skCrypt("GetMaterials")),				&SDK::Cached::Functions::MeshComponent::GetMaterials					},
			FunctionSearch { std::string(skCrypt("PrimitiveComponent")),	std::string(skCrypt("SetMaterial")),				&SDK::Cached::Functions::PrimitiveComponent::SetMaterial				},
			FunctionSearch { std::string(skCrypt("MaterialInstanceDynamic")),std::string(skCrypt("SetVectorParameterValue")),	&SDK::Cached::Functions::MaterialInstanceDynamic::SetVectorParameterValue},
			FunctionSearch { std::string(skCrypt("MaterialInstanceDynamic")),std::string(skCrypt("SetScalarParameterValue")),	&SDK::Cached::Functions::MaterialInstanceDynamic::SetScalarParameterValue},
			FunctionSearch { std::string(skCrypt("MaterialInterface")),		std::string(skCrypt("GetBaseMaterial")),			&SDK::Cached::Functions::MaterialInterface::GetBaseMaterial				},
		};

		std::vector<OffsetSearch> Offsets{
			OffsetSearch { std::string(skCrypt("GameViewportClient")),		std::string(skCrypt("GameInstance")),				&SDK::Cached::Offsets::GameViewportClient::GameInstance,		nullptr },
			OffsetSearch { std::string(skCrypt("Engine")),					std::string(skCrypt("GameViewport")),				&SDK::Cached::Offsets::Engine::GameViewport,					nullptr },
			OffsetSearch { std::string(skCrypt("Engine")),					std::string(skCrypt("WireframeMaterial")),			&SDK::Cached::Offsets::Engine::WireframeMaterial,				nullptr },
			OffsetSearch { std::string(skCrypt("GameViewportClient")),		std::string(skCrypt("World")),						&SDK::Cached::Offsets::GameViewportClient::World,				nullptr },
			OffsetSearch { std::string(skCrypt("GameInstance")),			std::string(skCrypt("LocalPlayers")),				&SDK::Cached::Offsets::GameInstance::LocalPlayers,				nullptr },
			OffsetSearch { std::string(skCrypt("Player")),					std::string(skCrypt("PlayerController")),			&SDK::Cached::Offsets::Player::PlayerController,				nullptr },
			OffsetSearch { std::string(skCrypt("PlayerController")),		std::string(skCrypt("AcknowledgedPawn")),			&SDK::Cached::Offsets::PlayerController::AcknowledgedPawn,		nullptr },
			OffsetSearch { std::string(skCrypt("PlayerController")),		std::string(skCrypt("PlayerCameraManager")),		&SDK::Cached::Offsets::PlayerController::PlayerCameraManager,	nullptr },
			OffsetSearch { std::string(skCrypt("FortPlayerStateZone")),		std::string(skCrypt("SpectatingTarget")),			&SDK::Cached::Offsets::FortPlayerStateZone::SpectatingTarget,	nullptr },
			OffsetSearch { std::string(skCrypt("HUD")),						std::string(skCrypt("DebugCanvas")),				&SDK::Cached::Offsets::HUD::Canvas,								nullptr },
			OffsetSearch { std::string(skCrypt("Pawn")),					std::string(skCrypt("PlayerState")),				&SDK::Cached::Offsets::Pawn::PlayerState,						nullptr },
			OffsetSearch { std::string(skCrypt("Character")),				std::string(skCrypt("Mesh")),						&SDK::Cached::Offsets::Character::Mesh,							nullptr },
			OffsetSearch { std::string(skCrypt("Font")),					std::string(skCrypt("LegacyFontSize")),				&SDK::Cached::Offsets::Font::LegacyFontSize,					nullptr },
			OffsetSearch { std::string(skCrypt("SkinnedMeshComponent")),	std::string(skCrypt("SkeletalMesh")),				&SDK::Cached::Offsets::SkeletalMeshComponent::SkeletalMesh,		nullptr },
			OffsetSearch { std::string(skCrypt("SkeletalMesh")),			std::string(skCrypt("Materials")),					&SDK::Cached::Offsets::SkeletalMesh::Materials,					nullptr },

			OffsetSearch { std::string(skCrypt("Material")),				std::string(skCrypt("bDisableDepthTest")),			&SDK::Cached::Offsets::Material::bDisableDepthTest,				&SDK::Cached::Masks::Material::bDisableDepthTest },
			OffsetSearch { std::string(skCrypt("Material")),				std::string(skCrypt("BlendMode")),					&SDK::Cached::Offsets::Material::BlendMode,						nullptr },
			OffsetSearch { std::string(skCrypt("Material")),				std::string(skCrypt("Wireframe")),					&SDK::Cached::Offsets::Material::Wireframe,						&SDK::Cached::Masks::Material::Wireframe },

			OffsetSearch { std::string(skCrypt("HitResult")),				std::string(skCrypt("TraceStart")),					&SDK::Cached::Offsets::HitResult::TraceStart,					nullptr },
			OffsetSearch { std::string(skCrypt("HitResult")),				std::string(skCrypt("Distance")),					&SDK::Cached::Offsets::HitResult::Distance,						nullptr },

			OffsetSearch { std::string(skCrypt("World")),					std::string(skCrypt("GameState")),					&SDK::Cached::Offsets::World::GameState,						nullptr },

			OffsetSearch { std::string(skCrypt("FortPickup")),				std::string(skCrypt("PrimaryPickupItemEntry")),		&SDK::Cached::Offsets::FortPickup::PrimaryPickupItemEntry,		nullptr },
			OffsetSearch { std::string(skCrypt("FortPickup")),				std::string(skCrypt("PickupEffectBlueprint")),		&SDK::Cached::Offsets::FortPickup::PickupEffectBlueprint,		nullptr },
			OffsetSearch { std::string(skCrypt("FortItemDefinition")),		std::string(skCrypt("DisplayName")),				&SDK::Cached::Offsets::FortItemDefinition::DisplayName,			nullptr },
			OffsetSearch { std::string(skCrypt("FortItemDefinition")),		std::string(skCrypt("Tier")),						&SDK::Cached::Offsets::FortItemDefinition::Tier,				nullptr },
			OffsetSearch { std::string(skCrypt("Actor")),					std::string(skCrypt("RootComponent")),				&SDK::Cached::Offsets::Actor::RootComponent,					nullptr },
			OffsetSearch { std::string(skCrypt("Actor")),					std::string(skCrypt("Role")),						&SDK::Cached::Offsets::Actor::Role,								nullptr },
			OffsetSearch { std::string(skCrypt("Actor")),					std::string(skCrypt("InstanceComponents")),			&SDK::Cached::Offsets::Actor::InstanceComponents,				nullptr },
			OffsetSearch { std::string(skCrypt("Actor")),					std::string(skCrypt("BlueprintCreatedComponents")),	&SDK::Cached::Offsets::Actor::BlueprintCreatedComponents,		nullptr },
			OffsetSearch { std::string(skCrypt("SceneComponent")),			std::string(skCrypt("RelativeLocation")),			&SDK::Cached::Offsets::SceneComponent::RelativeLocation,		nullptr },
			OffsetSearch { std::string(skCrypt("ActorComponent")),			std::string(skCrypt("ComponentTags")),				&SDK::Cached::Offsets::ActorComponent::ComponentTags,			nullptr },
			OffsetSearch { std::string(skCrypt("Canvas")),					std::string(skCrypt("SizeX")),						&SDK::Cached::Offsets::Canvas::SizeX,							nullptr },
			OffsetSearch { std::string(skCrypt("Canvas")),					std::string(skCrypt("SizeY")),						&SDK::Cached::Offsets::Canvas::SizeY,							nullptr },
			OffsetSearch { std::string(skCrypt("FortPawn")),				std::string(skCrypt("CurrentWeapon")),				&SDK::Cached::Offsets::FortPawn::CurrentWeapon,					nullptr },
			OffsetSearch { std::string(skCrypt("FortPawn")),				std::string(skCrypt("bIsDying")),					&SDK::Cached::Offsets::FortPawn::bIsDying,						&SDK::Cached::Masks::FortPawn::bIsDying },
			OffsetSearch { std::string(skCrypt("FortPlayerPawn")),			std::string(skCrypt("VehicleStateLocal")),			&SDK::Cached::Offsets::FortPlayerPawn::VehicleStateLocal,		nullptr },
			OffsetSearch { std::string(skCrypt("FortPlayerPawn")),			std::string(skCrypt("CharacterPartSkeletalMeshComponents")),&SDK::Cached::Offsets::FortPlayerPawn::CharacterPartSkeletalMeshComponents,nullptr },
			OffsetSearch { std::string(skCrypt("FortPlayerPawnAthena")),	std::string(skCrypt("bADSWhileNotOnGround")),		&SDK::Cached::Offsets::FortPlayerPawnAthena::bADSWhileNotOnGround,nullptr },
			OffsetSearch { std::string(skCrypt("FortWeapon")),				std::string(skCrypt("WeaponData")),					&SDK::Cached::Offsets::FortWeapon::WeaponData,					nullptr },
			OffsetSearch { std::string(skCrypt("FortWeapon")),				std::string(skCrypt("LastFireTime")),				&SDK::Cached::Offsets::FortWeapon::LastFireTime,				nullptr },
			OffsetSearch { std::string(skCrypt("FortWeapon")),				std::string(skCrypt("bIgnoreTryToFireSlotCooldownRestriction")), &SDK::Cached::Offsets::FortWeapon::bIgnoreTryToFireSlotCooldownRestriction, nullptr },
			OffsetSearch { std::string(skCrypt("FortWeapon")),				std::string(skCrypt("AmmoCount")),					&SDK::Cached::Offsets::FortWeapon::AmmoCount,					nullptr },
			//OffsetSearch { std::string(skCrypt("FortWeapon")),				std::string(skCrypt("AllWeaponMeshes")),			&SDK::Cached::Offsets::FortWeapon::AllWeaponMeshes,				nullptr },

			OffsetSearch { std::string(skCrypt("FortPlayerController")),	std::string(skCrypt("bBuildFree")),					&SDK::Cached::Offsets::FortPlayerController::bBuildFree,		&SDK::Cached::Masks::FortPlayerController::bBuildFree },
			OffsetSearch { std::string(skCrypt("FortPlayerController")),	std::string(skCrypt("bInfiniteAmmo")),				&SDK::Cached::Offsets::FortPlayerController::bInfiniteAmmo,		&SDK::Cached::Masks::FortPlayerController::bInfiniteAmmo },
			OffsetSearch { std::string(skCrypt("FortPlayerController")),	std::string(skCrypt("TargetedBuilding")),			&SDK::Cached::Offsets::FortPlayerController::TargetedBuilding,	nullptr },

			OffsetSearch { std::string(skCrypt("FortPlayerStateAthena")),	std::string(skCrypt("TeamIndex")),					&SDK::Cached::Offsets::FortPlayerStateAthena::TeamIndex,		nullptr },

			OffsetSearch { std::string(skCrypt("FortItemEntry")),			std::string(skCrypt("ItemDefinition")),				&SDK::Cached::Offsets::FortItemEntry::ItemDefinition,			nullptr },
			OffsetSearch { std::string(skCrypt("MinimalViewInfo")),			std::string(skCrypt("Location")),					&SDK::Cached::Offsets::MinimalViewInfo::Location,				nullptr },
			OffsetSearch { std::string(skCrypt("MinimalViewInfo")),			std::string(skCrypt("Rotation")),					&SDK::Cached::Offsets::MinimalViewInfo::Rotation,				nullptr },

			OffsetSearch { std::string(skCrypt("B_Pickups_Parent_C")),		std::string(skCrypt("Static_Mesh_Pickup")),			&SDK::Cached::Offsets::AB_Pickups_Parent_C::Static_Mesh_Pickup,	nullptr },
			OffsetSearch { std::string(skCrypt("B_Pickups_Parent_C")),		std::string(skCrypt("Skeletal_Mesh_Pickup")),		&SDK::Cached::Offsets::AB_Pickups_Parent_C::Skeletal_Mesh_Pickup,nullptr },
			OffsetSearch { std::string(skCrypt("FortPickupEffect")),		std::string(skCrypt("ItemDefinition")),				&SDK::Cached::Offsets::FortPickupEffect::ItemDefinition,		nullptr },

			OffsetSearch { std::string(skCrypt("FortMeleeWeaponStats")),	std::string(skCrypt("SwingPlaySpeed")),				&SDK::Cached::Offsets::FortMeleeWeaponStats::SwingPlaySpeed,	nullptr },

			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("Spread")),						&SDK::Cached::Offsets::FortRangedWeaponStats::Spread,			nullptr },
			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("SpreadDownsights")),			&SDK::Cached::Offsets::FortRangedWeaponStats::SpreadDownsights,	nullptr },
			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("StandingStillSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::StandingStillSpreadMultiplier, nullptr },
			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("AthenaCrouchingSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::AthenaCrouchingSpreadMultiplier, nullptr },
			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("AthenaJumpingFallingSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::AthenaJumpingFallingSpreadMultiplier, nullptr },
			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("AthenaSprintingSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::AthenaSprintingSpreadMultiplier, nullptr },
			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("MinSpeedForSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::MinSpeedForSpreadMultiplier, nullptr },
			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("MaxSpeedForSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::MaxSpeedForSpreadMultiplier, nullptr },
			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("RecoilVert")),					&SDK::Cached::Offsets::FortRangedWeaponStats::RecoilVert,		nullptr },
			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("RecoilHoriz")),				&SDK::Cached::Offsets::FortRangedWeaponStats::RecoilHoriz,		nullptr },
			OffsetSearch { std::string(skCrypt("FortRangedWeaponStats")),	std::string(skCrypt("BulletsPerCartridge")),		&SDK::Cached::Offsets::FortRangedWeaponStats::BulletsPerCartridge, nullptr },

			OffsetSearch { std::string(skCrypt("FortBaseWeaponStats")),		std::string(skCrypt("ReloadTime")),					&SDK::Cached::Offsets::FortRangedWeaponStats::ReloadTime,		nullptr },

			OffsetSearch { std::string(skCrypt("VehiclePawnState")),		std::string(skCrypt("Vehicle")),					&SDK::Cached::Offsets::VehiclePawnState::Vehicle,				nullptr },

			OffsetSearch { std::string(skCrypt("BuildingWeakSpot")),		std::string(skCrypt("bHit")),						&SDK::Cached::Offsets::BuildingWeakSpot::bHit,					&SDK::Cached::Masks::BuildingWeakSpot::bHit },
			OffsetSearch { std::string(skCrypt("BuildingWeakSpot")),		std::string(skCrypt("bFadeOut")),					&SDK::Cached::Offsets::BuildingWeakSpot::bFadeOut,				&SDK::Cached::Masks::BuildingWeakSpot::bFadeOut },
			OffsetSearch { std::string(skCrypt("BuildingWeakSpot")),		std::string(skCrypt("bActive")),					&SDK::Cached::Offsets::BuildingWeakSpot::bActive,				&SDK::Cached::Masks::BuildingWeakSpot::bActive },
		};

		if (SDK::GetGameVersion() >= 6.00) {
			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortRechargingActionTimer")), std::string(skCrypt("ChargeRate")), &SDK::Cached::Offsets::FortRechargingActionTimer::ChargeRate, nullptr });
			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortRechargingActionTimer")), std::string(skCrypt("ActiveExpenseRate")), &SDK::Cached::Offsets::FortRechargingActionTimer::ActiveExpenseRate, nullptr });
			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortRechargingActionTimer")), std::string(skCrypt("PassiveExpenseRate")), &SDK::Cached::Offsets::FortRechargingActionTimer::PassiveExpenseRate, nullptr });
			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortRechargingActionTimer")), std::string(skCrypt("Charge")), &SDK::Cached::Offsets::FortRechargingActionTimer::Charge, nullptr });

			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortAntelopeVehicleConfigs")), std::string(skCrypt("BoostAccumulationRate")), &SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostAccumulationRate, nullptr });
			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortAntelopeVehicleConfigs")), std::string(skCrypt("BoostExpenseRate")), &SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostExpenseRate, nullptr });

			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortAthenaAntelopeVehicle")), std::string(skCrypt("FortAntelopeVehicleConfigs")), &SDK::Cached::Offsets::FortAthenaAntelopeVehicle::FortAntelopeVehicleConfigs, nullptr });

			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortAthenaJackalVehicle")), std::string(skCrypt("BoostTimers")), &SDK::Cached::Offsets::FortAthenaJackalVehicle::BoostTimers, nullptr });

			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortGameStateAthena")), std::string(skCrypt("DefaultGliderRedeployCanRedeploy")), &SDK::Cached::Offsets::FortGameStateAthena::DefaultGliderRedeployCanRedeploy, nullptr });
		}

		if (SDK::GetGameVersion() >= 7.00) {
			// Bit of a hacky way to do it since its not 100% accurate if its using the enum for teams or the direct value, but they both have the same type so it doesn't matter
			Offsets.push_back(OffsetSearch{ std::string(skCrypt("BuildingActor")), std::string(skCrypt("TeamIndex")), &SDK::Cached::Offsets::BuildingActor::TeamIndex, nullptr });

			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortAthenaDoghouseVehicle")), std::string(skCrypt("BoostAction")), &SDK::Cached::Offsets::FortAthenaDoghouseVehicle::BoostAction, nullptr });

			Offsets.push_back(OffsetSearch{ std::string(skCrypt("ZiplinePawnState")), std::string(skCrypt("bIsZiplining")), &SDK::Cached::Offsets::ZiplinePawnState::bIsZiplining, nullptr });
			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortPlayerPawn")), std::string(skCrypt("ZiplineState")), &SDK::Cached::Offsets::FortPlayerPawn::ZiplineState, nullptr });
		}
		else {
			Offsets.push_back(OffsetSearch{ std::string(skCrypt("BuildingActor")), std::string(skCrypt("Team")), &SDK::Cached::Offsets::BuildingActor::TeamIndex, nullptr });
		}

		if (SDK::GetGameVersion() >= 10.00) {
			Offsets.push_back(OffsetSearch{ std::string(skCrypt("FortWeapon")), std::string(skCrypt("LastFireTimeVerified")), &SDK::Cached::Offsets::FortWeapon::LastFireTimeVerified, nullptr });
		}

		SDK::UObject::SetupObjects(Functions, Offsets);
	}

	Input::Init();
	Features::FortPawnHelper::Bone::Init();

	Features::Visuals::ChamManagerFortPawn::Manager = new Features::Visuals::ChamManagerFortPawn(Config::Visuals::Players::PawnChamSettings);
	Features::Visuals::ChamManagerFortPickup::Manager = new Features::Visuals::ChamManagerFortPickup(Config::Visuals::Weapons::PickupChamSettings);

	Features::Visuals::ChamManagerFortPawn::Manager->Init({ SDK::FName(skCrypt(L"WireFrameParameterHighlight")), SDK::FName(skCrypt(L"WireFrameFadeOffColor")) }, { SDK::FName(skCrypt(L"Emissive Modulation")) }, std::string(skCrypt("Material RezIn_Master.RezIn_Master")));
	Features::Visuals::ChamManagerFortPickup::Manager->Init({ SDK::FName(skCrypt(L"S Color1")), SDK::FName(skCrypt(L"S Color2")) }, { SDK::FName(skCrypt(L"Dissolve Pattern Emissive Brightness")), SDK::FName(skCrypt(L"Gradient Pass Emissive A")) }, std::string(skCrypt("Material CharacterShield_DimMak.CharacterShield_DimMak")));

	DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("SDK Initialized!")));

#if OBJECT_DUMP
	for (int i = 0; i < SDK::UObject::ObjectArray.Num(); i++) {
		SDK::UObject* Object = SDK::UObject::ObjectArray.GetByIndex(i);
		if (Object == nullptr) continue;

		DEBUG_LOG(std::string(skCrypt("[")) + std::to_string(i) + std::string(skCrypt("] ")) + Object->GetFullName());
	}
#endif
#if NAME_DUMP
	// this is a bit bad as it will eventually crash, due to the fact we loop until INT_MAX
	for (int i = 0; i < INT_MAX; i++) {
		SDK::FName Name;
		Name.ComparisonIndex = i;
		Name.Number = 0;

		DEBUG_LOG(std::string(skCrypt("[")) + std::to_string(i) + std::string(skCrypt("] ")) + Name.GetRawString());
	}
#endif
}

bool SDK::IsValidPointer(void* Address) {
	if (!Address) {
		return false;
	}

#ifdef USING_SEH
	__try {
		volatile auto value = *static_cast<char*>(Address);
		(void)value;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
#else
	// IMPROVVE THIS!!! IsBadWritePtr is a very bad and obsolete win api func
	if (LI_FN(IsBadReadPtr).safe_cached()(Address, 8)) {
		return false;
	}
#endif // USING_SEH

	return true;
}

uintptr_t SDK::GetBaseAddress() {
	return *(uintptr_t*)(__readgsqword(0x60) + 0x10);
}