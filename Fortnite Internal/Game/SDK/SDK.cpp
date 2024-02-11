#include "SDK.h"
#include "../../Utilities/Logger.h"
#include "../../Utilities/skCrypter.h"
#include "Classes/CoreUObject_classes.h"
#include "Classes/Engine_classes.h"
#include "SDKInitializer.h"
#include "../../Globals.h"
#include "../Game.h"

void SDK::Init() {
	DEBUG_LOG(skCrypt("Initializing SDK...").decrypt());

	// Init Objects
	{
		// Init GObjects
		SDKInitializer::InitGObjects();

		// Init Functions
		SDKInitializer::InitAppendString();
		SDKInitializer::InitGetBoneMatrix();
		SDKInitializer::InitLineTraceSingle();

		// Init VFT Indexes
		SDKInitializer::InitPRIndex();
		SDKInitializer::InitPEIndex();
		SDKInitializer::InitGPVIndex();
		SDKInitializer::InitGVIndex();

		SDK::UProperty::OffsetOffset		= 0x44;
		SDK::UClass::DefaultObjectOffset	= SDKInitializer::FindDefaultObjectOffset();
		SDK::UClass::CastFlagsOffset		= SDKInitializer::FindCastFlagsOffset();
		SDK::UStruct::SuperOffset			= SDKInitializer::FindSuperOffset();
	}

	// Check Engine Version
	{
		SDK::Cached::Functions::KismetSystemLibrary::GetEngineVersion = SDK::UObject::FindObjectFast(skCrypt("GetEngineVersion").decrypt());

		if (GetGameVersion() < 3.00 || GetGameVersion() >= 16.00) {
			THROW_ERROR(
				skCrypt("Unsupported game version! (").decrypt() +
				std::to_string(GetGameVersion()) +
				skCrypt(")\nSeason 3 to Season 15 are the only seasons supported currently").decrypt(),
				true);
		}

		Game::GameVersion = SDK::GetGameVersion();

		DEBUG_LOG(skCrypt("Game Version: ").decrypt() + std::to_string(GetGameVersion()));

		//if (GetGameVersion() >= 12.00) {
		//	SDK::UProperty::OffsetOffset = 0x48;
		//}
	}

	// Init Cached Objects
	{
		std::vector<FunctionSearch> Functions{
			FunctionSearch { skCrypt("Canvas").decrypt(),				skCrypt("K2_DrawLine").decrypt(),				&SDK::Cached::Functions::Canvas::K2_DrawLine },
			FunctionSearch { skCrypt("Canvas").decrypt(),				skCrypt("K2_DrawText").decrypt(),				&SDK::Cached::Functions::Canvas::K2_DrawText },
			FunctionSearch { skCrypt("Canvas").decrypt(),				skCrypt("K2_TextSize").decrypt(),				&SDK::Cached::Functions::Canvas::K2_TextSize },
			FunctionSearch { skCrypt("Canvas").decrypt(),				skCrypt("K2_Project").decrypt(),				&SDK::Cached::Functions::Canvas::K2_Project },
			FunctionSearch { skCrypt("GameplayStatics").decrypt(),		skCrypt("GetAllActorsOfClass").decrypt(),		&SDK::Cached::Functions::GameplayStatics::GetAllActorsOfClass },
			FunctionSearch { skCrypt("PlayerCameraManager").decrypt(),	skCrypt("GetCameraLocation").decrypt(),			&SDK::Cached::Functions::PlayerCameraManager::GetCameraLocation },
			FunctionSearch { skCrypt("PlayerCameraManager").decrypt(),	skCrypt("GetCameraRotation").decrypt(),			&SDK::Cached::Functions::PlayerCameraManager::GetCameraRotation },
			FunctionSearch { skCrypt("PlayerCameraManager").decrypt(),	skCrypt("GetFOVAngle").decrypt(),				&SDK::Cached::Functions::PlayerCameraManager::GetFOVAngle },
			FunctionSearch { skCrypt("Controller").decrypt(),			skCrypt("ClientSetRotation").decrypt(),			&SDK::Cached::Functions::PlayerController::ClientSetRotation },
			FunctionSearch { skCrypt("KismetSystemLibrary").decrypt(),	skCrypt("LineTraceSingle").decrypt(),			&SDK::Cached::Functions::KismetSystemLibrary::LineTraceSingle },
			FunctionSearch { skCrypt("PlayerState").decrypt(),			skCrypt("GetPlayerName").decrypt(),				&SDK::Cached::Functions::PlayerState::GetPlayerName },
			FunctionSearch { skCrypt("SkinnedMeshComponent").decrypt(),	skCrypt("GetBoneName").decrypt(),				&SDK::Cached::Functions::SkinnedMeshComponent::GetBoneName },
			FunctionSearch { skCrypt("SceneComponent").decrypt(),		skCrypt("GetSocketLocation").decrypt(),			&SDK::Cached::Functions::SkinnedMeshComponent::GetSocketTransform },
		};

		std::vector<OffsetSearch> Offsets{
			OffsetSearch { skCrypt("Engine").decrypt(),					skCrypt("GameViewport").decrypt(),				&SDK::Cached::Offsets::Engine::GameViewport,					OffsetType::Class},
			OffsetSearch { skCrypt("GameViewportClient").decrypt(),		skCrypt("World").decrypt(),						&SDK::Cached::Offsets::GameViewportClient::World,				OffsetType::Class },
			OffsetSearch { skCrypt("GameViewportClient").decrypt(),		skCrypt("GameInstance").decrypt(),				&SDK::Cached::Offsets::GameViewportClient::GameInstance,		OffsetType::Class },
			OffsetSearch { skCrypt("GameInstance").decrypt(),			skCrypt("LocalPlayers").decrypt(),				&SDK::Cached::Offsets::GameInstance::LocalPlayers,				OffsetType::Class },
			OffsetSearch { skCrypt("Player").decrypt(),					skCrypt("PlayerController").decrypt(),			&SDK::Cached::Offsets::Player::PlayerController,				OffsetType::Class },
			OffsetSearch { skCrypt("PlayerController").decrypt(),		skCrypt("AcknowledgedPawn").decrypt(),			&SDK::Cached::Offsets::PlayerController::AcknowledgedPawn,		OffsetType::Class },
			OffsetSearch { skCrypt("PlayerController").decrypt(),		skCrypt("PlayerCameraManager").decrypt(),		&SDK::Cached::Offsets::PlayerController::PlayerCameraManager,	OffsetType::Class },
			OffsetSearch { skCrypt("PlayerController").decrypt(),		skCrypt("MyHUD").decrypt(),						&SDK::Cached::Offsets::PlayerController::MyHUD,					OffsetType::Class },
			OffsetSearch { skCrypt("HUD").decrypt(),					skCrypt("DebugCanvas").decrypt(),				&SDK::Cached::Offsets::HUD::Canvas,								OffsetType::Class },
			OffsetSearch { skCrypt("Pawn").decrypt(),					skCrypt("PlayerState").decrypt(),				&SDK::Cached::Offsets::Pawn::PlayerState,						OffsetType::Class },
			OffsetSearch { skCrypt("Character").decrypt(),				skCrypt("Mesh").decrypt(),						&SDK::Cached::Offsets::Character::Mesh,							OffsetType::Class },
			OffsetSearch { skCrypt("Font").decrypt(),					skCrypt("LegacyFontSize").decrypt(),			&SDK::Cached::Offsets::Font::LegacyFontSize,					OffsetType::Class },

			OffsetSearch { skCrypt("FortPickup").decrypt(),				skCrypt("PrimaryPickupItemEntry").decrypt(),	&SDK::Cached::Offsets::FortPickup::PrimaryPickupItemEntry,		OffsetType::Class },
			OffsetSearch { skCrypt("FortItemDefinition").decrypt(),		skCrypt("DisplayName").decrypt(),				&SDK::Cached::Offsets::FortItemDefinition::DisplayName,			OffsetType::Class },
			OffsetSearch { skCrypt("FortItemDefinition").decrypt(),		skCrypt("Tier").decrypt(),						&SDK::Cached::Offsets::FortItemDefinition::Tier,				OffsetType::Class },
			OffsetSearch { skCrypt("Actor").decrypt(),					skCrypt("RootComponent").decrypt(),				&SDK::Cached::Offsets::Actor::RootComponent,					OffsetType::Class },
			OffsetSearch { skCrypt("SceneComponent").decrypt(),			skCrypt("RelativeLocation").decrypt(),			&SDK::Cached::Offsets::SceneComponent::RelativeLocation,		OffsetType::Class },
			OffsetSearch { skCrypt("Canvas").decrypt(),					skCrypt("SizeX").decrypt(),						&SDK::Cached::Offsets::Canvas::SizeX,							OffsetType::Class },
			OffsetSearch { skCrypt("Canvas").decrypt(),					skCrypt("SizeY").decrypt(),						&SDK::Cached::Offsets::Canvas::SizeY,							OffsetType::Class },
			OffsetSearch { skCrypt("FortPlayerStateAthena").decrypt(),	skCrypt("TeamIndex").decrypt(),					&SDK::Cached::Offsets::FortPlayerStateAthena::TeamIndex,		OffsetType::Class },
			OffsetSearch { skCrypt("FortPawn").decrypt(),				skCrypt("CurrentWeapon").decrypt(),				&SDK::Cached::Offsets::FortPawn::CurrentWeapon,					OffsetType::Class },
			OffsetSearch { skCrypt("BuildingWeakSpot").decrypt(),		skCrypt("bHit").decrypt(),						&SDK::Cached::Offsets::BuildingWeakSpot::WeakSpotInfoBitField,	OffsetType::Class },
			OffsetSearch { skCrypt("FortWeapon").decrypt(),				skCrypt("WeaponData").decrypt(),				&SDK::Cached::Offsets::FortWeapon::WeaponData,					OffsetType::Class },

			OffsetSearch { skCrypt("FortItemEntry").decrypt(),			skCrypt("ItemDefinition").decrypt(),			&SDK::Cached::Offsets::FortItemEntry::ItemDefinition,			OffsetType::Struct },
			OffsetSearch { skCrypt("MinimalViewInfo").decrypt(),		skCrypt("Location").decrypt(),					&SDK::Cached::Offsets::MinimalViewInfo::Location,				OffsetType::Struct },
			OffsetSearch { skCrypt("MinimalViewInfo").decrypt(),		skCrypt("Rotation").decrypt(),					&SDK::Cached::Offsets::MinimalViewInfo::Rotation,				OffsetType::Struct },
		};

		SDK::UObject::SetupObjects(Functions, Offsets);
	}

	DEBUG_LOG(skCrypt("SDK Initialized!").decrypt());

#if OBJECT_DUMP
	for (int i = 0; i < SDK::UObject::ObjectArray.Num(); i++) {
		SDK::UObject* Object = SDK::UObject::ObjectArray.GetByIndex(i);
		if (!Object) continue;

		DEBUG_LOG(skCrypt("[").decrypt() + std::to_string(i) + skCrypt("] ").decrypt() + Object->GetFullName());
	}
#endif
#if NAME_DUMP
	// this is a bit bad as it will eventually crash, due to the fact we loop until INT_MAX
	for (int i = 0; i < INT_MAX; i++) {
		SDK::FName Name;
		Name.ComparisonIndex = i;

		DEBUG_LOG(skCrypt("[").decrypt() + std::to_string(i) + skCrypt("] ").decrypt() + Name.ToString());
	}
#endif
}