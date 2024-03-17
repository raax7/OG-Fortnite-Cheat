#include "SDK.h"
#include "SDKInitializer.h"

#include "../../Globals.h"

#include "../../External-Libs/skCrypter.h"

#include "../../Utilities/Logger.h"

#include "Classes/CoreUObject_classes.h"
#include "Classes/Engine_classes.h"

#include "../Input/Input.h"
#include "../Features/FortPawnHelper/Bone.h"

void SDK::Init() {
	DEBUG_LOG(LOG_OFFSET, skCrypt("Initializing SDK...").decrypt());

	// Init Offsets, Functions, and VFT Indexes
	{
		// Init GObjects
		SDKInitializer::InitGObjects();

		// Init Functions
		SDKInitializer::InitAppendString();
		SDKInitializer::InitFNameConstructor();
		SDKInitializer::InitLineTraceSingle();

		// Init Class Offsets
		SDK::UProperty::OffsetOffset		= 0x44; // Doesn't change (as far as I know)
		SDK::UBoolProperty::ByteMaskOffset	= 0x72; // Doesn't change (as far as I know)
		SDK::UClass::DefaultObjectOffset	= SDKInitializer::FindDefaultObjectOffset();
		SDK::UClass::CastFlagsOffset		= SDKInitializer::FindCastFlagsOffset();
		SDK::UStruct::SuperOffset			= SDKInitializer::FindSuperOffset();
		SDK::UStruct::ChildPropertiesOffset = SDKInitializer::FindChildPropertiesOffset();
		SDK::UStruct::ChildrenOffset		= SDKInitializer::FindChildrenOffset();
		SDK::UField::NextOffset 			= SDKInitializer::FindUFieldNextOffset();
		SDK::UFunction::FunctionFlagsOffset	= SDKInitializer::FindFunctionFlagsOffset();

		// Init VFT Indexes
		SDKInitializer::InitPEIndex();

		// Check Game Version
		{
			SDK::Cached::Functions::KismetSystemLibrary::GetEngineVersion = SDK::UObject::FindObjectFast(skCrypt("GetEngineVersion").decrypt());

			if (GetGameVersion() < 3.00 || GetGameVersion() >= 20.00) {
				THROW_ERROR(
					skCrypt("Unsupported game version! (").decrypt() +
					std::to_string(GetGameVersion()) +
					skCrypt(")\nSeason 3 to Season 15 are the only seasons supported currently").decrypt(),
					true);
			}

			DEBUG_LOG(LOG_OFFSET, skCrypt("Game Version: ").decrypt() + std::to_string(GetGameVersion()));
		}

		// Continue VFT Indexes
		SDKInitializer::InitPRIndex();
		SDKInitializer::InitGVIndex();
		SDKInitializer::InitGPVIndex();
	}

	// Init Cached Objects
	{
		std::vector<FunctionSearch> Functions {
			FunctionSearch { skCrypt("Canvas").decrypt(),				skCrypt("K2_DrawLine").decrypt(),				&SDK::Cached::Functions::Canvas::K2_DrawLine							},
			FunctionSearch { skCrypt("Canvas").decrypt(),				skCrypt("K2_DrawText").decrypt(),				&SDK::Cached::Functions::Canvas::K2_DrawText							},
			FunctionSearch { skCrypt("Canvas").decrypt(),				skCrypt("K2_TextSize").decrypt(),				&SDK::Cached::Functions::Canvas::K2_TextSize							},
			FunctionSearch { skCrypt("Canvas").decrypt(),				skCrypt("K2_Project").decrypt(),				&SDK::Cached::Functions::Canvas::K2_Project								},
			FunctionSearch { skCrypt("GameplayStatics").decrypt(),		skCrypt("GetAllActorsOfClass").decrypt(),		&SDK::Cached::Functions::GameplayStatics::GetAllActorsOfClass			},
			FunctionSearch { skCrypt("PlayerCameraManager").decrypt(),	skCrypt("GetCameraLocation").decrypt(),			&SDK::Cached::Functions::PlayerCameraManager::GetCameraLocation			},
			FunctionSearch { skCrypt("PlayerCameraManager").decrypt(),	skCrypt("GetCameraRotation").decrypt(),			&SDK::Cached::Functions::PlayerCameraManager::GetCameraRotation			},
			FunctionSearch { skCrypt("PlayerCameraManager").decrypt(),	skCrypt("GetFOVAngle").decrypt(),				&SDK::Cached::Functions::PlayerCameraManager::GetFOVAngle				},
			FunctionSearch { skCrypt("PlayerController").decrypt(),		skCrypt("IsInputKeyDown").decrypt(),			&SDK::Cached::Functions::PlayerController::IsInputKeyDown				},
			FunctionSearch { skCrypt("PlayerController").decrypt(),		skCrypt("WasInputKeyJustReleased").decrypt(),	&SDK::Cached::Functions::PlayerController::WasInputKeyJustReleased		},
			FunctionSearch { skCrypt("PlayerController").decrypt(),		skCrypt("WasInputKeyJustPressed").decrypt(),	&SDK::Cached::Functions::PlayerController::WasInputKeyJustPressed		},
			FunctionSearch { skCrypt("PlayerController").decrypt(),		skCrypt("GetMousePosition").decrypt(),			&SDK::Cached::Functions::PlayerController::GetMousePosition				},
			FunctionSearch { skCrypt("Controller").decrypt(),			skCrypt("ClientSetRotation").decrypt(),			&SDK::Cached::Functions::PlayerController::ClientSetRotation			},
			FunctionSearch { skCrypt("Controller").decrypt(),			skCrypt("SetControlRotation").decrypt(),		&SDK::Cached::Functions::PlayerController::SetControlRotation			},
			FunctionSearch { skCrypt("KismetSystemLibrary").decrypt(),	skCrypt("LineTraceSingle").decrypt(),			&SDK::Cached::Functions::KismetSystemLibrary::LineTraceSingle			},
			FunctionSearch { skCrypt("KismetMathLibrary").decrypt(),	skCrypt("FindLookAtRotation").decrypt(),		&SDK::Cached::Functions::KismetMathLibrary::FindLookAtRotation			},
			FunctionSearch { skCrypt("KismetMathLibrary").decrypt(),	skCrypt("GetForwardVector").decrypt(),			&SDK::Cached::Functions::KismetMathLibrary::GetForwardVector			},
			FunctionSearch { skCrypt("KismetMathLibrary").decrypt(),	skCrypt("GetRightVector").decrypt(),			&SDK::Cached::Functions::KismetMathLibrary::GetRightVector				},
			FunctionSearch { skCrypt("PlayerState").decrypt(),			skCrypt("GetPlayerName").decrypt(),				&SDK::Cached::Functions::PlayerState::GetPlayerName						},
			FunctionSearch { skCrypt("SkinnedMeshComponent").decrypt(),	skCrypt("GetBoneName").decrypt(),				&SDK::Cached::Functions::SkinnedMeshComponent::GetBoneName				},
			FunctionSearch { skCrypt("SceneComponent").decrypt(),		skCrypt("GetSocketLocation").decrypt(),			&SDK::Cached::Functions::SkinnedMeshComponent::GetSocketLocation		},
			FunctionSearch { skCrypt("Actor").decrypt(),				skCrypt("K2_TeleportTo").decrypt(),				&SDK::Cached::Functions::Actor::K2_TeleportTo							},
			FunctionSearch { skCrypt("Actor").decrypt(),				skCrypt("K2_SetActorRotation").decrypt(),		&SDK::Cached::Functions::Actor::K2_SetActorRotation						},
			FunctionSearch { skCrypt("Actor").decrypt(),				skCrypt("K2_SetActorLocation").decrypt(),		&SDK::Cached::Functions::Actor::K2_SetActorLocation						},
			FunctionSearch { skCrypt("Actor").decrypt(),				skCrypt("SetActorEnableCollision").decrypt(),	&SDK::Cached::Functions::Actor::SetActorEnableCollision					},
			FunctionSearch { skCrypt("PrimitiveComponent").decrypt(),	skCrypt("SetPhysicsLinearVelocity").decrypt(),	&SDK::Cached::Functions::SceneComponent::SetPhysicsLinearVelocity		},
			FunctionSearch { skCrypt("Pawn").decrypt(),					skCrypt("GetMovementComponent").decrypt(),		&SDK::Cached::Functions::Pawn::GetMovementComponent						},
			FunctionSearch { skCrypt("MovementComponent").decrypt(),	skCrypt("StopMovementImmediately").decrypt(),	&SDK::Cached::Functions::MovementComponent::StopMovementImmediately		},
			FunctionSearch { skCrypt("FortWeapon").decrypt(),			skCrypt("IsProjectileWeapon").decrypt(),		&SDK::Cached::Functions::FortWeapon::IsProjectileWeapon					},
			FunctionSearch { skCrypt("FortWeapon").decrypt(),			skCrypt("GetProjectileSpeed").decrypt(),		&SDK::Cached::Functions::FortWeapon::GetProjectileSpeed					},
			FunctionSearch { skCrypt("FortPlayerPawn").decrypt(),		skCrypt("ServerHandlePickup").decrypt(),		&SDK::Cached::Functions::FortPlayerPawn::ServerHandlePickup				},
		};

		std::vector<OffsetSearch> Offsets {
			// REQUIRED FOR INITIALIZATION
			OffsetSearch { skCrypt("GameViewportClient").decrypt(),		skCrypt("GameInstance").decrypt(),				&SDK::Cached::Offsets::GameViewportClient::GameInstance,		nullptr },
			// REQUIRED FOR INITIALIZATION

			OffsetSearch { skCrypt("Engine").decrypt(),					skCrypt("GameViewport").decrypt(),				&SDK::Cached::Offsets::Engine::GameViewport,					nullptr },
			OffsetSearch { skCrypt("GameViewportClient").decrypt(),		skCrypt("World").decrypt(),						&SDK::Cached::Offsets::GameViewportClient::World,				nullptr },
			OffsetSearch { skCrypt("GameInstance").decrypt(),			skCrypt("LocalPlayers").decrypt(),				&SDK::Cached::Offsets::GameInstance::LocalPlayers,				nullptr },
			OffsetSearch { skCrypt("Player").decrypt(),					skCrypt("PlayerController").decrypt(),			&SDK::Cached::Offsets::Player::PlayerController,				nullptr },
			OffsetSearch { skCrypt("PlayerController").decrypt(),		skCrypt("AcknowledgedPawn").decrypt(),			&SDK::Cached::Offsets::PlayerController::AcknowledgedPawn,		nullptr },
			OffsetSearch { skCrypt("PlayerController").decrypt(),		skCrypt("PlayerCameraManager").decrypt(),		&SDK::Cached::Offsets::PlayerController::PlayerCameraManager,	nullptr },
			OffsetSearch { skCrypt("HUD").decrypt(),					skCrypt("DebugCanvas").decrypt(),				&SDK::Cached::Offsets::HUD::Canvas,								nullptr },
			OffsetSearch { skCrypt("Pawn").decrypt(),					skCrypt("PlayerState").decrypt(),				&SDK::Cached::Offsets::Pawn::PlayerState,						nullptr },
			OffsetSearch { skCrypt("Character").decrypt(),				skCrypt("Mesh").decrypt(),						&SDK::Cached::Offsets::Character::Mesh,							nullptr },
			OffsetSearch { skCrypt("Font").decrypt(),					skCrypt("LegacyFontSize").decrypt(),			&SDK::Cached::Offsets::Font::LegacyFontSize,					nullptr },
			
			OffsetSearch { skCrypt("FortPickup").decrypt(),				skCrypt("PrimaryPickupItemEntry").decrypt(),	&SDK::Cached::Offsets::FortPickup::PrimaryPickupItemEntry,		nullptr },
			OffsetSearch { skCrypt("FortItemDefinition").decrypt(),		skCrypt("DisplayName").decrypt(),				&SDK::Cached::Offsets::FortItemDefinition::DisplayName,			nullptr },
			OffsetSearch { skCrypt("FortItemDefinition").decrypt(),		skCrypt("Tier").decrypt(),						&SDK::Cached::Offsets::FortItemDefinition::Tier,				nullptr },
			OffsetSearch { skCrypt("Actor").decrypt(),					skCrypt("RootComponent").decrypt(),				&SDK::Cached::Offsets::Actor::RootComponent,					nullptr },
			OffsetSearch { skCrypt("SceneComponent").decrypt(),			skCrypt("RelativeLocation").decrypt(),			&SDK::Cached::Offsets::SceneComponent::RelativeLocation,		nullptr },
			OffsetSearch { skCrypt("Canvas").decrypt(),					skCrypt("SizeX").decrypt(),						&SDK::Cached::Offsets::Canvas::SizeX,							nullptr },
			OffsetSearch { skCrypt("Canvas").decrypt(),					skCrypt("SizeY").decrypt(),						&SDK::Cached::Offsets::Canvas::SizeY,							nullptr },
			OffsetSearch { skCrypt("FortPawn").decrypt(),				skCrypt("CurrentWeapon").decrypt(),				&SDK::Cached::Offsets::FortPawn::CurrentWeapon,					nullptr },
			OffsetSearch { skCrypt("FortPawn").decrypt(),				skCrypt("bIsDying").decrypt(),					&SDK::Cached::Offsets::FortPawn::bIsDying,						&SDK::Cached::Masks::FortPawn::bIsDying },
			OffsetSearch { skCrypt("FortPlayerPawn").decrypt(),			skCrypt("VehicleStateLocal").decrypt(),			&SDK::Cached::Offsets::FortPawn::VehicleStateLocal,				nullptr },
			OffsetSearch { skCrypt("BuildingWeakSpot").decrypt(),		skCrypt("bHit").decrypt(),						&SDK::Cached::Offsets::BuildingWeakSpot::WeakSpotInfoBitField,	nullptr },
			OffsetSearch { skCrypt("FortWeapon").decrypt(),				skCrypt("WeaponData").decrypt(),				&SDK::Cached::Offsets::FortWeapon::WeaponData,					nullptr },
			OffsetSearch { skCrypt("FortWeapon").decrypt(),				skCrypt("LastFireTime").decrypt(),				&SDK::Cached::Offsets::FortWeapon::LastFireTime,				nullptr },
			
			OffsetSearch { skCrypt("FortPlayerController").decrypt(),	skCrypt("bBuildFree").decrypt(),				&SDK::Cached::Offsets::FortPlayerController::bBuildFree,		&SDK::Cached::Masks::FortPlayerController::bBuildFree },
			OffsetSearch { skCrypt("FortPlayerController").decrypt(),	skCrypt("bInfiniteAmmo").decrypt(),				&SDK::Cached::Offsets::FortPlayerController::bInfiniteAmmo,		&SDK::Cached::Masks::FortPlayerController::bInfiniteAmmo },
			OffsetSearch { skCrypt("FortPlayerController").decrypt(),	skCrypt("TargetedBuilding").decrypt(),			&SDK::Cached::Offsets::FortPlayerController::TargetedBuilding,	nullptr },

			OffsetSearch { skCrypt("FortPlayerStateAthena").decrypt(),	skCrypt("TeamIndex").decrypt(),					&SDK::Cached::Offsets::FortPlayerStateAthena::TeamIndex,		nullptr },

			OffsetSearch { skCrypt("FortItemEntry").decrypt(),			skCrypt("ItemDefinition").decrypt(),			&SDK::Cached::Offsets::FortItemEntry::ItemDefinition,			nullptr },
			OffsetSearch { skCrypt("MinimalViewInfo").decrypt(),		skCrypt("Location").decrypt(),					&SDK::Cached::Offsets::MinimalViewInfo::Location,				nullptr },
			OffsetSearch { skCrypt("MinimalViewInfo").decrypt(),		skCrypt("Rotation").decrypt(),					&SDK::Cached::Offsets::MinimalViewInfo::Rotation,				nullptr },
		
			OffsetSearch { skCrypt("FortMeleeWeaponStats").decrypt(),	skCrypt("SwingPlaySpeed").decrypt(),			&SDK::Cached::Offsets::FortMeleeWeaponStats::SwingPlaySpeed,	nullptr },
			
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("Spread").decrypt(),					&SDK::Cached::Offsets::FortRangedWeaponStats::Spread,			nullptr },
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("SpreadDownsights").decrypt(),			&SDK::Cached::Offsets::FortRangedWeaponStats::SpreadDownsights,	nullptr },
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("StandingStillSpreadMultiplier").decrypt(),	&SDK::Cached::Offsets::FortRangedWeaponStats::StandingStillSpreadMultiplier, nullptr },
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("AthenaCrouchingSpreadMultiplier").decrypt(), &SDK::Cached::Offsets::FortRangedWeaponStats::AthenaCrouchingSpreadMultiplier, nullptr },
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("AthenaJumpingFallingSpreadMultiplier").decrypt(), &SDK::Cached::Offsets::FortRangedWeaponStats::AthenaJumpingFallingSpreadMultiplier, nullptr },
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("AthenaSprintingSpreadMultiplier").decrypt(), &SDK::Cached::Offsets::FortRangedWeaponStats::AthenaSprintingSpreadMultiplier, nullptr },
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("MinSpeedForSpreadMultiplier").decrypt(), &SDK::Cached::Offsets::FortRangedWeaponStats::MinSpeedForSpreadMultiplier, nullptr },
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("MaxSpeedForSpreadMultiplier").decrypt(), &SDK::Cached::Offsets::FortRangedWeaponStats::MaxSpeedForSpreadMultiplier, nullptr },
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("RecoilVert").decrypt(),				&SDK::Cached::Offsets::FortRangedWeaponStats::RecoilVert,		nullptr },
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("RecoilHoriz").decrypt(),				&SDK::Cached::Offsets::FortRangedWeaponStats::RecoilHoriz,		nullptr },
			OffsetSearch { skCrypt("FortRangedWeaponStats").decrypt(),	skCrypt("BulletsPerCartridge").decrypt(),		&SDK::Cached::Offsets::FortRangedWeaponStats::BulletsPerCartridge, nullptr },

			OffsetSearch { skCrypt("FortBaseWeaponStats").decrypt(),	skCrypt("ReloadTime").decrypt(),				&SDK::Cached::Offsets::FortRangedWeaponStats::ReloadTime,		nullptr },
			
			OffsetSearch { skCrypt("VehiclePawnState").decrypt(),		skCrypt("Vehicle").decrypt(),					&SDK::Cached::Offsets::VehiclePawnState::Vehicle,				nullptr },
		};

		if (SDK::GetGameVersion() >= 6.00) {
			Offsets.push_back(OffsetSearch{ skCrypt("FortRechargingActionTimer").decrypt(), skCrypt("ChargeRate").decrypt(), &SDK::Cached::Offsets::FortRechargingActionTimer::ChargeRate, nullptr });
			Offsets.push_back(OffsetSearch{ skCrypt("FortRechargingActionTimer").decrypt(), skCrypt("ActiveExpenseRate").decrypt(), &SDK::Cached::Offsets::FortRechargingActionTimer::ActiveExpenseRate, nullptr });
			Offsets.push_back(OffsetSearch{ skCrypt("FortRechargingActionTimer").decrypt(), skCrypt("PassiveExpenseRate").decrypt(), &SDK::Cached::Offsets::FortRechargingActionTimer::PassiveExpenseRate, nullptr });
			Offsets.push_back(OffsetSearch{ skCrypt("FortRechargingActionTimer").decrypt(), skCrypt("Charge").decrypt(), &SDK::Cached::Offsets::FortRechargingActionTimer::Charge, nullptr });

			Offsets.push_back(OffsetSearch{ skCrypt("FortAntelopeVehicleConfigs").decrypt(), skCrypt("BoostAccumulationRate").decrypt(), &SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostAccumulationRate, nullptr });
			Offsets.push_back(OffsetSearch{ skCrypt("FortAntelopeVehicleConfigs").decrypt(), skCrypt("BoostExpenseRate").decrypt(), &SDK::Cached::Offsets::FortAntelopeVehicleConfigs::BoostExpenseRate, nullptr });

			Offsets.push_back(OffsetSearch{ skCrypt("FortAthenaAntelopeVehicle").decrypt(), skCrypt("FortAntelopeVehicleConfigs").decrypt(), &SDK::Cached::Offsets::FortAthenaAntelopeVehicle::FortAntelopeVehicleConfigs, nullptr });

			Offsets.push_back(OffsetSearch{ skCrypt("FortAthenaJackalVehicle").decrypt(), skCrypt("BoostTimers").decrypt(), &SDK::Cached::Offsets::FortAthenaJackalVehicle::BoostTimers, nullptr });
		}

		if (SDK::GetGameVersion() >= 7.00) {
			// Bit of a hacky way to do it since its not 100% accurate if its using the enum for teams or the direct value, but they both have the same type so it doesn't matter
			Offsets.push_back(OffsetSearch{ skCrypt("BuildingActor").decrypt(), skCrypt("TeamIndex").decrypt(), &SDK::Cached::Offsets::BuildingActor::TeamIndex, nullptr });
		
			Offsets.push_back(OffsetSearch{ skCrypt("FortAthenaDoghouseVehicle").decrypt(), skCrypt("BoostAction").decrypt(),	&SDK::Cached::Offsets::FortAthenaDoghouseVehicle::BoostAction, nullptr });
		}
		else {
			Offsets.push_back(OffsetSearch{ skCrypt("BuildingActor").decrypt(), skCrypt("Team").decrypt(), &SDK::Cached::Offsets::BuildingActor::TeamIndex, nullptr });
		}

		if (SDK::GetGameVersion() >= 10.00) {
			Offsets.push_back(OffsetSearch{ skCrypt("FortWeapon").decrypt(), skCrypt("LastFireTimeVerified").decrypt(), &SDK::Cached::Offsets::FortWeapon::LastFireTimeVerified, nullptr });
		}

		SDK::UObject::SetupObjects(Functions, Offsets);
	}

	Input::Init();
	Features::FortPawnHelper::Bone::Init();

#if OBJECT_DUMP
	for (int i = 0; i < SDK::UObject::ObjectArray.Num(); i++) {
		SDK::UObject* Object = SDK::UObject::ObjectArray.GetByIndex(i);
		if (Object == nullptr) continue;

		DEBUG_LOG(skCrypt("[").decrypt() + std::to_string(i) + skCrypt("] ").decrypt() + Object->GetFullName());
	}
#endif
#if NAME_DUMP
	// this is a bit bad as it will eventually crash, due to the fact we loop until INT_MAX
	for (int i = 0; i < INT_MAX; i++) {
		SDK::FName Name;
		Name.ComparisonIndex = i;
		Name.Number = 0;

		DEBUG_LOG(skCrypt("[").decrypt() + std::to_string(i) + skCrypt("] ").decrypt() + Name.GetRawString());
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