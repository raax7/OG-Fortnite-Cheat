#include "Engine_classes.h"

#include "CoreUObject_classes.h"

#include "../../../Utilities/Error.h"
#include "../../Features/FortPawnHelper/Bone.h"
#include "../../Game.h"

// Classes

SDK::FName SDK::USkeletalMeshComponent::GetBoneName(int32 BoneIndex) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return FName{};

	struct {
		int32 BoneIndex;

		FName return_value;
	} params_GetBoneName{};

	params_GetBoneName.BoneIndex = BoneIndex;

	this->ProcessEvent(SDK::Cached::Functions::SkinnedMeshComponent::GetBoneName, &params_GetBoneName);

	return params_GetBoneName.return_value;
}
SDK::FVector SDK::USkeletalMeshComponent::GetSocketLocation(FName InSocketName) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return FVector();

	struct {
		FName InSocketName;

		FVector return_value;
	} params_GetSocketLocation{};

	params_GetSocketLocation.InSocketName = InSocketName;

	this->ProcessEvent(SDK::Cached::Functions::SkinnedMeshComponent::GetSocketLocation, &params_GetSocketLocation);

	return params_GetSocketLocation.return_value;
}

SDK::FString SDK::APlayerState::GetPlayerName() {
	if (!SDK::IsValidPointer((uintptr_t)this)) return FString{};

	struct {
		FString return_value;
	} params_GetPlayerName{};

	this->ProcessEvent(SDK::Cached::Functions::PlayerState::GetPlayerName, &params_GetPlayerName);

	return params_GetPlayerName.return_value;
}

SDK::FVector SDK::APlayerCameraManager::GetCameraLocation() {
	if (!this) return SDK::FVector();

	struct {
		SDK::FVector        return_value;
	} params_GetCameraLocation{};

	this->ProcessEvent(SDK::Cached::Functions::PlayerCameraManager::GetCameraLocation, &params_GetCameraLocation);

	return params_GetCameraLocation.return_value;
}
SDK::FRotator SDK::APlayerCameraManager::GetCameraRotation() {
	if (!this) return SDK::FRotator();

	struct {
		SDK::FRotator        return_value;
	} params_GetCameraRotation{};

	this->ProcessEvent(SDK::Cached::Functions::PlayerCameraManager::GetCameraRotation, &params_GetCameraRotation);

	return params_GetCameraRotation.return_value;
}
float SDK::APlayerCameraManager::GetFOVAngle() {
	if (!this) return 0.f;

	struct {
		float				return_value;
	} params_GetFOVAngle{};

	this->ProcessEvent(SDK::Cached::Functions::PlayerCameraManager::GetFOVAngle, &params_GetFOVAngle);

	return params_GetFOVAngle.return_value;
}

void SDK::APlayerController::ClientSetRotation(FRotator& NewRotation, bool bResetCamera) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return;

	struct {
		FRotator NewRotation;
		bool bResetCamera;
	} params_ClientSetRotation{};

	params_ClientSetRotation.NewRotation = NewRotation;
	params_ClientSetRotation.bResetCamera = bResetCamera;

	this->ProcessEvent(SDK::Cached::Functions::PlayerController::ClientSetRotation, &params_ClientSetRotation);

	return;
}
bool SDK::APlayerController::WasInputKeyJustReleased(FKey& Key) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return false;

	struct {
		FKey Key;
		bool return_value;
		uint8 Pad_9D1[0x7];
	} params_WasInputKeyJustReleased{};

	params_WasInputKeyJustReleased.Key = Key;

	this->ProcessEvent(SDK::Cached::Functions::PlayerController::WasInputKeyJustReleased, &params_WasInputKeyJustReleased);

	return params_WasInputKeyJustReleased.return_value;
}
bool SDK::APlayerController::WasInputKeyJustPressed(FKey& Key) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return false;

	struct {
		FKey Key;
		bool return_value;
		uint8 Pad_9D1[0x7];
	} params_WasInputKeyJustPressed{};

	params_WasInputKeyJustPressed.Key = Key;

	this->ProcessEvent(SDK::Cached::Functions::PlayerController::WasInputKeyJustPressed, &params_WasInputKeyJustPressed);

	return params_WasInputKeyJustPressed.return_value;
}
bool SDK::APlayerController::IsInputKeyDown(FKey& Key) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return false;

	struct {
		FKey Key;
		bool return_value;
		uint8 Pad_9D1[0x7];
	} params_IsInputKeyDown{};

	params_IsInputKeyDown.Key = Key;

	this->ProcessEvent(SDK::Cached::Functions::PlayerController::IsInputKeyDown, &params_IsInputKeyDown);

	return params_IsInputKeyDown.return_value;
}
bool SDK::APlayerController::GetMousePosition(float* LocationX, float* LocationY) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return false;

	struct {
		float LocationX;
		float LocationY;
		bool return_value;
		uint8 Pad_9D4[0x3];
	} params_GetMousePosition{};

	this->ProcessEvent(SDK::Cached::Functions::PlayerController::GetMousePosition, &params_GetMousePosition);

	*LocationX = params_GetMousePosition.LocationX;
	*LocationY = params_GetMousePosition.LocationY;

	return params_GetMousePosition.return_value;
}

SDK::UClass* SDK::UEngine::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(skCrypt("Engine").decrypt());

	return Clss;
}
SDK::UEngine* SDK::UEngine::GetDefaultObj() {
	static class UEngine* Default = nullptr;

	if (!Default)
		Default = static_cast<UEngine*>(UEngine::StaticClass()->DefaultObject());

	return Default;
}

SDK::TArray<SDK::AActor*> SDK::UGameplayStatics::GetAllActorsOfClass(UObject* WorldContextObject, UObject* ActorClass) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return TArray<AActor*>{};

	struct {
		UObject* WorldContextObject;
		UObject* ActorClass;
		TArray<AActor*> OutActors;
	} params_GetAllActorsOfClass{};

	params_GetAllActorsOfClass.WorldContextObject = WorldContextObject;
	params_GetAllActorsOfClass.ActorClass = ActorClass;

	this->ProcessEvent(SDK::Cached::Functions::GameplayStatics::GetAllActorsOfClass, &params_GetAllActorsOfClass);

	return params_GetAllActorsOfClass.OutActors;
}
SDK::UGameplayStatics* SDK::UGameplayStatics::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(skCrypt("GameplayStatics").decrypt());

	return reinterpret_cast<UGameplayStatics*>(Clss);
}

SDK::FString SDK::UKismetSystemLibrary::GetEngineVersion() {
	if (!this) return FString{};

	struct {
		FString return_value;
	} params_GetEngineVersion{};

	this->ProcessEvent(SDK::Cached::Functions::KismetSystemLibrary::GetEngineVersion, &params_GetEngineVersion);

	return params_GetEngineVersion.return_value;
}
bool SDK::UKismetSystemLibrary::LineTraceSingle(class UObject* WorldContextObject, const struct FVector& Start, const struct FVector& End, enum class ETraceTypeQuery TraceChannel, bool bTraceComplex, TArray<class AActor*>& ActorsToIgnore, enum class EDrawDebugTrace DrawDebugType, struct FHitResult* OutHit, bool bIgnoreSelf, const struct FLinearColor& TraceColor, const struct FLinearColor& TraceHitColor, float DrawTime) {
	return reinterpret_cast<bool(*)
		(UObject * WorldContextObject,
			FVector Start,
			FVector End,
			ETraceTypeQuery TraceChannel,
			bool bTraceComplex,
			TArray<class AActor*> ActorsToIgnore,
			EDrawDebugTrace DrawDebugType,
			FHitResult OutHit, bool bIgnoreSelf,
			FLinearColor TraceColor,
			FLinearColor TraceHitColor,
			float DrawTime
			)>(SDK::LineTraceSingle)(
				WorldContextObject,
				Start,
				End,
				TraceChannel,
				true,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				{},
				true,
				{},
				{},
				0.f);
}
SDK::UClass* SDK::UKismetSystemLibrary::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(skCrypt("KismetSystemLibrary").decrypt());

	return Clss;
}

SDK::UFont* SDK::Roboto::StaticFont() {
	static class UFont* Font = nullptr;

	if (!Font)
		Font = reinterpret_cast<SDK::UFont*>(UObject::FindObject(skCrypt("Font Roboto.Roboto").decrypt()));

	return Font;
}

void SDK::UCanvas::K2_DrawLine(const FVector2D& ScreenPositionA, const FVector2D& ScreenPositionB, float Thickness, const FLinearColor& RenderColor) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return;

	struct
	{
		FVector2D			ScreenPositionA;
		FVector2D			ScreenPositionB;
		float               Thickness;
		FLinearColor        RenderColor;
	} params_K2_DrawLine{};

	params_K2_DrawLine.ScreenPositionA = ScreenPositionA;
	params_K2_DrawLine.ScreenPositionB = ScreenPositionB;
	params_K2_DrawLine.Thickness = Thickness;
	params_K2_DrawLine.RenderColor = RenderColor;

	this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_DrawLine, &params_K2_DrawLine);
}
SDK::FVector2D SDK::UCanvas::K2_Project(FVector& WorldLocation) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return FVector2D{};

	struct {
		FVector WorldLocation;

		FVector return_value;
	} params_K2_Project{};

	params_K2_Project.WorldLocation = WorldLocation;

	this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_Project, &params_K2_Project);

	if (params_K2_Project.return_value.Z > 0) {
		return FVector2D(params_K2_Project.return_value.X, params_K2_Project.return_value.Y);
	}

	return FVector2D(0, 0);
}
void SDK::UCanvas::K2_DrawText(FString& RenderText, FVector2D ScreenPosition, int32 FontSize, FLinearColor RenderColor, bool bCentreX, bool bCentreY, bool bOutlined) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return;

	int32 OriginalFontSize = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->GetFontSize();
	reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->SetFontSize(FontSize);

	if (SDK::GetGameVersion() < 7.00) {
		struct {
			UFont* RenderFont;
			FString RenderText;
			FVector2D ScreenPosition;
			FLinearColor RenderColor;
			float Kerning;
			FLinearColor ShadowColor;
			FVector2D ShadowOffset;
			bool bCentreX;
			bool bCentreY;
			bool bOutlined;
			FLinearColor OutlineColor;
		} params_K2_DrawText_1{};

		params_K2_DrawText_1.RenderFont = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont());
		params_K2_DrawText_1.RenderText = RenderText;
		params_K2_DrawText_1.ScreenPosition = { ScreenPosition.X, ScreenPosition.Y };
		params_K2_DrawText_1.RenderColor = RenderColor;
		params_K2_DrawText_1.Kerning = false;
		params_K2_DrawText_1.ShadowColor = { 0.f, 0.f, 0.f, 0.f };
		params_K2_DrawText_1.ShadowOffset = { ScreenPosition.X + 1.5f, ScreenPosition.Y + 1.5f };
		params_K2_DrawText_1.bCentreX = bCentreX;
		params_K2_DrawText_1.bCentreY = bCentreY;
		params_K2_DrawText_1.bOutlined = bOutlined;
		params_K2_DrawText_1.OutlineColor = { 0.f, 0.f, 0.f, 1.f };

		this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_DrawText, &params_K2_DrawText_1);
	}
	else {
		struct {
			UFont* RenderFont;
			FString RenderText;
			FVector2D ScreenPosition;
			FVector2D Scale;
			FLinearColor RenderColor;
			float Kerning;
			FLinearColor ShadowColor;
			FVector2D ShadowOffset;
			bool bCentreX;
			bool bCentreY;
			bool bOutlined;
			FLinearColor OutlineColor;
		} params_K2_DrawText_2{};

		params_K2_DrawText_2.RenderFont = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont());
		params_K2_DrawText_2.RenderText = RenderText;
		params_K2_DrawText_2.ScreenPosition = { ScreenPosition.X, ScreenPosition.Y };
		params_K2_DrawText_2.Scale = { 1.f, 1.f };
		params_K2_DrawText_2.RenderColor = RenderColor;
		params_K2_DrawText_2.Kerning = false;
		params_K2_DrawText_2.ShadowColor = { 0.f, 0.f, 0.f, 0.f };
		params_K2_DrawText_2.ShadowOffset = { ScreenPosition.X + 1.5f, ScreenPosition.Y + 1.5f };
		params_K2_DrawText_2.bCentreX = bCentreX;
		params_K2_DrawText_2.bCentreY = bCentreY;
		params_K2_DrawText_2.bOutlined = bOutlined;
		params_K2_DrawText_2.OutlineColor = { 0.f, 0.f, 0.f, 1.f };

		this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_DrawText, &params_K2_DrawText_2);
	}

	reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->SetFontSize(OriginalFontSize);

	return;
}
SDK::FVector2D SDK::UCanvas::K2_TextSize(const FString& RenderText, int32 FontSize) {
	if (!SDK::IsValidPointer((uintptr_t)this)) return FVector2D{};

	int32 OriginalFontSize = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->GetFontSize();
	reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->SetFontSize(FontSize);

	struct {
		UFont* RenderFont;
		FString RenderText;
		FVector2D Scale;

		FVector2D return_value;
	} params_K2_TextSize{};

	params_K2_TextSize.RenderFont = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont());
	params_K2_TextSize.RenderText = RenderText;
	params_K2_TextSize.Scale = SDK::FVector2D(1.f, 1.f);

	this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_TextSize, &params_K2_TextSize);

	reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->SetFontSize(OriginalFontSize);

	return params_K2_TextSize.return_value;
}



// Wrapper Functions

SDK::FVector SDK::USkeletalMeshComponent::GetBonePosition(uint8_t BoneID) {
	return GetSocketLocation(Features::FortPawnHelper::Bone::GetBoneName(BoneID));
}

SDK::FVector2D SDK::Project(FVector& WorldLocation) {
	return reinterpret_cast<SDK::UCanvas*>(Game::CurrentCanvas)->K2_Project(WorldLocation);
}
bool SDK::IsPositionVisible(SDK::UObject* WorldContextObj, FVector CameraPosition, FVector TargetPosition, SDK::AActor* ActorToIgnore) {
	FHitResult Hit{};
	TArray<AActor*> IgnoredActors;

	IgnoredActors.Add(ActorToIgnore);

	Hit.TraceStart = CameraPosition;
	Hit.TraceEnd = TargetPosition;

	bool bHitSomething = SDK::UKismetSystemLibrary::LineTraceSingle(
		WorldContextObj,
		CameraPosition,
		TargetPosition,
		ETraceTypeQuery::TraceTypeQuery6,
		true,
		IgnoredActors,
		EDrawDebugTrace::None,
		&Hit,
		false,
		FLinearColor(0.f, 0.f, 0.f, 0.f),
		FLinearColor(0.f, 0.f, 0.f, 0.f),
		0.f
	);

	return !(bHitSomething);
}
float SDK::GetGameVersion() {
	if (!Game::GameVersion) {
		FString EngineVersion = reinterpret_cast<UKismetSystemLibrary*>(UKismetSystemLibrary::StaticClass())->GetEngineVersion();
		std::string EngineVersionStr = EngineVersion.ToString();

		size_t LastHyphenPos = EngineVersionStr.rfind('-');
		std::string VersionSubstring = EngineVersionStr.substr(LastHyphenPos + 1);
		Game::GameVersion = std::stof(VersionSubstring);

		if (!Game::GameVersion) {
			THROW_ERROR(skCrypt("Failed to determine game version!").decrypt(), true);
		}
	}

	return Game::GameVersion;
}
SDK::ULocalPlayer* SDK::GetLocalPlayer() {
	return SDK::GetEngine()->GameViewport()->GameInstance()->LocalPlayers()[0];
}
SDK::APlayerController* SDK::GetLocalController() {
	return SDK::GetLocalPlayer()->PlayerController();
}
SDK::UCanvas* SDK::GetLocalCanvas() {
	return reinterpret_cast<UCanvas*>(Game::CurrentCanvas);
}



// Engine Functions

SDK::UEngine* SDK::GetEngine()
{
	static UEngine* GEngine = nullptr;

	if (!GEngine)
	{
		for (int i = 0; i < UObject::ObjectArray.Num(); i++)
		{
			UObject* Obj = UObject::ObjectArray.GetByIndex(i);

			if (!Obj)
				continue;

			if (Obj->IsA(UEngine::StaticClass()) && !Obj->IsDefaultObject())
			{
				GEngine = static_cast<UEngine*>(Obj);
				break;
			}
		}
	}

	return GEngine;
}
SDK::UWorld* SDK::GetWorld()
{
	if (UEngine* Engine = GetEngine())
	{
		if (!Engine->GameViewport())
			return nullptr;

		return Engine->GameViewport()->World();
	}

	return nullptr;
}