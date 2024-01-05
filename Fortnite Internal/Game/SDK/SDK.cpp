#include <algorithm>
#include <iomanip>
#include "SDK.h"
#include "../../Utilitys/Memory.h"
#include "../../Utilitys/Logger.h"
#include "../../Utilitys/skCrypter.h"

#include "Classes/Basic.h"
#include "Classes/CoreUObject_classes.h"
#include "Classes/Engine_classes.h"

void InitGObjects() {
	EXTRA_DEBUG_LOG(skCrypt("Searching for GObjects...").decrypt());

	SDK::UObject::ObjectArray.IsChunked = true;

	uintptr_t TUObjectArray = Memory::PatternScan(
		SDK::GetBaseAddress(),
		skCrypt("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1").decrypt(),
		7,
		true
	);

	if (!SDK::IsValidPointer(TUObjectArray)) {
		SDK::UObject::ObjectArray.IsChunked = false;
		TUObjectArray = Memory::PatternScan(
			SDK::GetBaseAddress(),
			skCrypt("48 8B 05 ? ? ? ? 48 8D 14 C8 EB 02").decrypt(),
			7,
			true
		);
	}

	if (!SDK::IsValidPointer(TUObjectArray)) {
		THROW_ERROR(skCrypt("Failed to find GObjects!").decrypt(), true);
	}

	if (SDK::UObject::ObjectArray.IsChunked) {
		SDK::UObject::ObjectArray.ChunkedObjects = reinterpret_cast<SDK::Chunked_TUObjectArray*>(TUObjectArray);
	}
	else {
		SDK::UObject::ObjectArray.FixedObjects = reinterpret_cast<SDK::Fixed_TUObjectArray*>(TUObjectArray);
	}

	DEBUG_LOG(skCrypt("GObjects Found at: ").decrypt() + std::to_string(TUObjectArray - SDK::GetBaseAddress()));
}
void InitAppendString() {
	EXTRA_DEBUG_LOG(skCrypt("Searching for AppendString...").decrypt());

	std::vector<const char*> possibleSigs = {
		skCrypt("48 8D ? ? 48 8D ? ? E8").decrypt(),
		skCrypt("48 8D ? ? ? 48 8D ? ? E8").decrypt(),
		skCrypt("48 8D ? ? 49 8B ? E8").decrypt(),
		skCrypt("48 8D ? ? ? 49 8B ? E8").decrypt()
	};

	EXTRA_DEBUG_LOG(skCrypt("Searching for string reference: \"ForwardShadingQuality_\"").decrypt());
	auto stringRef = Memory::FindByStringInAllSections(reinterpret_cast<const char*>(skCrypt("ForwardShadingQuality_").decrypt()));

	EXTRA_DEBUG_LOG(skCrypt("Searching for AppendString offset").decrypt());

	for (int i = 0; i < possibleSigs.size() && !SDK::AppendStringOffset; ++i) {
		SDK::AppendStringOffset = reinterpret_cast<uintptr_t>(
			Memory::FindPatternInRange(possibleSigs[i], stringRef, 0x60, true, -1)
			) - SDK::GetBaseAddress();
	}

	possibleSigs.clear();

	if (SDK::IsValidPointer(SDK::AppendStringOffset)) {
		DEBUG_LOG(skCrypt("AppendString offset found: ").decrypt() + std::to_string(SDK::AppendStringOffset));
	}
	else {
		THROW_ERROR(skCrypt("Failed to find AppendString!").decrypt(), true);
	}
}
void InitGetBoneMatrix() {
	EXTRA_DEBUG_LOG(skCrypt("Searching for GetBoneMatrix...").decrypt());

	std::vector<const char*> possibleSigs = { skCrypt("E8 ? ? ? ? 48 8B CB 0F ? 00 0F 11 83 ? ? 00 00 0F ? 48 10 0F 11 8B").decrypt() };

	EXTRA_DEBUG_LOG(skCrypt("Searching for string reference: \"USkeletalMeshComponent::InitArticulated : Could not find root physics body: '%s'\"").decrypt());
	auto stringRef = Memory::FindByStringInAllSections(skCrypt(L"USkeletalMeshComponent::InitArticulated : Could not find root physics body: '%s'").decrypt());

	EXTRA_DEBUG_LOG(skCrypt("Searching for GetBoneMatrix offset").decrypt());

	for (int i = 0; !SDK::GetBoneMatrix && i < possibleSigs.size(); ++i) {
		EXTRA_DEBUG_LOG(std::to_string(i2));

		uint8_t* patternAddress = reinterpret_cast<uint8_t*>(
			Memory::FindPatternInRange(possibleSigs[i], (stringRef - 0x80), 0x220, false, 0)
			);

		if (!patternAddress) {
			continue;
		}

		// Assuming the call instruction is 5 bytes (including the opcode)
		uintptr_t callInstructionAddress = reinterpret_cast<uintptr_t>(patternAddress);
		int32_t relativeOffset = *reinterpret_cast<int32_t*>(callInstructionAddress + 1); // Offset is at position 1 in x64 call instruction

		// Calculate the target address of the function being called
		SDK::GetBoneMatrix = callInstructionAddress + relativeOffset + 5;
		SDK::GetBoneMatrix -= SDK::GetBaseAddress();
	}

	possibleSigs.clear();

	if (SDK::IsValidPointer(SDK::GetBoneMatrix)) {
		DEBUG_LOG(skCrypt("GetBoneMatrix offset found: ").decrypt() + std::to_string(SDK::GetBoneMatrix));
	}
	else {
		THROW_ERROR(skCrypt("Failed to find GetBoneMatrix!").decrypt(), true);
	}
}
void InitPRIndex() {
	EXTRA_DEBUG_LOG(skCrypt("Searching for PostRender VFT Index...").decrypt());

	std::vector<const char*> possibleSigs = {
		skCrypt("FF 90 ? ? ? ? 80 3D ? ? ? 02 00").decrypt(),
		skCrypt("FF 90 ? ? ? ? 48 8D 0D ? ? ? ? E8").decrypt()
	};

	EXTRA_DEBUG_LOG(skCrypt("Searching for string reference: \"STAT_HudTime\"").decrypt());
	uint8_t* stringRef = Memory::FindByStringInAllSections(skCrypt(L"STAT_HudTime").decrypt());

	EXTRA_DEBUG_LOG(skCrypt("Searching for PostRender VFT Index offset").decrypt());

	for (int i = 0; !SDK::Cached::VFT::PostRender && i < possibleSigs.size(); ++i) {
		uint8_t* patternAddress = reinterpret_cast<uint8_t*>(
			Memory::FindPatternInRange(possibleSigs[i], stringRef, 0x300, false, 0)
			);

		if (patternAddress) {
			// Add 2 bytes of data (FF 90)
			DWORD displacement = *reinterpret_cast<DWORD*>(patternAddress + 2);

			// Divide by size of uintptr_t
			SDK::Cached::VFT::PostRender = displacement / 8;
		}
	}

	possibleSigs.clear();

	if (SDK::IsValidPointer(SDK::Cached::VFT::PostRender)) {
		DEBUG_LOG(skCrypt("PostRender VFT Index offset found: ").decrypt() + std::to_string(SDK::Cached::VFT::PostRender));
	}
	else {
		THROW_ERROR(skCrypt("Failed to find PostRender VFT Index!").decrypt(), true);
	}
}
void InitPEIndex() {
	EXTRA_DEBUG_LOG(skCrypt("Searching for ProcessEvent VFT Index...").decrypt());

	std::vector<const char*> possibleSigs = { skCrypt("41 FF 91 ? ? ? ? E9 ? ? ? ? 80 3D ? ? ? ? ?").decrypt() };

	EXTRA_DEBUG_LOG(skCrypt("Searching for string reference: \"FLatentActionManager::ProcessLatentActions: Could not find latent action resume point named '%s' on '%s' called by '%s'\"").decrypt());
	uint8_t* stringRef = Memory::FindByStringInAllSections(skCrypt(L"FLatentActionManager::ProcessLatentActions: Could not find latent action resume point named '%s' on '%s' called by '%s'").decrypt());

	EXTRA_DEBUG_LOG(skCrypt("Searching for ProcessEvent VFT Index offset").decrypt());

	for (int i = 0; !SDK::Cached::VFT::ProcessEvent && i < possibleSigs.size(); ++i) {
		uint8_t* patternAddress = reinterpret_cast<uint8_t*>(
			Memory::FindPatternInRange(possibleSigs[i], (stringRef - 0x100), 0x200, false, 0)
			);

		if (patternAddress) {
			DWORD displacement = *reinterpret_cast<DWORD*>(patternAddress + 3);
			SDK::Cached::VFT::ProcessEvent = displacement / 8; // Divide by size of uintptr_t
		}
	}

	possibleSigs.clear();

	if (SDK::IsValidPointer(SDK::Cached::VFT::ProcessEvent)) {
		DEBUG_LOG(skCrypt("ProcessEvent VFT Index offset found: ").decrypt() + std::to_string(SDK::Cached::VFT::ProcessEvent));
	}
	else {
		THROW_ERROR(skCrypt("Failed to find ProcessEvent VFT Index!").decrypt(), true);
	}
}

int32_t FindCastFlagsOffset() {
	std::vector<std::pair<void*, SDK::EClassCastFlags>> infos = {
		{ SDK::UObject::FindObjectFast(skCrypt("Actor").decrypt()), SDK::EClassCastFlags::Actor},
		{ SDK::UObject::FindObjectFast(skCrypt("Class").decrypt()), SDK::EClassCastFlags::Field | SDK::EClassCastFlags::Struct | SDK::EClassCastFlags::Class }
	};
	return Memory::FindOffset(infos);
}
int32_t FindDefaultObjectOffset() {
	std::vector<std::pair<void*, void*>> infos = {
		{ SDK::UObject::FindObjectFast(skCrypt("Object").decrypt()), SDK::UObject::FindObjectFast(skCrypt("Default__Object").decrypt()) },
		{ SDK::UObject::FindObjectFast(skCrypt("Field").decrypt()), SDK::UObject::FindObjectFast(skCrypt("Default__Field").decrypt()) }
	};
	return Memory::FindOffset(infos);
}
int32_t FindSuperOffset() {
	std::vector<std::pair<void*, void*>> infos = {
		{ SDK::UObject::FindObjectFast(skCrypt("Struct").decrypt()), SDK::UObject::FindObjectFast(skCrypt("Field").decrypt()) },
		{ SDK::UObject::FindObjectFast(skCrypt("Class").decrypt()), SDK::UObject::FindObjectFast(skCrypt("Struct").decrypt()) }
	};

	// Thanks to the ue4 dev who decided UStruct should be spelled Ustruct
	if (infos[0].first == nullptr) {
		infos[0].first = infos[1].second = SDK::UObject::FindObjectFast(skCrypt("struct").decrypt());
	}

	return Memory::FindOffset(infos);
}

void SDK::Init() {
	DEBUG_LOG(skCrypt("Initializing SDK...").decrypt());

	InitGObjects();
	InitAppendString();
	InitGetBoneMatrix();
	InitPRIndex();
	InitPEIndex();

	SDK::UProperty::OffsetOffset = 0x44;
	SDK::UClass::DefaultObjectOffset = FindDefaultObjectOffset();
	SDK::UClass::CastFlagsOffset = FindCastFlagsOffset();
	SDK::UStruct::SuperOffset = FindSuperOffset();

	SDK::Cached::Functions::GetEngineVersion = SDK::UObject::FindObject(skCrypt("Function Engine.KismetSystemLibrary.GetEngineVersion").decrypt());

	if (GetGameVersion() < 3.0 || GetGameVersion() >= 11.0) {
		THROW_ERROR(skCrypt("Unsupported game version! (").decrypt() + std::to_string(GetGameVersion()) + skCrypt(")\nSeason 3 to Season 10 are the only seasons supported currently").decrypt(), true);
	}

	SDK::Cached::Functions::K2_DrawLine = SDK::UObject::FindObjectFast(skCrypt("K2_DrawLine").decrypt());
	SDK::Cached::Functions::K2_DrawText = SDK::UObject::FindObjectFast(skCrypt("K2_DrawText").decrypt());
	SDK::Cached::Functions::K2_Project = SDK::UObject::FindObjectFast(skCrypt("K2_Project").decrypt());
	SDK::Cached::Functions::GetAllActorsOfClass = SDK::UObject::FindObject(skCrypt("Function Engine.GameplayStatics.GetAllActorsOfClass").decrypt());

	SDK::Cached::Offsets::Engine::GameViewport = SDK::UObject::GetOffset(skCrypt("Engine").decrypt(), skCrypt("GameViewport").decrypt());
	SDK::Cached::Offsets::GameViewportClient::World = SDK::UObject::GetOffset(skCrypt("GameViewportClient").decrypt(), skCrypt("World").decrypt());
	SDK::Cached::Offsets::PlayerState = SDK::UObject::GetOffset(skCrypt("Pawn").decrypt(), skCrypt("PlayerState").decrypt());
	SDK::Cached::Offsets::Mesh = SDK::UObject::GetOffset(skCrypt("Character").decrypt(), skCrypt("Mesh").decrypt());
	SDK::Cached::Offsets::LegacyFontSize = SDK::UObject::GetOffset(skCrypt("Font").decrypt(), skCrypt("LegacyFontSize").decrypt());

	DEBUG_LOG(skCrypt("Game Version: ").decrypt() + std::to_string(GetGameVersion()));
}