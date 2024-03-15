#include "Engine_classes.h"

#include "CoreUObject_classes.h"

#include "../../../Utilities/Error.h"
#include "../../Features/FortPawnHelper/Bone.h"
#include "../../Game.h"



// Classes

void SDK::UMovementComponent::StopMovementImmediately() {
	if (!SDK::IsValidPointer(this)) return;

	struct {
	} params_StopMovementImmediately{};

	this->ProcessEvent(SDK::Cached::Functions::MovementComponent::StopMovementImmediately, &params_StopMovementImmediately);

	return;
}

bool SDK::AActor::K2_TeleportTo(const struct FVector& DestLocation, const struct FRotator& DestRotation) {
	if (!SDK::IsValidPointer(this)) return false;

	struct {
		FVector DestLocation;
		FRotator DestRotation;
		bool return_value;
		uint8 Pad_56F[0x3];
	} params_K2_TeleportTo{};

	params_K2_TeleportTo.DestLocation = DestLocation;
	params_K2_TeleportTo.DestRotation = DestRotation;

	this->ProcessEvent(SDK::Cached::Functions::Actor::K2_TeleportTo, &params_K2_TeleportTo);

	return params_K2_TeleportTo.return_value;
}

bool SDK::AActor::K2_SetActorRotation(const struct FRotator& NewRotation, bool bTeleportPhysics) {
	if (!SDK::IsValidPointer(this)) return false;

	struct {
		FRotator NewRotation;
		bool bTeleportPhysics;
		bool return_value;
		uint8 Pad_577[0x2];
	} params_K2_SetActorRotation{};

	params_K2_SetActorRotation.NewRotation = NewRotation;
	params_K2_SetActorRotation.bTeleportPhysics = bTeleportPhysics;

	this->ProcessEvent(SDK::Cached::Functions::Actor::K2_SetActorRotation, &params_K2_SetActorRotation);

	return params_K2_SetActorRotation.return_value;
}

bool SDK::AActor::K2_SetActorLocation(const struct FVector& NewLocation, bool bSweep, struct FHitResult* SweepHitResult, bool bTeleport) {
	if (!SDK::IsValidPointer(this)) return false;

	struct {
		FVector NewLocation;
		bool bSweep;
		uint8 Pad_585[0x3];
		FHitResult* SweepHitResult;
		bool bTeleport;
		bool return_value;
		uint8 Pad_586[0x2];
	} params_K2_SetActorLocation{};

	params_K2_SetActorLocation.NewLocation = NewLocation;
	params_K2_SetActorLocation.bSweep = bSweep;
	params_K2_SetActorLocation.SweepHitResult = SweepHitResult;
	params_K2_SetActorLocation.bTeleport = bTeleport;

	this->ProcessEvent(SDK::Cached::Functions::Actor::K2_SetActorLocation, &params_K2_SetActorLocation);

	return params_K2_SetActorLocation.return_value;
}

SDK::FName SDK::USkeletalMeshComponent::GetBoneName(int32 BoneIndex) {
	if (!SDK::IsValidPointer(this)) return FName{};

	struct {
		int32 BoneIndex;

		FName return_value;
	} params_GetBoneName{};

	params_GetBoneName.BoneIndex = BoneIndex;

	this->ProcessEvent(SDK::Cached::Functions::SkinnedMeshComponent::GetBoneName, &params_GetBoneName);

	return params_GetBoneName.return_value;
}
SDK::FVector SDK::USkeletalMeshComponent::GetSocketLocation(FName InSocketName) {
	if (!SDK::IsValidPointer(this)) return FVector();

	struct {
		FName InSocketName;

		FVector return_value;
	} params_GetSocketLocation{};

	params_GetSocketLocation.InSocketName = InSocketName;

	this->ProcessEvent(SDK::Cached::Functions::SkinnedMeshComponent::GetSocketLocation, &params_GetSocketLocation);

	return params_GetSocketLocation.return_value;
}

SDK::UPawnMovementComponent* SDK::APawn::GetMovementComponent() {
	if (!SDK::IsValidPointer(this)) return nullptr;

	struct {
		UPawnMovementComponent* return_value;
	} params_GetMovementComponent{};

	this->ProcessEvent(SDK::Cached::Functions::Pawn::GetMovementComponent, &params_GetMovementComponent);

	return params_GetMovementComponent.return_value;
}

SDK::FString SDK::APlayerState::GetPlayerName() {
	if (!SDK::IsValidPointer(this)) return FString{};

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
	if (!SDK::IsValidPointer(this)) return;

	struct {
		FRotator NewRotation;
		bool bResetCamera;
	} params_ClientSetRotation{};

	params_ClientSetRotation.NewRotation = NewRotation;
	params_ClientSetRotation.bResetCamera = bResetCamera;

	this->ProcessEvent(SDK::Cached::Functions::PlayerController::ClientSetRotation, &params_ClientSetRotation);

	return;
}
void SDK::APlayerController::SetControlRotation(FRotator NewRotation) {
	if (!SDK::IsValidPointer(this)) return;

	struct {
		FRotator NewRotation;
	} params_SetControlRotation{};

	params_SetControlRotation.NewRotation = NewRotation;

	this->ProcessEvent(SDK::Cached::Functions::PlayerController::SetControlRotation, &params_SetControlRotation);

	return;
}
bool SDK::APlayerController::WasInputKeyJustReleased(FKey& Key) {
	if (!SDK::IsValidPointer(this)) return false;

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
	if (!SDK::IsValidPointer(this)) return false;

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
	if (!SDK::IsValidPointer(this)) return false;

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
	if (!SDK::IsValidPointer(this)) return false;

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
SDK::UClass* SDK::APlayerController::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(skCrypt("PlayerController").decrypt());

	return Clss;
}

SDK::UClass* SDK::UGameViewportClient::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(skCrypt("GameViewportClient").decrypt());

	return Clss;
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
	if (!SDK::IsValidPointer(this)) return TArray<AActor*>{};

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
bool SDK::UKismetSystemLibrary::LineTraceSingle(class UObject* WorldContextObject, const struct FVector& Start, const struct FVector& End, enum class ETraceTypeQuery TraceChannel, bool bTraceComplex, TArray<class AActor*>& ActorsToIgnore, enum class EDrawDebugTrace DrawDebugType, struct FHitResult& OutHit, bool bIgnoreSelf, const struct FLinearColor& TraceColor, const struct FLinearColor& TraceHitColor, float DrawTime) {
	using LineTraceSingleParams = bool(*)(
		UObject* WorldContextObject,
		FVector& Start,
		FVector& End,
		ETraceTypeQuery TraceChannel,
		bool bTraceComplex,
		TArray<class AActor*>& ActorsToIgnore,
		EDrawDebugTrace DrawDebugType,
		FHitResult OutHit,
		bool bIgnoreSelf,
		FLinearColor& TraceColor,
		FLinearColor& TraceHitColor,
		float DrawTime);
	static LineTraceSingleParams OriginalLineTraceSingle;

	if (OriginalLineTraceSingle == nullptr) OriginalLineTraceSingle = reinterpret_cast<LineTraceSingleParams>(SDK::GetBaseAddress() + SDK::Cached::Functions::LineTraceSingle);

	// Calling through spoof_call doesn't work here. Most likely because some parameters are passed by reference

	SDK::FVector StartCopy = Start;
	SDK::FVector EndCopy = End;

	SDK::FLinearColor EmptyColor = SDK::FLinearColor();

	return OriginalLineTraceSingle(WorldContextObject, StartCopy, EndCopy, TraceChannel, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::None, {}, bIgnoreSelf, EmptyColor, EmptyColor, 0.f);
}
SDK::UClass* SDK::UKismetSystemLibrary::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(skCrypt("KismetSystemLibrary").decrypt());

	return Clss;
}

SDK::FVector SDK::UKismetMathLibrary::GetForwardVector(const FRotator& InRot) {
	if (!SDK::IsValidPointer(this)) return FVector{};

	struct {
		FRotator InRot;

		FVector return_value;
	} params_GetForwardVector{};

	params_GetForwardVector.InRot = InRot;

	this->ProcessEvent(SDK::Cached::Functions::KismetMathLibrary::GetForwardVector, &params_GetForwardVector);

	return params_GetForwardVector.return_value;
}
SDK::FVector SDK::UKismetMathLibrary::GetRightVector(const FRotator& InRot) {
	if (!SDK::IsValidPointer(this)) return FVector{};

	struct {
		FRotator InRot;

		FVector return_value;
	} params_GetRightVector{};

	params_GetRightVector.InRot = InRot;

	this->ProcessEvent(SDK::Cached::Functions::KismetMathLibrary::GetRightVector, &params_GetRightVector);

	return params_GetRightVector.return_value;
}
SDK::FRotator SDK::UKismetMathLibrary::FindLookAtRotation(struct FVector Start, struct FVector Target) {
	if (!SDK::IsValidPointer(this)) return FRotator{};

	struct {
		FVector Start;
		FVector Target;

		FRotator return_value;
	} params_FindLookAtRotation{};

	params_FindLookAtRotation.Start = Start;
	params_FindLookAtRotation.Target = Target;

	this->ProcessEvent(SDK::Cached::Functions::KismetMathLibrary::FindLookAtRotation, &params_FindLookAtRotation);

	return params_FindLookAtRotation.return_value;
}
SDK::UKismetMathLibrary* SDK::UKismetMathLibrary::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(skCrypt("KismetMathLibrary").decrypt());

	return reinterpret_cast<UKismetMathLibrary*>(Clss);
}

SDK::UFont* SDK::Roboto::StaticFont() {
	static class UFont* Font = nullptr;

	if (!Font)
		Font = reinterpret_cast<SDK::UFont*>(UObject::FindObject(skCrypt("Font Roboto.Roboto").decrypt()));

	return Font;
}

void SDK::UCanvas::K2_DrawLine(const FVector2D& ScreenPositionA, const FVector2D& ScreenPositionB, float Thickness, const FLinearColor& RenderColor) {
	if (!SDK::IsValidPointer(this)) return;

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
SDK::FVector SDK::UCanvas::K2_Project(FVector& WorldLocation) {
	if (!SDK::IsValidPointer(this)) return FVector();

	struct {
		FVector WorldLocation;

		FVector return_value;
	} params_K2_Project{};

	params_K2_Project.WorldLocation = WorldLocation;

	this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_Project, &params_K2_Project);

	return params_K2_Project.return_value;
}
void SDK::UCanvas::K2_DrawText(FString& RenderText, FVector2D ScreenPosition, int32 FontSize, FLinearColor RenderColor, bool bCentreX, bool bCentreY, bool bOutlined) {
	if (!SDK::IsValidPointer(this)) return;

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
	if (!SDK::IsValidPointer(this)) return FVector2D{};

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

SDK::FVector2D SDK::Project(FVector WorldLocation) {
//#if _IMGUI
//	return ProjectWorldToScreen(WorldLocation, Actors::MainCamera.Position, Actors::MainCamera.Rotation, Actors::MainCamera.FOV);
//#else
	SDK::FVector ScreenLocation = SDK::GetLocalCanvas()->K2_Project(WorldLocation);

	if (ScreenLocation.Z > 0.f) {
		return SDK::FVector2D(ScreenLocation.X, ScreenLocation.Y);
	}

	return SDK::FVector2D(-1.f, -1.f);
//#endif
}
SDK::FVector SDK::Project3D(FVector WorldLocation) {
	// IMPROVE THIS SO IT WORKS THE SAME ON ENGINE AND IMGUI
///#if _IMGUI
//	SDK::FVector2D ScreenLocation = ProjectWorldToScreen(WorldLocation, Actors::MainCamera.Position, Actors::MainCamera.Rotation, Actors::MainCamera.FOV);
//	return SDK::FVector(ScreenLocation.X, ScreenLocation.Y, 1.f);
//#else
	return SDK::GetLocalCanvas()->K2_Project(WorldLocation);
//#endif
}
bool SDK::IsPositionVisible(SDK::UObject* WorldContextObj, FVector CameraPosition, FVector TargetPosition, SDK::AActor* ActorToIgnore, SDK::AActor* ActorToIgnore2) {
	FHitResult Hit{};
	TArray<AActor*> IgnoredActors;

	if (TargetPosition == SDK::FVector()) return false;

	if (ActorToIgnore) IgnoredActors.Add(ActorToIgnore);
	if (ActorToIgnore2) IgnoredActors.Add(ActorToIgnore2);

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
		Hit,
		true,
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
SDK::APawn* SDK::GetLocalPawn() {
	return GetLocalController()->AcknowledgedPawn();
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