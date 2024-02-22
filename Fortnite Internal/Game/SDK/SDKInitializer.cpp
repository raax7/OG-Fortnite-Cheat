#include "SDKInitializer.h"

#include "../../Utilities/Logger.h"

void SDKInitializer::WalkVFT(const char* TargetFunctionName, void** VFT, void* TargetFunction, uintptr_t& VFTIndex, int SearchRange) {
	EXTRA_DEBUG_LOG(skCrypt("Walking VFT searching for ").decrypt() + std::string(TargetFunctionName) + skCrypt(" VFT index").decrypt());

	for (int i = 0; !VFTIndex && i < SearchRange; ++i) {
		if (VFT[i] == TargetFunction) {
			VFTIndex = i;
			break;
		}
	}

	if (VFTIndex) {
		DEBUG_LOG(std::string(TargetFunctionName) + skCrypt(" VFT index found: ").decrypt() + std::to_string(VFTIndex));
	}
	else {
		THROW_ERROR(skCrypt("Failed to find ").decrypt() + std::string(TargetFunctionName) + skCrypt(" VFT index!").decrypt(), false);
	}
}
void SDKInitializer::InitVFTIndex(const char* VFTName, std::vector<const char*> PossibleSigs, const wchar_t* SearchString, uintptr_t& VFTIndex, int SearchRange, int SearchBytesBehind) {
	EXTRA_DEBUG_LOG(skCrypt("Searching for ").decrypt() + std::string(VFTName) + skCrypt(" VFT index").decrypt());

	uint8_t* StringRef = Memory::FindByStringInAllSections(SearchString);

	EXTRA_DEBUG_LOG(skCrypt("Searching for VFT Index offset").decrypt());

	for (int i = 0; !VFTIndex && i < PossibleSigs.size(); ++i) {
		uint8_t* PatternAddress = reinterpret_cast<uint8_t*>(
			Memory::FindPatternInRange(PossibleSigs[i], (StringRef - SearchBytesBehind), SearchRange, false, 0)
			);

		if (PatternAddress) {
			DWORD Displacement = *reinterpret_cast<DWORD*>(PatternAddress + FindFirstWildCard(PossibleSigs[i]));
			VFTIndex = Displacement / sizeof(uintptr_t);
		}
	}

	PossibleSigs.clear();

	if (VFTIndex) {
		DEBUG_LOG(std::string(VFTName) + skCrypt(" VFT Index offset found: ").decrypt() + std::to_string(VFTIndex));
	}
	else {
		THROW_ERROR(skCrypt("Failed to find ").decrypt() + std::string(VFTName) + skCrypt(" VFT Index!").decrypt(), false);
	}
}

void SDKInitializer::InitFunctionOffset(const char* FunctionName, std::vector<const char*> PossibleSigs, const wchar_t* SearchString, uintptr_t& FunctionOffset, int SearchRange, int SearchBytesBehind) {
	EXTRA_DEBUG_LOG(skCrypt("Searching for ").decrypt() + std::string(FunctionName) + skCrypt(" function offset").decrypt());

	uint8_t* StringRef = Memory::FindByStringInAllSections(SearchString);

	EXTRA_DEBUG_LOG(skCrypt("Searching for function offset").decrypt());

	for (int i = 0; !FunctionOffset && i < PossibleSigs.size(); ++i) {
		FunctionOffset = reinterpret_cast<uintptr_t>(
			Memory::FindPatternInRange(PossibleSigs[i], StringRef - SearchBytesBehind, SearchRange, true, -1)
			);
	}

	PossibleSigs.clear();

	if (FunctionOffset) {
		FunctionOffset -= SDK::GetBaseAddress();
		DEBUG_LOG(std::string(FunctionName) + skCrypt(" function offset found: ").decrypt() + std::to_string(FunctionOffset));
	}
	else {
		THROW_ERROR(skCrypt("Failed to find ").decrypt() + std::string(FunctionName) + skCrypt(" function offset!").decrypt(), false);
	}
}
void SDKInitializer::InitFunctionOffset(const char* FunctionName, std::vector<const char*> PossibleSigs, const char* SearchString, uintptr_t& FunctionOffset, int SearchRange, int SearchBytesBehind) {
	EXTRA_DEBUG_LOG(skCrypt("Searching for ").decrypt() + std::string(FunctionName) + skCrypt(" function offset").decrypt());
	EXTRA_DEBUG_LOG(skCrypt("Searching for string reference: ").decrypt() + std::string(SearchString));

	uint8_t* StringRef = Memory::FindByStringInAllSections(SearchString);

	EXTRA_DEBUG_LOG(skCrypt("Searching for function offset").decrypt());

	for (int i = 0; !FunctionOffset && i < PossibleSigs.size(); ++i) {
		FunctionOffset = reinterpret_cast<uintptr_t>(
			Memory::FindPatternInRange(PossibleSigs[i], StringRef - SearchBytesBehind, SearchRange, true, -1)
			);
	}

	PossibleSigs.clear();

	if (FunctionOffset) {
		FunctionOffset -= SDK::GetBaseAddress();
		DEBUG_LOG(std::string(FunctionName) + skCrypt(" function offset found: ").decrypt() + std::to_string(FunctionOffset));
	}
	else {
		THROW_ERROR(skCrypt("Failed to find ").decrypt() + std::string(FunctionName) + skCrypt(" function offset!").decrypt(), false);
	}
}

void SDKInitializer::InitPRIndex() {
	InitVFTIndex(
		skCrypt("PostRender").decrypt(),
		std::vector<const char*>{ skCrypt("FF 90 ? ? ? ? 80 3D ? ? ? 02 00").decrypt(), skCrypt("FF 90 ? ? ? ? 48 8D 0D ? ? ? ? E8").decrypt(), skCrypt("FF 90 ? ? ? ? 48 8B 4D 28 E8 ? ? ? ?").decrypt(), skCrypt("FF 90 ? ? ? ? 48 8B").decrypt() },
		skCrypt(L"STAT_HudTime").decrypt(),
		SDK::Cached::VFT::PostRender,
		0x400);
}
void SDKInitializer::InitPEIndex() {
	InitVFTIndex(
		skCrypt("ProcessEvent").decrypt(),
		std::vector<const char*>{ skCrypt("41 FF 91 ? ? ? ? E9 ? ? ? ? 80 3D ? ? ? ? ?").decrypt() },
		skCrypt(L"FLatentActionManager::ProcessLatentActions: Could not find latent action resume point named '%s' on '%s' called by '%s'").decrypt(),
		SDK::Cached::VFT::ProcessEvent,
		0x400);

	if (SDK::Cached::VFT::ProcessEvent) return;

	// DOESNT WORK ON SOME VERSIONS

	uint8_t* StringRef = Memory::FindByStringInAllSections(skCrypt(L"Accessed None").decrypt());
	uintptr_t NextFunctionStart = Memory::FindNextFunctionStart(StringRef);
	DEBUG_LOG(skCrypt("ProcessEvent function start found: ").decrypt() + std::to_string(NextFunctionStart - SDK::GetBaseAddress()));

	SDKInitializer::WalkVFT(skCrypt("ProcessEvent").decrypt(), SDK::UObject::ObjectArray.GetByIndex(0)->Vft, reinterpret_cast<void*>(NextFunctionStart), SDK::Cached::VFT::ProcessEvent, 0x150);
}
void SDKInitializer::InitGPVIndex() {
	InitVFTIndex(
		skCrypt("GetPlayerViewpoint").decrypt(),
		std::vector<const char*>{ skCrypt("FF 90 ? ? ? ?").decrypt() },
		skCrypt(L"STAT_VolumeStreamingTickTime").decrypt(),
		SDK::Cached::VFT::GetPlayerViewpoint,
		0x400);
}
void SDKInitializer::InitGVIndex() {
	InitVFTIndex(
		skCrypt("GetViewpoint").decrypt(),
		std::vector<const char*>{ skCrypt("FF 90 ? ? ? ?").decrypt() },
		skCrypt(L"STAT_CalcSceneView").decrypt(),
		SDK::Cached::VFT::GetViewpoint,
		0x400);
}

void SDKInitializer::InitAppendString() {
	InitFunctionOffset(
		skCrypt("AppendString").decrypt(),
		std::vector<const char*>
	{
		skCrypt("48 8D ? ? 48 8D ? ? E8").decrypt(),
		skCrypt("48 8D ? ? ? 48 8D ? ? E8").decrypt(),
		skCrypt("48 8D ? ? 49 8B ? E8").decrypt(),
		skCrypt("48 8D ? ? ? 49 8B ? E8").decrypt()
	},
		skCrypt("ForwardShadingQuality_").decrypt(),
		SDK::AppendStringOffset,
		0x60);
}
void SDKInitializer::InitFNameConstructor() {
	InitFunctionOffset(
		skCrypt("FName Constructor").decrypt(),
		std::vector<const char*> { skCrypt("E8").decrypt() },
		skCrypt(L"CanvasObject").decrypt(),
		SDK::FNameConstructorOffset,
		0x32);
}
void SDKInitializer::InitLineTraceSingle() {
	SDK::LineTraceSingle = Memory::PatternScan(
		SDK::GetBaseAddress(),
		skCrypt("48 8B 43 20 48 8D 4D E0 0F 10 45 C0 44 0F B6 8D ? ? ? ? 48 85 C0 0F 10 4D D0 4C 8D 45 A0 40 0F 95 C7 66 0F 7F 45 ? 48 8D 55 B0 F2 0F 10 45 ? 48 03 F8 8B 45 88 F2 0F 11 45 ? F2 0F 10 45 ? 89 45 A8 8B 45 98 F2 0F 11 45 ? F3 0F 10 44 24 ? F3 0F 11 44 24 ? 48 89 4C 24 ? 48 8D 4D F0 48 89 4C 24 ? 48 8B 4C 24 ? 44 88 7C 24 ? 48 89 74 24 ? 89 45 B8 0F B6 85 ? ? ? ? 89 44 24 30 4C 89 74 24 ? 44 88 64 24 ? 48 89 7B 20 66 0F 7F 4D ? E8").decrypt(),
		155, // magic number, will improve pattern scanning function later
		true
	);

	DEBUG_LOG(skCrypt("LineTraceSingle function offset found: ").decrypt() + std::to_string(SDK::LineTraceSingle - SDK::GetBaseAddress()));
}

void SDKInitializer::InitGObjects() {
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

	if (SDK::UObject::ObjectArray.IsChunked) {
		SDK::UObject::ObjectArray.ChunkedObjects = reinterpret_cast<SDK::Chunked_TUObjectArray*>(TUObjectArray);
	}
	else {
		SDK::UObject::ObjectArray.FixedObjects = reinterpret_cast<SDK::Fixed_TUObjectArray*>(TUObjectArray);
	}

	if (SDK::IsValidPointer(TUObjectArray)) {
		DEBUG_LOG(skCrypt("GObjects offset found: ").decrypt() + std::to_string(TUObjectArray - SDK::GetBaseAddress()));
	}
	else {
		THROW_ERROR(skCrypt("Failed to find GObjects!").decrypt(), true);
	}
}