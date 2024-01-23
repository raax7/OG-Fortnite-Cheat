#include "SDK.h"
#include "../../Utilitys/Logger.h"
#include "../../Utilitys/skCrypter.h"
#include "Classes/CoreUObject_classes.h"
#include "Classes/Engine_classes.h"
#include "SDKInitializer.h"
#include "../../Globals.h"

void SDK::Init() {
	DEBUG_LOG(skCrypt("Initializing SDK...").decrypt());

	// Init Objects
	{
		// Init GObjects
		SDKInitializer::InitGObjects();

		// Init Functions
		SDKInitializer::InitAppendString();
		SDKInitializer::InitGetBoneMatrix();

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

		if (GetGameVersion() < 3.0 || GetGameVersion() >= 11.0) {
			THROW_ERROR(
				skCrypt("Unsupported game version! (").decrypt() +
				std::to_string(GetGameVersion()) +
				skCrypt(")\nSeason 3 to Season 10 are the only seasons supported currently").decrypt()
				, true);
		}
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
			FunctionSearch { skCrypt("Controller").decrypt(),			skCrypt("ClientSetRotation").decrypt(),			&SDK::Cached::Functions::PlayerController::ClientSetRotation },
			FunctionSearch { skCrypt("KismetSystemLibrary").decrypt(),	skCrypt("LineTraceSingle").decrypt(),			&SDK::Cached::Functions::KismetSystemLibrary::LineTraceSingle },
			FunctionSearch { skCrypt("PlayerState").decrypt(),			skCrypt("GetPlayerName").decrypt(),				&SDK::Cached::Functions::FortPlayerState::GetPlayerName },
			FunctionSearch { skCrypt("SkinnedMeshComponent").decrypt(),	skCrypt("GetBoneName").decrypt(),				&SDK::Cached::Functions::SkinnedMeshComponent::GetBoneName },
			FunctionSearch { skCrypt("SceneComponent").decrypt(),		skCrypt("GetSocketLocation").decrypt(),			&SDK::Cached::Functions::SkinnedMeshComponent::GetSocketTransform },
		};

		std::vector<OffsetSearch> Offsets{
			OffsetSearch { skCrypt("Engine").decrypt(),					skCrypt("GameViewport").decrypt(),				&SDK::Cached::Offsets::Engine::GameViewport },
			OffsetSearch { skCrypt("GameViewportClient").decrypt(),		skCrypt("World").decrypt(),						&SDK::Cached::Offsets::GameViewportClient::World },
			OffsetSearch { skCrypt("GameViewportClient").decrypt(),		skCrypt("GameInstance").decrypt(),				&SDK::Cached::Offsets::GameViewportClient::GameInstance },
			OffsetSearch { skCrypt("GameInstance").decrypt(),			skCrypt("LocalPlayers").decrypt(),				&SDK::Cached::Offsets::GameInstance::LocalPlayers },
			OffsetSearch { skCrypt("Player").decrypt(),					skCrypt("PlayerController").decrypt(),			&SDK::Cached::Offsets::Player::PlayerController },
			OffsetSearch { skCrypt("PlayerController").decrypt(),		skCrypt("AcknowledgedPawn").decrypt(),			&SDK::Cached::Offsets::PlayerController::AcknowledgedPawn },
			OffsetSearch { skCrypt("PlayerController").decrypt(),		skCrypt("PlayerCameraManager").decrypt(),		&SDK::Cached::Offsets::PlayerController::PlayerCameraManager },
			OffsetSearch { skCrypt("Pawn").decrypt(),					skCrypt("PlayerState").decrypt(),				&SDK::Cached::Offsets::Pawn::PlayerState },
			OffsetSearch { skCrypt("Character").decrypt(),				skCrypt("Mesh").decrypt(),						&SDK::Cached::Offsets::Character::Mesh },
			OffsetSearch { skCrypt("Font").decrypt(),					skCrypt("LegacyFontSize").decrypt(),			&SDK::Cached::Offsets::Font::LegacyFontSize },
			OffsetSearch { skCrypt("FortItemEntry").decrypt(),			skCrypt("ItemDefinition").decrypt(),			&SDK::Cached::Offsets::FortItemEntry::ItemDefinition },
			OffsetSearch { skCrypt("FortPickup").decrypt(),				skCrypt("PrimaryPickupItemEntry").decrypt(),	&SDK::Cached::Offsets::FortPickup::PrimaryPickupItemEntry },
			OffsetSearch { skCrypt("FortItemDefinition").decrypt(),		skCrypt("DisplayName").decrypt(),				&SDK::Cached::Offsets::FortItemDefinition::DisplayName },
			OffsetSearch { skCrypt("FortItemDefinition").decrypt(),		skCrypt("Tier").decrypt(),						&SDK::Cached::Offsets::FortItemDefinition::Tier },
			OffsetSearch { skCrypt("Actor").decrypt(),					skCrypt("RootComponent").decrypt(),				&SDK::Cached::Offsets::Actor::RootComponent },
			OffsetSearch { skCrypt("SceneComponent").decrypt(),			skCrypt("RelativeLocation").decrypt(),			&SDK::Cached::Offsets::SceneComponent::RelativeLocation },
			OffsetSearch { skCrypt("Canvas").decrypt(),					skCrypt("SizeX").decrypt(),						&SDK::Cached::Offsets::Canvas::SizeX },
			OffsetSearch { skCrypt("Canvas").decrypt(),					skCrypt("SizeY").decrypt(),						&SDK::Cached::Offsets::Canvas::SizeY },
			OffsetSearch { skCrypt("FortPlayerStateAthena").decrypt(),	skCrypt("TeamIndex").decrypt(),					&SDK::Cached::Offsets::FortPlayerStateAthena::TeamIndex },
		};

		SDK::UObject::SetupObjects(Functions, Offsets);
	}

	DEBUG_LOG(skCrypt("SDK Initialized!").decrypt());
	DEBUG_LOG(skCrypt("Game Version: ").decrypt() + std::to_string(GetGameVersion()));

#if DUMP_OBJECTS
	for (int i = 0; i < INT_MAX; i++) {
		SDK::FName name;
		name.ComparisonIndex = i;

		DEBUG_LOG(skCrypt("[").decrypt() + std::to_string(i) + skCrypt("] ").decrypt() + name.ToString());
	}
#endif
#if DUMP_NAMES
	for (int i = 0; i < INT_MAX; i++) {
		SDK::FName name;
		name.ComparisonIndex = i;

		DEBUG_LOG(skCrypt("[").decrypt() + std::to_string(i) + skCrypt("] ").decrypt() + name.ToString());
	}
#endif
}