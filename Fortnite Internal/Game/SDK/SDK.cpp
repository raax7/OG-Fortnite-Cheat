#include "SDK.h"
#include "SDKInitializer.h"

#include "../../Globals.h"

#include "../../Utilities/Logger.h"
#include "../../Utilities/skCrypter.h"
#include "Classes/CoreUObject_classes.h"
#include "Classes/Engine_classes.h"

#include "../Input/Input.h"
#include "../Features/FortPawnHelper/Bone.h"

bool SDK::IsValidPointer(uintptr_t Address) {
	if (!Address) {
		return false;
	}

	// IMPROVVE THIS!!! IsBadWritePtr is a very bad and obselete win api func
	//if (LI_FN(IsBadWritePtr).safe_cached()(&Address, 8)) {
	//	return false;
	//}

	return true;
}

uintptr_t SDK::GetBaseAddress() {
	return *(uintptr_t*)(__readgsqword(0x60) + 0x10);
}

void SDK::Init() {
	DEBUG_LOG(skCrypt("Initializing SDK...").decrypt());

	// Init Offsets, Functions, and VFT Indexes
	{
		// Init GObjects
		SDKInitializer::InitGObjects();

		// Init Functions
		SDKInitializer::InitAppendString();
		SDKInitializer::InitFNameConstructor();
		SDKInitializer::InitLineTraceSingle();

		// Init VFT Indexes
		SDKInitializer::InitPRIndex();
		SDKInitializer::InitPEIndex();
		SDKInitializer::InitGPVIndex();
		SDKInitializer::InitGVIndex();

		// Init Class Offsets
		SDK::UProperty::OffsetOffset		= 0x44;
		SDK::UClass::DefaultObjectOffset	= SDKInitializer::FindDefaultObjectOffset();
		SDK::UClass::CastFlagsOffset		= SDKInitializer::FindCastFlagsOffset();
		SDK::UStruct::SuperOffset			= SDKInitializer::FindSuperOffset();
		SDK::UStruct::ChildPropertiesOffset = SDKInitializer::FindChildPropertiesOffset();
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

		DEBUG_LOG(skCrypt("Game Version: ").decrypt() + std::to_string(GetGameVersion()));

		// TEMP
		if (GetGameVersion() >= 12.00) {
			THROW_ERROR(skCrypt("Season 12+ will encounter an error in a few seconds! However the cheat will still work fine, press OK.").decrypt(), false);
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
			FunctionSearch { skCrypt("PlayerCameraManager").decrypt(),	skCrypt("GetFOVAngle").decrypt(),				&SDK::Cached::Functions::PlayerCameraManager::GetFOVAngle },
			FunctionSearch { skCrypt("PlayerController").decrypt(),		skCrypt("IsInputKeyDown").decrypt(),			&SDK::Cached::Functions::PlayerController::IsInputKeyDown },
			FunctionSearch { skCrypt("PlayerController").decrypt(),		skCrypt("WasInputKeyJustReleased").decrypt(),	&SDK::Cached::Functions::PlayerController::WasInputKeyJustReleased },
			FunctionSearch { skCrypt("PlayerController").decrypt(),		skCrypt("WasInputKeyJustPressed").decrypt(),	&SDK::Cached::Functions::PlayerController::WasInputKeyJustPressed },
			FunctionSearch { skCrypt("PlayerController").decrypt(),		skCrypt("GetMousePosition").decrypt(),			&SDK::Cached::Functions::PlayerController::GetMousePosition },
			FunctionSearch { skCrypt("Controller").decrypt(),			skCrypt("ClientSetRotation").decrypt(),			&SDK::Cached::Functions::PlayerController::ClientSetRotation },
			FunctionSearch { skCrypt("KismetSystemLibrary").decrypt(),	skCrypt("LineTraceSingle").decrypt(),			&SDK::Cached::Functions::KismetSystemLibrary::LineTraceSingle },
			FunctionSearch { skCrypt("PlayerState").decrypt(),			skCrypt("GetPlayerName").decrypt(),				&SDK::Cached::Functions::PlayerState::GetPlayerName },
			FunctionSearch { skCrypt("SkinnedMeshComponent").decrypt(),	skCrypt("GetBoneName").decrypt(),				&SDK::Cached::Functions::SkinnedMeshComponent::GetBoneName },
			FunctionSearch { skCrypt("SceneComponent").decrypt(),		skCrypt("GetSocketLocation").decrypt(),			&SDK::Cached::Functions::SkinnedMeshComponent::GetSocketLocation },
		};

		std::vector<OffsetSearch> Offsets{
			OffsetSearch { skCrypt("Engine").decrypt(),					skCrypt("GameViewport").decrypt(),				&SDK::Cached::Offsets::Engine::GameViewport,					OffsetType::Class},
			OffsetSearch { skCrypt("GameViewportClient").decrypt(),		skCrypt("World").decrypt(),						&SDK::Cached::Offsets::GameViewportClient::World,				OffsetType::Class },
			OffsetSearch { skCrypt("GameViewportClient").decrypt(),		skCrypt("GameInstance").decrypt(),				&SDK::Cached::Offsets::GameViewportClient::GameInstance,		OffsetType::Class },
			OffsetSearch { skCrypt("GameInstance").decrypt(),			skCrypt("LocalPlayers").decrypt(),				&SDK::Cached::Offsets::GameInstance::LocalPlayers,				OffsetType::Class },
			OffsetSearch { skCrypt("Player").decrypt(),					skCrypt("PlayerController").decrypt(),			&SDK::Cached::Offsets::Player::PlayerController,				OffsetType::Class },
			OffsetSearch { skCrypt("PlayerController").decrypt(),		skCrypt("AcknowledgedPawn").decrypt(),			&SDK::Cached::Offsets::PlayerController::AcknowledgedPawn,		OffsetType::Class },
			OffsetSearch { skCrypt("PlayerController").decrypt(),		skCrypt("PlayerCameraManager").decrypt(),		&SDK::Cached::Offsets::PlayerController::PlayerCameraManager,	OffsetType::Class },
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

	Input::Init();
	Features::FortPawnHelper::Bone::Init();

	DEBUG_LOG(skCrypt("SDK Initialized!").decrypt());

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

#if 0
	// GENERATE PSEUDO SOURCE CODE

	std::string Path = "C:\\Users\\raax\\Desktop\\Fortnite-Main\\test\\";

	struct ObjectInfo {
		SDK::UObject* Object;
		SDK::UClass* Class;
		std::string PackageName;
		std::string Name;
	};

	struct Package {
		SDK::UObject* Object;
		std::string Name;
	};

	struct Class {
		SDK::UObject* Object;
		std::string PackageName;
		std::string Name;
		std::string ClassName;
		int NumFuncs;
		int NumProps;
	};

	std::vector<Package> Packages;
	std::vector<ObjectInfo> Objects;
	std::vector<Class> Classes;

	DEBUG_LOG("Objects: " + std::to_string(SDK::UObject::ObjectArray.Num()));

	// populate objects
	for (int i = 0; i < SDK::UObject::ObjectArray.Num(); i++) {
		SDK::UObject* Object = SDK::UObject::ObjectArray.GetByIndex(i);
		if (Object == nullptr) continue;

		std::string ObjName = Object->GetFullName();

		///bool startswitclass = ObjName.starts_with(skCrypt("Class ").decrypt());
		if (ObjName.starts_with(skCrypt("Class ").decrypt())) {
			// example full obj name "Class FortniteUI.FortUIStateWidget_Frontend"
			// what we want from that "FortniteUI"

			size_t dotPos = ObjName.find_first_of('.');
			if (dotPos != std::string::npos) {
				std::string PackageName = ObjName.substr(6, dotPos - 6); // 6 is the length of "Class "
				std::string ClassName = ObjName.substr(dotPos + 1);
				Classes.push_back({ Object, PackageName, ObjName, ClassName, 0, 0 });
			}
		}
		else if (ObjName.starts_with(skCrypt("Package ").decrypt())) {
			Packages.push_back({ Object, ObjName.substr(8) });
		}
		else {
			if (ObjName.find(skCrypt("Property ").decrypt())) {
				std::string PackageName;

				// Find the position of the first dot ('.') in the property name
				size_t dotPosition = ObjName.find('.');

				// Extract the substring after the first dot
				std::string afterDot = ObjName.substr(dotPosition + 1);

				// Find the position of the second dot ('.') in the substring
				size_t secondDotPosition = afterDot.find('.');

				// Extract the substring before the second dot
				std::string extractedName = afterDot.substr(0, secondDotPosition);

				Objects.push_back({ Object, Object->Class, extractedName, ObjName });
			}
			else if (ObjName.find(skCrypt("Function ").decrypt())) {
				std::string PackageName;

				// Find the position of the first dot ('.') in the property name
				size_t dotPosition = ObjName.find('.');

				// Extract the substring after the first dot
				std::string afterDot = ObjName.substr(dotPosition + 1);

				// Find the position of the second dot ('.') in the substring
				size_t secondDotPosition = afterDot.find('.');

				// Extract the substring before the second dot
				std::string extractedName = afterDot.substr(0, secondDotPosition);

				Objects.push_back({ Object, Object->Class, extractedName, ObjName });
			}
		}
	}

	for (auto& Class : Classes) {
		//DEBUG_LOG("Package Name: " + Class.PackageName + " Class Name: " + Class.Name + " NumFuncs: " + std::to_string(Class.NumFuncs) + " NumProps: " + std::to_string(Class.NumProps));
	}

	DEBUG_LOG("Objects 2: " + std::to_string(Objects.size()));

	int count = 0;

	// populate classes
	for (auto& Object : Objects) {
		count++;

		// display progress every 1k objects
		if (count % 1000 == 0) {
			DEBUG_LOG("Progress: " + std::to_string(count));
		}

		if (Object.Name.starts_with(skCrypt("Function ").decrypt())) {
			for (auto& Class : Classes) {
				if (Class.PackageName != Object.PackageName) continue;

				DEBUG_LOG("Class Package: " + Class.PackageName);
				DEBUG_LOG("Object Package: " + Object.PackageName);
				DEBUG_LOG("");

				if (Object.Name.starts_with("Function " + Class.PackageName + "." + Class.ClassName)) {
					Class.NumFuncs++;
					DEBUG_LOG("Function " + Class.ClassName + " " + std::to_string(Class.NumFuncs));
					break;
				}
			}
		}
		else {
			for (auto& Class : Classes) {
				if (Class.PackageName != Object.PackageName) continue;

				//DEBUG_LOG("NOT YET Property " + Class.ClassName + " (" + Object.Name + ") " + std::to_string(Class.NumProps));

				if (Object.Name.find("." + Class.ClassName) != std::string::npos) {
					Class.NumProps++;
					//DEBUG_LOG("Property " + Class.ClassName + " (" + Object.Name + ") " + std::to_string(Class.NumProps));
					break;
				}
			}
		}
	}

	DEBUG_LOG("Classes: " + std::to_string(Classes.size()));

	// create folders by package
	for (auto& Package : Packages) {
		std::string PackageName = Package.Name;

		for (auto& Class : Classes) {
			if (Class.Name.starts_with("Class " + PackageName)) {
				DEBUG_LOG(skCrypt("Package - ").decrypt() + PackageName);

				Packages.push_back({ Class.Object, PackageName });

				// create folder
				std::string FolderPath = Path + PackageName;
				std::string FolderPath2 = Path + PackageName + "\\Private";
				std::string FolderPath23 = Path + PackageName + "\\Public";
				CreateDirectoryA(FolderPath.c_str(), NULL);
				CreateDirectoryA(FolderPath2.c_str(), NULL);
				CreateDirectoryA(FolderPath23.c_str(), NULL);

				// create Build.CS file
				std::string Path = FolderPath + "\\" + PackageName + ".Build.cs";
				std::ofstream BuildCSFile(Path);
				BuildCSFile << "using UnrealBuildTool;\n";
				BuildCSFile << "using System.Collections.Generic;\n";
				BuildCSFile << "public class " + PackageName + " : ModuleRules\n";
				BuildCSFile << "{\n";
				BuildCSFile << "    public " + PackageName + "(ReadOnlyTargetRules Target) : base(Target)\n";
				BuildCSFile << "    {\n";
				BuildCSFile << "        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;\n";
				BuildCSFile << "        PublicIncludePaths.AddRange(new string[] { \"" + PackageName + "/Public\" });\n";
				BuildCSFile << "        PrivateIncludePaths.AddRange(new string[] { \"" + PackageName + "/Private\" });\n";
				BuildCSFile << "        PublicDependencyModuleNames.AddRange(new string[] { \"Core\", \"CoreUObject\", \"Engine\", \"InputCore\", \"HeadMountedDisplay\" });\n";
				BuildCSFile << "        PrivateDependencyModuleNames.AddRange(new string[] {  });\n";
				BuildCSFile << "        DynamicallyLoadedModuleNames.AddRange(new string[] {  });\n";
				BuildCSFile << "    }\n";
				BuildCSFile << "}\n";
				BuildCSFile.close();

				break;
			}
		}

		if (Package.Name == "FortniteUI") {
			break;
		}
	}

	DEBUG_LOG("Packages: " + std::to_string(Packages.size()));

	// create classes
	for (auto& Class : Classes) {
		for (auto& Package : Packages) {
			if (Class.Name.starts_with("Class " + Package.Name)) {
				std::string ClassName = Class.Object->GetName();

				DEBUG_LOG(skCrypt("Class (").decrypt() + Package.Name + ") - " + ClassName + " NUMS: " + std::to_string(Class.NumFuncs) + " " + std::to_string(Class.NumProps));

				// create the class CPP and H file

				// CPP
				std::string PathCPP = Path + Package.Name + "\\Private\\" + ClassName + ".cpp";
				std::ofstream CPPFile(PathCPP);
				CPPFile << "#include \"" + Package.Name + ".h\"\n";
				CPPFile << "\n";
				CPPFile << ClassName + "::" + ClassName + "()\n";
				CPPFile << "{\n";
				CPPFile << "}\n";

				// add random bmments to bump the size depending on Ufunction and Uproperty count
				for (int i = 0; i < Class.NumFuncs; i++) {
					CPPFile << "\n// Function " + ClassName + " " + std::to_string(i) + "\n";
					CPPFile << "void " + ClassName + "::Function" + std::to_string(i) + "() {";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n// THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!! THIS IS TO FILL UP SPACE!!\n";
					CPPFile << "\n}";
				}


				CPPFile.close();

				// H
				PathCPP = Path + Package.Name + "\\Public\\" + ClassName + ".h";
				std::ofstream HFile(PathCPP);
				HFile << "#pragma once\n";
				HFile << "#include \"CoreMinimal.h\"\n";
				HFile << "#include \"UObject/NoExportTypes.h\"\n";
				HFile << "#include \"" + ClassName + ".generated.h\"\n";
				HFile << "\n";
				HFile << "UCLASS()\n";
				HFile << "class " + Package.Name + " : public UObject\n";
				HFile << "{\n";
				HFile << "    GENERATED_BODY()\n";
				HFile << "\n";
				HFile << "public:\n";
				HFile << "    " + ClassName + "();\n";
				for (int i = 0; i < Class.NumFuncs; i++) {
					// add funcs
					HFile << "    UFUNCTION()\n";
					HFile << "    void Function" + std::to_string(i) + "();\n";

				}
				for (int i = 0; i < Class.NumProps; i++) {
					HFile << "	\n// Property " + ClassName + " " + std::to_string(i) + " (THIS IS TO FILL UP SPACE!!) (THIS IS TO FILL UP SPACE!!) (THIS IS TO FILL UP SPACE!!) (THIS IS TO FILL UP SPACE!!) (THIS IS TO FILL UP SPACE!!)\n";
					HFile << "    UPROPERTY()\n";
					HFile << "	int " + ClassName + "::Property" + std::to_string(i) + " = 0;\n";
				}
				HFile << "};\n";
				HFile.close();

				break;
			}
		}
	}

	DEBUG_LOG("Classes: " + std::to_string(Classes.size()));
#endif
}