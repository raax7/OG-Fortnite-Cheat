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
	DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("Initializing SDK... (")) + std::to_string(GetBaseAddress()) + std::string(skCrypt(" - ")) + std::to_string((uint64_t)CurrentModule) + std::string(skCrypt(")")));

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

		// Init functions for DisablePreEdits (only for game versions before DisablePreEdits)
		if (SDK::GetGameVersion() < 15.20) {
			SDKInitializer::InitPerformBuildingEditInteraction();
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
			FunctionSearch { SDK::FName(skCrypt(L"Canvas")),				SDK::FName(skCrypt(L"K2_DrawLine")),				&SDK::Cached::Functions::Canvas::K2_DrawLine							},
			FunctionSearch { SDK::FName(skCrypt(L"Canvas")),				SDK::FName(skCrypt(L"K2_DrawText")),				&SDK::Cached::Functions::Canvas::K2_DrawText							},
			FunctionSearch { SDK::FName(skCrypt(L"Canvas")),				SDK::FName(skCrypt(L"K2_TextSize")),				&SDK::Cached::Functions::Canvas::K2_TextSize							},
			FunctionSearch { SDK::FName(skCrypt(L"Canvas")),				SDK::FName(skCrypt(L"K2_Project")),					&SDK::Cached::Functions::Canvas::K2_Project								},
			FunctionSearch { SDK::FName(skCrypt(L"Canvas")),				SDK::FName(skCrypt(L"K2_DrawBox")),					&SDK::Cached::Functions::Canvas::K2_DrawBox								},
			FunctionSearch { SDK::FName(skCrypt(L"GameplayStatics")),		SDK::FName(skCrypt(L"GetAllActorsOfClass")),		&SDK::Cached::Functions::GameplayStatics::GetAllActorsOfClass			},
			FunctionSearch { SDK::FName(skCrypt(L"PlayerCameraManager")),	SDK::FName(skCrypt(L"GetCameraLocation")),			&SDK::Cached::Functions::PlayerCameraManager::GetCameraLocation			},
			FunctionSearch { SDK::FName(skCrypt(L"PlayerCameraManager")),	SDK::FName(skCrypt(L"GetCameraRotation")),			&SDK::Cached::Functions::PlayerCameraManager::GetCameraRotation			},
			FunctionSearch { SDK::FName(skCrypt(L"PlayerCameraManager")),	SDK::FName(skCrypt(L"GetFOVAngle")),				&SDK::Cached::Functions::PlayerCameraManager::GetFOVAngle				},
			FunctionSearch { SDK::FName(skCrypt(L"PlayerController")),		SDK::FName(skCrypt(L"IsInputKeyDown")),				&SDK::Cached::Functions::PlayerController::IsInputKeyDown				},
			FunctionSearch { SDK::FName(skCrypt(L"PlayerController")),		SDK::FName(skCrypt(L"WasInputKeyJustReleased")),	&SDK::Cached::Functions::PlayerController::WasInputKeyJustReleased		},
			FunctionSearch { SDK::FName(skCrypt(L"PlayerController")),		SDK::FName(skCrypt(L"WasInputKeyJustPressed")),		&SDK::Cached::Functions::PlayerController::WasInputKeyJustPressed		},
			FunctionSearch { SDK::FName(skCrypt(L"PlayerController")),		SDK::FName(skCrypt(L"GetMousePosition")),			&SDK::Cached::Functions::PlayerController::GetMousePosition				},
			FunctionSearch { SDK::FName(skCrypt(L"PlayerController")),		SDK::FName(skCrypt(L"AddYawInput")),				&SDK::Cached::Functions::PlayerController::AddYawInput					},
			FunctionSearch { SDK::FName(skCrypt(L"PlayerController")),		SDK::FName(skCrypt(L"AddPitchInput")),				&SDK::Cached::Functions::PlayerController::AddPitchInput				},
			FunctionSearch { SDK::FName(skCrypt(L"Controller")),			SDK::FName(skCrypt(L"ClientSetRotation")),			&SDK::Cached::Functions::PlayerController::ClientSetRotation			},
			FunctionSearch { SDK::FName(skCrypt(L"Controller")),			SDK::FName(skCrypt(L"SetControlRotation")),			&SDK::Cached::Functions::PlayerController::SetControlRotation			},
			FunctionSearch { SDK::FName(skCrypt(L"KismetSystemLibrary")),	SDK::FName(skCrypt(L"LineTraceSingle")),			&SDK::Cached::Functions::KismetSystemLibrary::LineTraceSingle			},
			FunctionSearch { SDK::FName(skCrypt(L"KismetMaterialLibrary")),	SDK::FName(skCrypt(L"CreateDynamicMaterialInstance")),&SDK::Cached::Functions::KismetMaterialLibrary::CreateDynamicMaterialInstance},
			FunctionSearch { SDK::FName(skCrypt(L"KismetMathLibrary")),		SDK::FName(skCrypt(L"FindLookAtRotation")),			&SDK::Cached::Functions::KismetMathLibrary::FindLookAtRotation			},
			FunctionSearch { SDK::FName(skCrypt(L"KismetMathLibrary")),		SDK::FName(skCrypt(L"GetForwardVector")),			&SDK::Cached::Functions::KismetMathLibrary::GetForwardVector			},
			FunctionSearch { SDK::FName(skCrypt(L"KismetMathLibrary")),		SDK::FName(skCrypt(L"GetRightVector")),				&SDK::Cached::Functions::KismetMathLibrary::GetRightVector				},
			FunctionSearch { SDK::FName(skCrypt(L"KismetMathLibrary")),		SDK::FName(skCrypt(L"FMod")),						&SDK::Cached::Functions::KismetMathLibrary::FMod						},
			FunctionSearch { SDK::FName(skCrypt(L"PlayerState")),			SDK::FName(skCrypt(L"GetPlayerName")),				&SDK::Cached::Functions::PlayerState::GetPlayerName						},
			FunctionSearch { SDK::FName(skCrypt(L"SkinnedMeshComponent")),	SDK::FName(skCrypt(L"GetBoneName")),				&SDK::Cached::Functions::SkinnedMeshComponent::GetBoneName				},
			FunctionSearch { SDK::FName(skCrypt(L"SceneComponent")),		SDK::FName(skCrypt(L"GetSocketLocation")),			&SDK::Cached::Functions::SkinnedMeshComponent::GetSocketLocation		},
			FunctionSearch { SDK::FName(skCrypt(L"Actor")),					SDK::FName(skCrypt(L"K2_TeleportTo")),				&SDK::Cached::Functions::Actor::K2_TeleportTo							},
			FunctionSearch { SDK::FName(skCrypt(L"Actor")),					SDK::FName(skCrypt(L"K2_SetActorRotation")),		&SDK::Cached::Functions::Actor::K2_SetActorRotation						},
			FunctionSearch { SDK::FName(skCrypt(L"Actor")),					SDK::FName(skCrypt(L"K2_SetActorLocation")),		&SDK::Cached::Functions::Actor::K2_SetActorLocation						},
			FunctionSearch { SDK::FName(skCrypt(L"Actor")),					SDK::FName(skCrypt(L"SetActorEnableCollision")),	&SDK::Cached::Functions::Actor::SetActorEnableCollision					},
			FunctionSearch { SDK::FName(skCrypt(L"PrimitiveComponent")),	SDK::FName(skCrypt(L"SetPhysicsLinearVelocity")),	&SDK::Cached::Functions::SceneComponent::SetPhysicsLinearVelocity		},
			FunctionSearch { SDK::FName(skCrypt(L"PrimitiveComponent")),	SDK::FName(skCrypt(L"CreateDynamicMaterialInstance")),&SDK::Cached::Functions::SceneComponent::CreateDynamicMaterialInstance},
			FunctionSearch { SDK::FName(skCrypt(L"Pawn")),					SDK::FName(skCrypt(L"GetMovementComponent")),		&SDK::Cached::Functions::Pawn::GetMovementComponent						},
			FunctionSearch { SDK::FName(skCrypt(L"MovementComponent")),		SDK::FName(skCrypt(L"StopMovementImmediately")),	&SDK::Cached::Functions::MovementComponent::StopMovementImmediately		},
			FunctionSearch { SDK::FName(skCrypt(L"FortWeapon")),			SDK::FName(skCrypt(L"IsProjectileWeapon")),			&SDK::Cached::Functions::FortWeapon::IsProjectileWeapon					},
			FunctionSearch { SDK::FName(skCrypt(L"FortWeapon")),			SDK::FName(skCrypt(L"GetProjectileSpeed")),			&SDK::Cached::Functions::FortWeapon::GetProjectileSpeed					},
			FunctionSearch { SDK::FName(skCrypt(L"FortWeapon")),			SDK::FName(skCrypt(L"GetBulletsPerClip")),			&SDK::Cached::Functions::FortWeapon::GetBulletsPerClip					},
			FunctionSearch { SDK::FName(skCrypt(L"FortPlayerPawn")),		SDK::FName(skCrypt(L"ServerHandlePickup")),			&SDK::Cached::Functions::FortPlayerPawn::ServerHandlePickup				},
			FunctionSearch { SDK::FName(skCrypt(L"MeshComponent")),			SDK::FName(skCrypt(L"GetMaterials")),				&SDK::Cached::Functions::MeshComponent::GetMaterials					},
			FunctionSearch { SDK::FName(skCrypt(L"PrimitiveComponent")),	SDK::FName(skCrypt(L"SetMaterial")),				&SDK::Cached::Functions::PrimitiveComponent::SetMaterial				},
			FunctionSearch { SDK::FName(skCrypt(L"MaterialInstanceDynamic")),SDK::FName(skCrypt(L"SetVectorParameterValue")),	&SDK::Cached::Functions::MaterialInstanceDynamic::SetVectorParameterValue},
			FunctionSearch { SDK::FName(skCrypt(L"MaterialInstanceDynamic")),SDK::FName(skCrypt(L"SetScalarParameterValue")),	&SDK::Cached::Functions::MaterialInstanceDynamic::SetScalarParameterValue},
			FunctionSearch { SDK::FName(skCrypt(L"MaterialInterface")),		SDK::FName(skCrypt(L"GetBaseMaterial")),			&SDK::Cached::Functions::MaterialInterface::GetBaseMaterial				},
		};
		std::vector<OffsetSearch> Offsets{
			OffsetSearch { SDK::FName(skCrypt(L"GameViewportClient")),		SDK::FName(skCrypt(L"GameInstance")),				&SDK::Cached::Offsets::GameViewportClient::GameInstance,		nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"Engine")),					SDK::FName(skCrypt(L"GameViewport")),				&SDK::Cached::Offsets::Engine::GameViewport,					nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"Engine")),					SDK::FName(skCrypt(L"WireframeMaterial")),			&SDK::Cached::Offsets::Engine::WireframeMaterial,				nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"GameViewportClient")),		SDK::FName(skCrypt(L"World")),						&SDK::Cached::Offsets::GameViewportClient::World,				nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"GameInstance")),			SDK::FName(skCrypt(L"LocalPlayers")),				&SDK::Cached::Offsets::GameInstance::LocalPlayers,				nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"Player")),					SDK::FName(skCrypt(L"PlayerController")),			&SDK::Cached::Offsets::Player::PlayerController,				nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"PlayerController")),		SDK::FName(skCrypt(L"AcknowledgedPawn")),			&SDK::Cached::Offsets::PlayerController::AcknowledgedPawn,		nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"PlayerController")),		SDK::FName(skCrypt(L"PlayerCameraManager")),		&SDK::Cached::Offsets::PlayerController::PlayerCameraManager,	nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"PlayerController")),		SDK::FName(skCrypt(L"InputYawScale")),				&SDK::Cached::Offsets::PlayerController::InputYawScale,			nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"PlayerController")),		SDK::FName(skCrypt(L"InputPitchScale")),			&SDK::Cached::Offsets::PlayerController::InputPitchScale,		nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"FortPlayerStateZone")),		SDK::FName(skCrypt(L"SpectatingTarget")),			&SDK::Cached::Offsets::FortPlayerStateZone::SpectatingTarget,	nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"HUD")),						SDK::FName(skCrypt(L"DebugCanvas")),				&SDK::Cached::Offsets::HUD::Canvas,								nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"Pawn")),					SDK::FName(skCrypt(L"PlayerState")),				&SDK::Cached::Offsets::Pawn::PlayerState,						nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"Character")),				SDK::FName(skCrypt(L"Mesh")),						&SDK::Cached::Offsets::Character::Mesh,							nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"Font")),					SDK::FName(skCrypt(L"LegacyFontSize")),				&SDK::Cached::Offsets::Font::LegacyFontSize,					nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"SkinnedMeshComponent")),	SDK::FName(skCrypt(L"SkeletalMesh")),				&SDK::Cached::Offsets::SkeletalMeshComponent::SkeletalMesh,		nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"SkeletalMesh")),			SDK::FName(skCrypt(L"Materials")),					&SDK::Cached::Offsets::SkeletalMesh::Materials,					nullptr },

			OffsetSearch { SDK::FName(skCrypt(L"Material")),				SDK::FName(skCrypt(L"bDisableDepthTest")),			&SDK::Cached::Offsets::Material::bDisableDepthTest,				&SDK::Cached::Masks::Material::bDisableDepthTest },
			OffsetSearch { SDK::FName(skCrypt(L"Material")),				SDK::FName(skCrypt(L"BlendMode")),					&SDK::Cached::Offsets::Material::BlendMode,						nullptr },
			OffsetSearch { SDK::FName(skCrypt(L"Material")),				SDK::FName(skCrypt(L"Wireframe")),					&SDK::Cached::Offsets::Material::Wireframe,						&SDK::Cached::Masks::Material::Wireframe },

			OffsetSearch { SDK::FName(skCrypt(L"HitResult")),				SDK::FName(skCrypt(L"TraceStart")),					&SDK::Cached::Offsets::HitResult::TraceStart,					nullptr },
			OffsetSearch{ SDK::FName(skCrypt(L"HitResult")),				SDK::FName(skCrypt(L"Distance")),					&SDK::Cached::Offsets::HitResult::Distance,						nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"World")),					SDK::FName(skCrypt(L"GameState")),					&SDK::Cached::Offsets::World::GameState,						nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"FortPickup")),				SDK::FName(skCrypt(L"PrimaryPickupItemEntry")),		&SDK::Cached::Offsets::FortPickup::PrimaryPickupItemEntry,		nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortPickup")),				SDK::FName(skCrypt(L"PickupEffectBlueprint")),		&SDK::Cached::Offsets::FortPickup::PickupEffectBlueprint,		nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortItemDefinition")),		SDK::FName(skCrypt(L"DisplayName")),				&SDK::Cached::Offsets::FortItemDefinition::DisplayName,			nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortItemDefinition")),		SDK::FName(skCrypt(L"Tier")),						&SDK::Cached::Offsets::FortItemDefinition::Tier,				nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"Actor")),					SDK::FName(skCrypt(L"RootComponent")),				&SDK::Cached::Offsets::Actor::RootComponent,					nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"Actor")),					SDK::FName(skCrypt(L"Role")),						&SDK::Cached::Offsets::Actor::Role,								nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"Actor")),					SDK::FName(skCrypt(L"InstanceComponents")),			&SDK::Cached::Offsets::Actor::InstanceComponents,				nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"Actor")),					SDK::FName(skCrypt(L"BlueprintCreatedComponents")),	&SDK::Cached::Offsets::Actor::BlueprintCreatedComponents,		nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"SceneComponent")),			SDK::FName(skCrypt(L"RelativeLocation")),			&SDK::Cached::Offsets::SceneComponent::RelativeLocation,		nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"ActorComponent")),			SDK::FName(skCrypt(L"ComponentTags")),				&SDK::Cached::Offsets::ActorComponent::ComponentTags,			nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"Canvas")),					SDK::FName(skCrypt(L"SizeX")),						&SDK::Cached::Offsets::Canvas::SizeX,							nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"Canvas")),					SDK::FName(skCrypt(L"SizeY")),						&SDK::Cached::Offsets::Canvas::SizeY,							nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortPawn")),				SDK::FName(skCrypt(L"CurrentWeapon")),				&SDK::Cached::Offsets::FortPawn::CurrentWeapon,					nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortPawn")),				SDK::FName(skCrypt(L"bIsDying")),					&SDK::Cached::Offsets::FortPawn::bIsDying,						&SDK::Cached::Masks::FortPawn::bIsDying },
				OffsetSearch{ SDK::FName(skCrypt(L"FortPlayerPawn")),			SDK::FName(skCrypt(L"VehicleStateLocal")),			&SDK::Cached::Offsets::FortPlayerPawn::VehicleStateLocal,		nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortPlayerPawn")),			SDK::FName(skCrypt(L"CharacterPartSkeletalMeshComponents")),&SDK::Cached::Offsets::FortPlayerPawn::CharacterPartSkeletalMeshComponents,nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortPlayerPawnAthena")),	SDK::FName(skCrypt(L"bADSWhileNotOnGround")),		&SDK::Cached::Offsets::FortPlayerPawnAthena::bADSWhileNotOnGround,nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortWeapon")),				SDK::FName(skCrypt(L"WeaponData")),					&SDK::Cached::Offsets::FortWeapon::WeaponData,					nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortWeapon")),				SDK::FName(skCrypt(L"LastFireTime")),				&SDK::Cached::Offsets::FortWeapon::LastFireTime,				nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortWeapon")),				SDK::FName(skCrypt(L"bIgnoreTryToFireSlotCooldownRestriction")), &SDK::Cached::Offsets::FortWeapon::bIgnoreTryToFireSlotCooldownRestriction, nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortWeapon")),				SDK::FName(skCrypt(L"AmmoCount")),					&SDK::Cached::Offsets::FortWeapon::AmmoCount,					nullptr },
				//OffsetSearch { SDK::FName(skCrypt(L"FortWeapon")),				SDK::FName(skCrypt(L"AllWeaponMeshes")),			&SDK::Cached::Offsets::FortWeapon::AllWeaponMeshes,				nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"FortPlayerController")),	SDK::FName(skCrypt(L"bBuildFree")),					&SDK::Cached::Offsets::FortPlayerController::bBuildFree,		&SDK::Cached::Masks::FortPlayerController::bBuildFree },
				OffsetSearch{ SDK::FName(skCrypt(L"FortPlayerController")),	SDK::FName(skCrypt(L"bInfiniteAmmo")),				&SDK::Cached::Offsets::FortPlayerController::bInfiniteAmmo,		&SDK::Cached::Masks::FortPlayerController::bInfiniteAmmo },
				OffsetSearch{ SDK::FName(skCrypt(L"FortPlayerController")),	SDK::FName(skCrypt(L"TargetedBuilding")),			&SDK::Cached::Offsets::FortPlayerController::TargetedBuilding,	nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"FortPlayerStateAthena")),	SDK::FName(skCrypt(L"TeamIndex")),					&SDK::Cached::Offsets::FortPlayerStateAthena::TeamIndex,		nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"FortItemEntry")),			SDK::FName(skCrypt(L"ItemDefinition")),				&SDK::Cached::Offsets::FortItemEntry::ItemDefinition,			nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"MinimalViewInfo")),			SDK::FName(skCrypt(L"Location")),					&SDK::Cached::Offsets::MinimalViewInfo::Location,				nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"MinimalViewInfo")),			SDK::FName(skCrypt(L"Rotation")),					&SDK::Cached::Offsets::MinimalViewInfo::Rotation,				nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"B_Pickups_Parent_C")),		SDK::FName(skCrypt(L"Static_Mesh_Pickup")),			&SDK::Cached::Offsets::AB_Pickups_Parent_C::Static_Mesh_Pickup,	nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"B_Pickups_Parent_C")),		SDK::FName(skCrypt(L"Skeletal_Mesh_Pickup")),		&SDK::Cached::Offsets::AB_Pickups_Parent_C::Skeletal_Mesh_Pickup,nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortPickupEffect")),		SDK::FName(skCrypt(L"ItemDefinition")),				&SDK::Cached::Offsets::FortPickupEffect::ItemDefinition,		nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"FortMeleeWeaponStats")),	SDK::FName(skCrypt(L"SwingPlaySpeed")),				&SDK::Cached::Offsets::FortMeleeWeaponStats::SwingPlaySpeed,	nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"Spread")),						&SDK::Cached::Offsets::FortRangedWeaponStats::Spread,			nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"SpreadDownsights")),			&SDK::Cached::Offsets::FortRangedWeaponStats::SpreadDownsights,	nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"StandingStillSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::StandingStillSpreadMultiplier, nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"AthenaCrouchingSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::AthenaCrouchingSpreadMultiplier, nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"AthenaJumpingFallingSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::AthenaJumpingFallingSpreadMultiplier, nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"AthenaSprintingSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::AthenaSprintingSpreadMultiplier, nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"MinSpeedForSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::MinSpeedForSpreadMultiplier, nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"MaxSpeedForSpreadMultiplier")), &SDK::Cached::Offsets::FortRangedWeaponStats::MaxSpeedForSpreadMultiplier, nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"RecoilVert")),					&SDK::Cached::Offsets::FortRangedWeaponStats::RecoilVert,		nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"RecoilHoriz")),				&SDK::Cached::Offsets::FortRangedWeaponStats::RecoilHoriz,		nullptr },
				OffsetSearch{ SDK::FName(skCrypt(L"FortRangedWeaponStats")),	SDK::FName(skCrypt(L"BulletsPerCartridge")),		&SDK::Cached::Offsets::FortRangedWeaponStats::BulletsPerCartridge, nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"FortBaseWeaponStats")),		SDK::FName(skCrypt(L"ReloadTime")),					&SDK::Cached::Offsets::FortRangedWeaponStats::ReloadTime,		nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"VehiclePawnState")),		SDK::FName(skCrypt(L"Vehicle")),					&SDK::Cached::Offsets::VehiclePawnState::Vehicle,				nullptr },

				OffsetSearch{ SDK::FName(skCrypt(L"BuildingWeakSpot")),		SDK::FName(skCrypt(L"bHit")),						&SDK::Cached::Offsets::BuildingWeakSpot::bHit,					&SDK::Cached::Masks::BuildingWeakSpot::bHit },
				OffsetSearch{ SDK::FName(skCrypt(L"BuildingWeakSpot")),		SDK::FName(skCrypt(L"bFadeOut")),					&SDK::Cached::Offsets::BuildingWeakSpot::bFadeOut,				&SDK::Cached::Masks::BuildingWeakSpot::bFadeOut },
				OffsetSearch{ SDK::FName(skCrypt(L"BuildingWeakSpot")),		SDK::FName(skCrypt(L"bActive")),					&SDK::Cached::Offsets::BuildingWeakSpot::bActive,				&SDK::Cached::Masks::BuildingWeakSpot::bActive },
		};

		if (SDK::GetGameVersion() >= 6.00) {
			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortRechargingActionTimer")), SDK::FName(skCrypt(L"ChargeRate")), &SDK::Cached::Offsets::FortRechargingActionTimer::ChargeRate, nullptr });
			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortRechargingActionTimer")), SDK::FName(skCrypt(L"ActiveExpenseRate")), &SDK::Cached::Offsets::FortRechargingActionTimer::ActiveExpenseRate, nullptr });
			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortRechargingActionTimer")), SDK::FName(skCrypt(L"PassiveExpenseRate")), &SDK::Cached::Offsets::FortRechargingActionTimer::PassiveExpenseRate, nullptr });
			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortRechargingActionTimer")), SDK::FName(skCrypt(L"Charge")), &SDK::Cached::Offsets::FortRechargingActionTimer::Charge, nullptr });

			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortAntelopeVehicleConfigs")), SDK::FName(skCrypt(L"BoostAccumulationRate")), &SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostAccumulationRate, nullptr });
			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortAntelopeVehicleConfigs")), SDK::FName(skCrypt(L"BoostExpenseRate")), &SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostExpenseRate, nullptr });

			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortAthenaAntelopeVehicle")), SDK::FName(skCrypt(L"FortAntelopeVehicleConfigs")), &SDK::Cached::Offsets::FortAthenaAntelopeVehicle::FortAntelopeVehicleConfigs, nullptr });

			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortAthenaJackalVehicle")), SDK::FName(skCrypt(L"BoostTimers")), &SDK::Cached::Offsets::FortAthenaJackalVehicle::BoostTimers, nullptr });

			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortGameStateAthena")), SDK::FName(skCrypt(L"DefaultGliderRedeployCanRedeploy")), &SDK::Cached::Offsets::FortGameStateAthena::DefaultGliderRedeployCanRedeploy, nullptr });
		}
		if (SDK::GetGameVersion() >= 7.00) {
			// Bit of a hacky way to do it since its not 100% accurate if its using the enum for teams or the direct value, but they both have the same type so it doesn't matter
			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"BuildingActor")), SDK::FName(skCrypt(L"TeamIndex")), &SDK::Cached::Offsets::BuildingActor::TeamIndex, nullptr });

			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortAthenaDoghouseVehicle")), SDK::FName(skCrypt(L"BoostAction")), &SDK::Cached::Offsets::FortAthenaDoghouseVehicle::BoostAction, nullptr });

			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"ZiplinePawnState")), SDK::FName(skCrypt(L"bIsZiplining")), &SDK::Cached::Offsets::ZiplinePawnState::bIsZiplining, nullptr });
			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortPlayerPawn")), SDK::FName(skCrypt(L"ZiplineState")), &SDK::Cached::Offsets::FortPlayerPawn::ZiplineState, nullptr });
		}
		else {
			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"BuildingActor")), SDK::FName(skCrypt(L"Team")), &SDK::Cached::Offsets::BuildingActor::TeamIndex, nullptr });
		}
		if (SDK::GetGameVersion() >= 10.00) {
			Offsets.push_back(OffsetSearch{ SDK::FName(skCrypt(L"FortWeapon")), SDK::FName(skCrypt(L"LastFireTimeVerified")), &SDK::Cached::Offsets::FortWeapon::LastFireTimeVerified, nullptr });
		}

		SDK::UObject::SetupObjects(Functions, Offsets);
	}

	// Init Features
	{
		Input::Init();
		Features::FortPawnHelper::Bone::Init();

		Features::Visuals::ChamManagerFortPawn::Manager = new Features::Visuals::ChamManagerFortPawn(Config::Visuals::Players::PawnChamSettings);
		Features::Visuals::ChamManagerFortPickup::Manager = new Features::Visuals::ChamManagerFortPickup(Config::Visuals::Weapons::PickupChamSettings);

		Features::Visuals::ChamManagerFortPawn::Manager->Init({ SDK::FName(skCrypt(L"WireFrameParameterHighlight")), SDK::FName(skCrypt(L"WireFrameFadeOffColor")), SDK::FName(skCrypt(L"Top Color")), SDK::FName(skCrypt(L"Bottom Color")) }, { SDK::FName(skCrypt(L"Emissive Modulation")) }, std::string(skCrypt("Material RezIn_Master.RezIn_Master")));
		Features::Visuals::ChamManagerFortPickup::Manager->Init({ SDK::FName(skCrypt(L"S Color1")), SDK::FName(skCrypt(L"S Color2")) }, { SDK::FName(skCrypt(L"Dissolve Pattern Emissive Brightness")), SDK::FName(skCrypt(L"Gradient Pass Emissive A")) }, std::string(skCrypt("Material CharacterShield_DimMak.CharacterShield_DimMak")));
	}

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

#if USING_SEH
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