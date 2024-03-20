#include "SDKInitializer.h"

#include "../SDK/Classes/Engine_classes.h"

#include "../../Utilities/Logger.h"

void SDKInitializer::WalkVFT(const char* TargetFunctionName, void** VFT, void* TargetFunction, uintptr_t& VFTIndex, int SearchRange) {
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Walking VFT searching for ")) + std::string(TargetFunctionName) + std::string(skCrypt(" VFT index")));

	for (int i = 0; !VFTIndex && i < SearchRange; ++i) {
		if (VFT[i] == TargetFunction) {
			VFTIndex = i;
			break;
		}
	}

	if (VFTIndex) {
		DEBUG_LOG(LOG_OFFSET, std::string(TargetFunctionName) + std::string(skCrypt(" VFT index found: ")) + std::to_string(VFTIndex));
	}
	else {
		THROW_ERROR(std::string(skCrypt("Failed to find ")) + std::string(TargetFunctionName) + std::string(skCrypt(" VFT index!")), CRASH_ON_NOT_FOUND);
	}
}
void SDKInitializer::InitVFTIndex(const char* VFTName, std::vector<const char*> PossibleSigs, const wchar_t* SearchString, uintptr_t& VFTIndex, int SearchRange, int SearchBytesBehind) {
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Searching for ")) + std::string(VFTName) + std::string(skCrypt(" VFT index")));

	uint8_t* StringRef = Memory::FindByStringInAllSections(SearchString);

	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Searching for VFT Index offset")));

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
		DEBUG_LOG(LOG_OFFSET, std::string(VFTName) + std::string(skCrypt(" VFT Index offset found: ")) + std::to_string(VFTIndex));
	}
	else {
		THROW_ERROR(std::string(skCrypt("Failed to find ")) + std::string(VFTName) + std::string(skCrypt(" VFT Index!")), CRASH_ON_NOT_FOUND);
	}
}

void SDKInitializer::InitFunctionOffset(const char* FunctionName, std::vector<const char*> PossibleSigs, const wchar_t* SearchString, uintptr_t& FunctionOffset, int SearchRange, int SearchBytesBehind) {
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Searching for ")) + std::string(FunctionName) + std::string(skCrypt(" function offset")));

	uint8_t* StringRef = Memory::FindByStringInAllSections(SearchString);

	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Searching for function offset")));

	for (int i = 0; !FunctionOffset && i < PossibleSigs.size(); ++i) {
		FunctionOffset = reinterpret_cast<uintptr_t>(
			Memory::FindPatternInRange(PossibleSigs[i], StringRef - SearchBytesBehind, SearchRange, true, -1)
			);
	}

	PossibleSigs.clear();

	if (FunctionOffset) {
		FunctionOffset -= SDK::GetBaseAddress();
		DEBUG_LOG(LOG_OFFSET, std::string(FunctionName) + std::string(skCrypt(" function offset found: ")) + std::to_string(FunctionOffset));
	}
	else {
		THROW_ERROR(std::string(skCrypt("Failed to find ")) + std::string(FunctionName) + std::string(skCrypt(" function offset!")), CRASH_ON_NOT_FOUND);
	}
}
void SDKInitializer::InitFunctionOffset(const char* FunctionName, std::vector<const char*> PossibleSigs, const char* SearchString, uintptr_t& FunctionOffset, int SearchRange, int SearchBytesBehind) {
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Searching for ")) + std::string(FunctionName) + std::string(skCrypt(" function offset")));
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Searching for string reference: ")) + std::string(SearchString));

	uint8_t* StringRef = Memory::FindByStringInAllSections(SearchString);

	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Searching for function offset")));

	for (int i = 0; !FunctionOffset && i < PossibleSigs.size(); ++i) {
		FunctionOffset = reinterpret_cast<uintptr_t>(
			Memory::FindPatternInRange(PossibleSigs[i], StringRef - SearchBytesBehind, SearchRange, true, -1)
			);
	}

	PossibleSigs.clear();

	if (FunctionOffset) {
		FunctionOffset -= SDK::GetBaseAddress();
		DEBUG_LOG(LOG_OFFSET, std::string(FunctionName) + std::string(skCrypt(" function offset found: ")) + std::to_string(FunctionOffset));
	}
	else {
		THROW_ERROR(std::string(skCrypt("Failed to find ")) + std::string(FunctionName) + std::string(skCrypt(" function offset!")), CRASH_ON_NOT_FOUND);
	}
}

void SDKInitializer::InitPRIndex() {
	void** Vft = nullptr;

	for (int i = 0; i < SDK::UObject::ObjectArray.Num(); i++)
	{
		SDK::UObject* Obj = SDK::UObject::ObjectArray.GetByIndex(i);

		if (!Obj)
			continue;

		if (Obj->IsA(SDK::UGameViewportClient::StaticClass()) && Obj->IsDefaultObject() == false)
		{
			Vft = static_cast<SDK::UGameViewportClient*>(Obj)->Vft;
			break;
		}
	}

	if (Vft == nullptr) {
		THROW_ERROR(std::string(skCrypt("Failed to find VFT for UGameViewportClient!")), false);
	}

	uintptr_t GameInstance = 0;

	std::vector<FunctionSearch> Functions = {};
	std::vector<OffsetSearch> Offsets = { OffsetSearch{ std::string(skCrypt("GameViewportClient")), std::string(skCrypt("GameInstance")), &GameInstance, nullptr }};

	SDK::UObject::SetupObjects(Functions, Offsets);

	int bSuppressTransitionMessage = (int)GameInstance + sizeof(void*);

	//DEBUG_LOG(LOG_INFO, std::string(skCrypt("INT: ")) + std::to_string(bSuppressTransitionMessage));

	auto Resolve32BitRelativeJump = [](void* FunctionPtr) -> uint8_t*
	{
		uint8_t* Address = reinterpret_cast<uint8_t*>(FunctionPtr);
		if (*Address == 0xE9)
		{
			uint8_t* Ret = ((Address + 5) + *reinterpret_cast<int32_t*>(Address + 1));

			if (Memory::IsInProcessRange(uintptr_t(Ret)))
				return Ret;
		}

		return reinterpret_cast<uint8_t*>(FunctionPtr);
	};

	for (int i = 0; i < 0x150; i++)
	{
		//DEBUG_LOG(LOG_INFO, std::string(skCrypt("Searching for ProcessEvent VFT index: ")) + std::to_string(i) + " -- " + std::to_string((uintptr_t)Vft[i] - SDK::GetBaseAddress()));

		if (!Vft[i] || !Memory::IsInProcessRange(reinterpret_cast<uintptr_t>(Vft[i])))
			continue;

		//DEBUG_LOG(LOG_INFO, std::string(skCrypt("CHECKING !!!! Searching for ProcessEvent VFT index: ")) + std::to_string(i) + " -- " + std::to_string((uintptr_t)Vft[i] - SDK::GetBaseAddress()));

		if (Memory::FindPatternInRange({ 0x80, 0xB9, bSuppressTransitionMessage, 0x00, 0x00, 0x00, 0x00 }, Resolve32BitRelativeJump(Vft[i]), 0x35))
		{
			SDK::Cached::VFT::PostRender = i;
			DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("PostRender VFT index found: ")) + std::to_string(SDK::Cached::VFT::PostRender));

			return;
		}
	}

	if (SDK::Cached::VFT::PostRender == 0x0) {
		THROW_ERROR(std::string(skCrypt("Failed to find PostRender VFT index!")), CRASH_ON_NOT_FOUND);
	}
}
void SDKInitializer::InitPEIndex() {
	void** Vft = SDK::UObject::ObjectArray.GetByIndex(0)->Vft;

	auto Resolve32BitRelativeJump = [](void* FunctionPtr) -> uint8_t*
	{
		uint8_t* Address = reinterpret_cast<uint8_t*>(FunctionPtr);
		if (*Address == 0xE9)
		{
			uint8_t* Ret = ((Address + 5) + *reinterpret_cast<int32_t*>(Address + 1));

			if (Memory::IsInProcessRange(uintptr_t(Ret)))
				return Ret;
		}

		return reinterpret_cast<uint8_t*>(FunctionPtr);
	};

	for (int i = 0; i < 0x150; i++)
	{
		if (!Vft[i] || !Memory::IsInProcessRange(reinterpret_cast<uintptr_t>(Vft[i])))
			break;

		if (Memory::FindPatternInRange({ 0xF7, -0x1, (int32_t)SDK::UFunction::FunctionFlagsOffset, 0x0, 0x0, 0x0, 0x0, 0x04, 0x0, 0x0 }, Resolve32BitRelativeJump(Vft[i]), 0x400)
			&& Memory::FindPatternInRange({ 0xF7, -0x1, (int32_t)SDK::UFunction::FunctionFlagsOffset, 0x0, 0x0, 0x0, 0x0, 0x0, 0x40, 0x0 }, Resolve32BitRelativeJump(Vft[i]), 0x400))
		{
			SDK::Cached::VFT::ProcessEvent = i;
			DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("ProcessEvent VFT index found: ")) + std::to_string(SDK::Cached::VFT::ProcessEvent));
			return;
		}
	}

	uint8_t* StringRef = Memory::FindByStringInAllSections(skCrypt(L"Accessed None"));
	uintptr_t NextFunctionStart = Memory::FindNextFunctionStart(StringRef);

	SDKInitializer::WalkVFT(skCrypt("ProcessEvent"), SDK::UObject::ObjectArray.GetByIndex(0)->Vft, reinterpret_cast<void*>(NextFunctionStart), SDK::Cached::VFT::ProcessEvent, 0x150);
}
void SDKInitializer::InitGPVIndex() {
	void** Vft = nullptr;

	for (int i = 0; i < SDK::UObject::ObjectArray.Num(); i++)
	{
		SDK::UObject* Obj = SDK::UObject::ObjectArray.GetByIndex(i);

		if (!Obj)
			continue;

		if (Obj->IsA(SDK::APlayerController::StaticClass()) && Obj->IsDefaultObject() == false)
		{
			Vft = static_cast<SDK::APlayerController*>(Obj)->Vft;
			break;
		}
	}

	if (Vft == nullptr) {
		THROW_ERROR(std::string(skCrypt("Failed to find VFT for APlayerController!")), CRASH_ON_NOT_FOUND);
	}

	auto Resolve32BitRelativeJump = [](void* FunctionPtr) -> uint8_t*
	{
		uint8_t* Address = reinterpret_cast<uint8_t*>(FunctionPtr);
		if (*Address == 0xE9)
		{
			uint8_t* Ret = ((Address + 5) + *reinterpret_cast<int32_t*>(Address + 1));

			if (Memory::IsInProcessRange(uintptr_t(Ret)))
				return Ret;
		}

		return reinterpret_cast<uint8_t*>(FunctionPtr);
	};

	for (int i = 0; i < 0x150; i++)
	{
		if (!Vft[i] || !Memory::IsInProcessRange(reinterpret_cast<uintptr_t>(Vft[i])))
			continue;

		if (// Signature for most builds
			(Memory::FindPatternInRange({ 0x80, 0xBB, -0x01, -0x01, 0x00, 0x00, 0x03 }, Resolve32BitRelativeJump(Vft[i]), 0x70)
			&& Memory::FindPatternInRange({ 0x84, 0xC0 }, Resolve32BitRelativeJump(Vft[i]), 0x70)
			&& Memory::FindPatternInRange({ 0x48, 0x8B, 0xCB }, Resolve32BitRelativeJump(Vft[i]), 0x70)
				)

			// Signature for builds where Fortnite added their own wrapper for it
			|| (Memory::FindPatternInRange({ 0x48, 0x83, 0xEC, 0x30, 0x48, 0x8B, 0x81, -0x01, -0x01, -0x01, -0x01, 0x49, 0x8B, 0xD8, 0x48, 0x85, 0xC0 }, Resolve32BitRelativeJump(Vft[i]), 0x70)
				)

			// Another general signature
			|| (Memory::FindPatternInRange({ 0x0F, 0x10, 0x89, -0x01, -0x01, 0x00, 0x00 }, Resolve32BitRelativeJump(Vft[i]), 0x70)
			&& Memory::FindPatternInRange({ 0x0F, 0xC6, -0x01, -0x01, 0x0F, 0xC6, -0x01, -0x01 }, Resolve32BitRelativeJump(Vft[i]), 0x70)
				)
			
			// Signature for UE5 builds (19.00+)
			|| (SDK::GetGameVersion() >= 19.00 && Memory::FindPatternInRange({ 0x48, 0x81, 0xEC, -0x01, -0x01, 0x00, 0x00 }, Resolve32BitRelativeJump(Vft[i]), 0x70)
				&& (Memory::FindPatternInRange({ 0x44, 0x0F, -0x01, -0x01, -0x01, -0x01, 0x44, 0x0F, -0x01, -0x01, -0x01, -0x01 }, Resolve32BitRelativeJump(Vft[i]), 0x70)
					|| Memory::FindPatternInRange({ 0x44, 0x0F, -0x01, -0x01, -0x01, 0x44, 0x0F, -0x01, -0x01, -0x01 }, Resolve32BitRelativeJump(Vft[i]), 0x70))
				)
			)
		{
			SDK::Cached::VFT::GetPlayerViewpoint = i;
			DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("GetPlayerViewpoint VFT index found: ")) + std::to_string(SDK::Cached::VFT::GetPlayerViewpoint));

			return;
		}
	}

	if (SDK::Cached::VFT::GetPlayerViewpoint == 0x0) {
		THROW_ERROR(std::string(skCrypt("Failed to find GetPlayerViewpoint VFT index!")), CRASH_ON_NOT_FOUND);
	}
}
void SDKInitializer::InitGVIndex() {
	InitVFTIndex(
		skCrypt("GetViewpoint"),
		std::vector<const char*>{ skCrypt("FF 90 ? ? ? ?") },
		skCrypt(L"STAT_CalcSceneView"),
		SDK::Cached::VFT::GetViewpoint,
		0x400);
}
void SDKInitializer::InitGetWeaponStatsIndex(const SDK::UObject* WeaponActor) {
	// Visual explanation: https://imgur.com/a/BnH41YR
	// 
	// VFT-Base: 0x40CE3E0
	// VFT-Index-Address: 0x40CEA60
	// VFT-Index: ((VFT-Index-Address - VFT-Base) / 8) = 0xD0

	// Since we require a AFortWeapon instance to find the VFT index
	// we have to check if we've already tried to avoid insane lag
	static bool HasTried = false;
	if (HasTried) {
		return;
	}
	HasTried = true;

	void** Vft = WeaponActor->Vft;

	if (Vft == nullptr) {
		THROW_ERROR(std::string(skCrypt("Failed to find VFT for AFortWeapon!")), CRASH_ON_NOT_FOUND);
	}

	auto Resolve32BitRelativeJump = [](void* FunctionPtr) -> uint8_t*
	{
		uint8_t* Address = reinterpret_cast<uint8_t*>(FunctionPtr);
		if (*Address == 0xE9)
		{
			uint8_t* Ret = ((Address + 5) + *reinterpret_cast<int32_t*>(Address + 1));

			if (Memory::IsInProcessRange(uintptr_t(Ret)))
				return Ret;
		}

		return reinterpret_cast<uint8_t*>(FunctionPtr);
	};

	for (int i = 0; i < 0x100; i++)
	{
		if (!Vft[i] || !Memory::IsInProcessRange(reinterpret_cast<uintptr_t>(Vft[i])))
			continue;

		// 48 83 EC 58 48 8B 91 ? ? ? ? 48 85 D2 0F 84 ? ? ? ?
		// 48 83 EC 58 48 8B 89 ? ? ? ? 48 85 C9 0F 84 ? ? ? ?
		if (Memory::FindPatternInRange({ 0x48, 0x83, 0xEC, 0x58, 0x48, 0x8B, -0x01, -0x01, -0x01, -0x01, -0x01, 0x48, 0x85 }, Resolve32BitRelativeJump(Vft[i]), 0x50)
			|| Memory::FindPatternInRange({ 0x48, 0x8B, 0x89, -0x01, -0x01, 0x00, 0x00, 0x48, -0x01, -0x01, 0x74, 0x13 }, Resolve32BitRelativeJump(Vft[i]), 0x50))
		{
			SDK::Cached::VFT::GetWeaponStats = i;
			DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("GetWeaponStats VFT index found: ")) + std::to_string(SDK::Cached::VFT::GetWeaponStats));

			return;
		}
	}

	if (SDK::Cached::VFT::GetWeaponStats == 0x0) {
		THROW_ERROR(std::string(skCrypt("Failed to find GetWeaponStats VFT index!")), CRASH_ON_NOT_FOUND);
	}
}

void SDKInitializer::InitAppendString() {
	// FName::AppendString documentation: https://docs.unrealengine.com/4.26/en-US/API/Runtime/Core/UObject/FName/AppendString/1/
	// "ForwardShadingQuality_" reference in UE source: https://github.com/EpicGames/UnrealEngine/blob/4.26/Engine/Source/Runtime/MaterialShaderQualitySettings/Private/MaterialShaderQualitySettings.cpp#L55
	// Visual explanation: https://imgur.com/a/mCe1SF6
	// 
	// AppendString is used to convert an FName to an FString.
	// It is very easy to find as in nearly ever Unreal Engine game it is
	// called directly after the reference of "ForwardShadingQuality_"

	InitFunctionOffset(
		skCrypt("AppendString"),
		std::vector<const char*>
	{
		skCrypt("48 8D ? ? 48 8D ? ? E8"),
		skCrypt("48 8D ? ? ? 48 8D ? ? E8"),
		skCrypt("48 8D ? ? 49 8B ? E8"),
		skCrypt("48 8D ? ? ? 49 8B ? E8")
	},
		skCrypt("ForwardShadingQuality_"),
		SDK::Cached::Functions::AppendString,
		0x60);
}
void SDKInitializer::InitFNameConstructor() {
	InitFunctionOffset(
		skCrypt("FName Constructor"),
		std::vector<const char*> { skCrypt("E8") },
		skCrypt(L"CanvasObject"),
		SDK::Cached::Functions::FNameConstructor,
		0x32);
}
void SDKInitializer::InitLineTraceSingle() {
#if SEASON_20_PLUS
	for (int i = 0; i < 1000; i++) {
		SDK::Cached::Functions::LineTraceSingle = Memory::PatternScan(
			SDK::GetBaseAddress(),
			skCrypt("44 24 ? E8 ? ? ? ? 0F 10 45 80 4C 39 6B 20 4C 8D 45 D0 F2 0F 10 4D ? 48 8D 55 F0 44 0F B6 8D ? ? ? ? 49 8B C5 48 8B 4C 24 ? 0F 95 C0 48 01 43 20 44 88 74 24 ? 0F 29 45 D0 0F 10 45 98 48 89 7C 24 ? F2 0F 11 4D ? F2 0F 10 4D ? 48 89 74 24 ? 0F 29 45 F0 F2 0F 11 4D ? 44 88 7C 24 ? E8 ? ? ? ?"),
			i, // magic number, will improve pattern scanning function later
			true
		);

		DEBUG_LOG(LOG_INFO, std::to_string(i) + std::string(skCrypt(" - ")) + std::string(skCrypt("LineTraceSingle: ")) + std::to_string(SDK::Cached::Functions::LineTraceSingle - SDK::GetBaseAddress()));
	}
#else
	SDK::Cached::Functions::LineTraceSingle = Memory::PatternScan(
		SDK::GetBaseAddress(),
		skCrypt("48 8B 43 20 48 8D 4D E0 0F 10 45 C0 44 0F B6 8D ? ? ? ? 48 85 C0 0F 10 4D D0 4C 8D 45 A0 40 0F 95 C7 66 0F 7F 45 ? 48 8D 55 B0 F2 0F 10 45 ? 48 03 F8 8B 45 88 F2 0F 11 45 ? F2 0F 10 45 ? 89 45 A8 8B 45 98 F2 0F 11 45 ? F3 0F 10 44 24 ? F3 0F 11 44 24 ? 48 89 4C 24 ? 48 8D 4D F0 48 89 4C 24 ? 48 8B 4C 24 ? 44 88 7C 24 ? 48 89 74 24 ? 89 45 B8 0F B6 85 ? ? ? ? 89 44 24 30 4C 89 74 24 ? 44 88 64 24 ? 48 89 7B 20 66 0F 7F 4D ? E8"),
		155, // magic number, will improve pattern scanning function later
		true
	);

	if (SDK::Cached::Functions::LineTraceSingle == 0x0) {
		SDK::Cached::Functions::LineTraceSingle = Memory::PatternScan(
			SDK::GetBaseAddress(),
			skCrypt("4C 39 6B 20 4C 8D 45 A0 F2 0F 10 45 ? 48 8D 55 B0 44 0F B6 8D ? ? ? ? 49 8B C5 48 8B 4C 24 ? 0F 95 C0 48 01 43 20 8B 45 88 44 88 74 24 ? F2 0F 11 45 ? F2 0F 10 45 ? 89 45 A8 8B 45 98 48 89 7C 24 ? 48 89 74 24 ? 89 45 B8 F2 0F 11 45 ? 44 88 7C 24 ? E8 ? ? ? ?"),
			92, // magic number, will improve pattern scanning function later
			true
		);
	}
#endif

	if (SDK::Cached::Functions::LineTraceSingle == 0x0) {
		THROW_ERROR(std::string(skCrypt("Failed to find LineTraceSingle!")), CRASH_ON_NOT_FOUND);
	}

	SDK::Cached::Functions::LineTraceSingle -= SDK::GetBaseAddress();

	DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("LineTraceSingle function offset found: ")) + std::to_string(SDK::Cached::Functions::LineTraceSingle));
}

void SDKInitializer::InitGObjects() {
	DEBUG_LOG(LOG_INFO, std::string(skCrypt("Searching for GObjects...")));

	SDK::UObject::ObjectArray.IsChunked = true;

	uintptr_t TUObjectArray = Memory::PatternScan(
		SDK::GetBaseAddress(),
		skCrypt("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1"),
		7,
		true
	);

	if (SDK::IsValidPointer((void*)TUObjectArray) == false) {
		SDK::UObject::ObjectArray.IsChunked = false;

		TUObjectArray = Memory::PatternScan(
			SDK::GetBaseAddress(),
			skCrypt("48 8B 05 ? ? ? ? 48 8D 14 C8 EB 02"),
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

	if (SDK::IsValidPointer((void*)TUObjectArray)) {
		DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("GObjects offset found: ")) + std::to_string(TUObjectArray - SDK::GetBaseAddress()));
	}
	else {
		THROW_ERROR(std::string(skCrypt("Failed to find GObjects!")), CRASH_ON_NOT_FOUND);
	}
}