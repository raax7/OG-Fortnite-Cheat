#include "Engine_classes.h"

#include <numbers>

#include "CoreUObject_classes.h"

#include "../../../Utilities/Error.h"

#include "../../Features/FortPawnHelper/Bone.h"
#include "../../Features/Visuals/Chams.h"

#include "../../Game.h"
#include "../../../Configs/Config.h"



// Classes

void SDK::USceneComponent::SetPhysicsLinearVelocity(FVector NewVel, bool bAddToCurrent, FName BoneName) {
	if (SDK::IsValidPointer(this) == false) return;

	struct {
		FVector NewVel;
		bool bAddToCurrent;
		FName BoneName;
	} params_SetPhysicsLinearVelocity{};

	params_SetPhysicsLinearVelocity.NewVel = NewVel;
	params_SetPhysicsLinearVelocity.bAddToCurrent = bAddToCurrent;
	params_SetPhysicsLinearVelocity.BoneName = BoneName;

	this->ProcessEvent(SDK::Cached::Functions::SceneComponent::SetPhysicsLinearVelocity, &params_SetPhysicsLinearVelocity);

	return;
}

SDK::UMaterialInstanceDynamic* SDK::UPrimitiveComponent::CreateDynamicMaterialInstance(int32 ElementIndex, class UMaterialInterface* SourceMaterial, class FName OptionalName) {
	if (SDK::IsValidPointer(this) == false) return nullptr;

	struct {
		int32 ElementIndex;
		uint8 Pad_277[0x4];
		UMaterialInterface* SourceMaterial;
		FName OptionalName;

		UMaterialInstanceDynamic* return_value;
	} params_CreateDynamicMaterialInstance{};

	params_CreateDynamicMaterialInstance.ElementIndex = ElementIndex;
	params_CreateDynamicMaterialInstance.SourceMaterial = SourceMaterial;
	params_CreateDynamicMaterialInstance.OptionalName = OptionalName;

	this->ProcessEvent(SDK::Cached::Functions::SceneComponent::CreateDynamicMaterialInstance, &params_CreateDynamicMaterialInstance);

	return params_CreateDynamicMaterialInstance.return_value;
}

void SDK::UPrimitiveComponent::SetMaterial(int32 ElementIndex, UMaterialInterface* Material) {
	if (SDK::IsValidPointer(this) == false) return;

	struct {
		int32 ElementIndex;
		uint8 Pad_677[0x4];
		UMaterialInterface* Material;
	} params_SetMaterial{};

	params_SetMaterial.ElementIndex = ElementIndex;
	params_SetMaterial.Material = Material;

	this->ProcessEvent(SDK::Cached::Functions::PrimitiveComponent::SetMaterial, &params_SetMaterial);

	return;
}

void SDK::UMovementComponent::StopMovementImmediately() {
	if (SDK::IsValidPointer(this) == false) return;

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
	if (SDK::IsValidPointer(this) == false) return false;

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
	if (SDK::IsValidPointer(this) == false) return false;

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

void SDK::AActor::SetActorEnableCollision(bool bNewActorEnableCollision) {
	if (SDK::IsValidPointer(this) == false) return;

	struct {
		bool bNewActorEnableCollision;
	} params_SetActorEnableCollision{};

	params_SetActorEnableCollision.bNewActorEnableCollision = bNewActorEnableCollision;

	this->ProcessEvent(SDK::Cached::Functions::Actor::SetActorEnableCollision, &params_SetActorEnableCollision);

	return;
}

SDK::TArray<SDK::UMaterialInterface*> SDK::UMeshComponent::GetMaterials() {
	if (SDK::IsValidPointer(this) == false) return TArray<UMaterialInterface*>{};

	struct {
		TArray<UMaterialInterface*> return_value;
	} params_GetMaterials{};

	this->ProcessEvent(SDK::Cached::Functions::MeshComponent::GetMaterials, &params_GetMaterials);

	return params_GetMaterials.return_value;
}

SDK::UClass* SDK::UMeshComponent::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("MeshComponent")));

	return Clss;
}

SDK::FName SDK::USkeletalMeshComponent::GetBoneName(int32 BoneIndex) {
	if (SDK::IsValidPointer(this) == false) return FName{};

	struct {
		int32 BoneIndex;

		FName return_value;
	} params_GetBoneName{};

	params_GetBoneName.BoneIndex = BoneIndex;

	this->ProcessEvent(SDK::Cached::Functions::SkinnedMeshComponent::GetBoneName, &params_GetBoneName);

	return params_GetBoneName.return_value;
}
SDK::FVector SDK::USkeletalMeshComponent::GetSocketLocation(FName InSocketName) {
	if (SDK::IsValidPointer(this) == false) return FVector();

	struct {
		FName InSocketName;

		FVector return_value;
	} params_GetSocketLocation{};

	params_GetSocketLocation.InSocketName = InSocketName;

	this->ProcessEvent(SDK::Cached::Functions::SkinnedMeshComponent::GetSocketLocation, &params_GetSocketLocation);

	return params_GetSocketLocation.return_value;
}

SDK::UPawnMovementComponent* SDK::APawn::GetMovementComponent() {
	if (SDK::IsValidPointer(this) == false) return nullptr;

	struct {
		UPawnMovementComponent* return_value;
	} params_GetMovementComponent{};

	this->ProcessEvent(SDK::Cached::Functions::Pawn::GetMovementComponent, &params_GetMovementComponent);

	return params_GetMovementComponent.return_value;
}

SDK::UClass* SDK::USkeletalMeshComponentBudgeted::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("SkeletalMeshComponentBudgeted")));

	return Clss;
}

SDK::FString SDK::APlayerState::GetPlayerName() {
	if (SDK::IsValidPointer(this) == false) return FString{};

	struct {
		FString return_value;
	} params_GetPlayerName{};

	this->ProcessEvent(SDK::Cached::Functions::PlayerState::GetPlayerName, &params_GetPlayerName);

	return params_GetPlayerName.return_value;
}

SDK::FVector SDK::APlayerCameraManager::GetCameraLocation() {
	if (SDK::IsValidPointer(this) == false) return SDK::FVector();

	struct {
		SDK::FVector        return_value;
	} params_GetCameraLocation{};

	this->ProcessEvent(SDK::Cached::Functions::PlayerCameraManager::GetCameraLocation, &params_GetCameraLocation);

	return params_GetCameraLocation.return_value;
}
SDK::FRotator SDK::APlayerCameraManager::GetCameraRotation() {
	if (SDK::IsValidPointer(this) == false) return SDK::FRotator();

	struct {
		SDK::FRotator        return_value;
	} params_GetCameraRotation{};

	this->ProcessEvent(SDK::Cached::Functions::PlayerCameraManager::GetCameraRotation, &params_GetCameraRotation);

	return params_GetCameraRotation.return_value;
}
float SDK::APlayerCameraManager::GetFOVAngle() {
	if (SDK::IsValidPointer(this) == false) return 0.f;

	struct {
		float				return_value;
	} params_GetFOVAngle{};

	this->ProcessEvent(SDK::Cached::Functions::PlayerCameraManager::GetFOVAngle, &params_GetFOVAngle);

	return params_GetFOVAngle.return_value;
}

void SDK::APlayerController::ClientSetRotation(FRotator& NewRotation, bool bResetCamera) {
	if (SDK::IsValidPointer(this) == false) return;

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
	if (SDK::IsValidPointer(this) == false) return;

	struct {
		FRotator NewRotation;
	} params_SetControlRotation{};

	params_SetControlRotation.NewRotation = NewRotation;

	this->ProcessEvent(SDK::Cached::Functions::PlayerController::SetControlRotation, &params_SetControlRotation);

	return;
}
void SDK::APlayerController::AddYawInput(float Val) {
	if (SDK::IsValidPointer(this) == false) return;

	struct {
		float Val;
	} params_AddYawInput{};

	params_AddYawInput.Val = Val;

	this->ProcessEvent(SDK::Cached::Functions::PlayerController::AddYawInput, &params_AddYawInput);
}
void SDK::APlayerController::AddPitchInput(float Val) {
	if (SDK::IsValidPointer(this) == false) return;

	struct {
		float Val;
	} params_AddPitchInput{};

	params_AddPitchInput.Val = Val;

	this->ProcessEvent(SDK::Cached::Functions::PlayerController::AddPitchInput, &params_AddPitchInput);
}
bool SDK::APlayerController::WasInputKeyJustReleased(FKey& Key) {
	if (SDK::IsValidPointer(this) == false) return false;

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
	if (SDK::IsValidPointer(this) == false) return false;

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
	if (SDK::IsValidPointer(this) == false) return false;

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
	if (SDK::IsValidPointer(this) == false) return false;

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
		Clss = UObject::FindClassFast(std::string(skCrypt("PlayerController")));

	return Clss;
}

SDK::UClass* SDK::UGameViewportClient::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("GameViewportClient")));

	return Clss;
}
SDK::UClass* SDK::UEngine::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("Engine")));

	return Clss;
}
SDK::UEngine* SDK::UEngine::GetDefaultObj() {
	static class UEngine* Default = nullptr;

	if (!Default)
		Default = static_cast<UEngine*>(UEngine::StaticClass()->DefaultObject());

	return Default;
}

SDK::TArray<SDK::AActor*> SDK::UGameplayStatics::GetAllActorsOfClass(UObject* WorldContextObject, UObject* ActorClass) {
	struct {
		UObject* WorldContextObject;
		UObject* ActorClass;
		TArray<AActor*> OutActors;
	} params_GetAllActorsOfClass{};

	params_GetAllActorsOfClass.WorldContextObject = WorldContextObject;
	params_GetAllActorsOfClass.ActorClass = ActorClass;

	StaticClass()->ProcessEvent(SDK::Cached::Functions::GameplayStatics::GetAllActorsOfClass, &params_GetAllActorsOfClass);

	return params_GetAllActorsOfClass.OutActors;
}
SDK::UClass* SDK::UGameplayStatics::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("GameplayStatics")));

	return Clss;
}

SDK::FString SDK::UKismetSystemLibrary::GetEngineVersion() {
	struct {
		FString return_value;
	} params_GetEngineVersion{};

	StaticClass()->ProcessEvent(SDK::Cached::Functions::KismetSystemLibrary::GetEngineVersion, &params_GetEngineVersion);

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
		FHitResult& OutHit,
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

	return OriginalLineTraceSingle(WorldContextObject, StartCopy, EndCopy, TraceChannel, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::None, OutHit, bIgnoreSelf, EmptyColor, EmptyColor, 0.f);
}
SDK::UClass* SDK::UKismetSystemLibrary::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("KismetSystemLibrary")));

	return Clss;
}

SDK::UMaterialInstanceDynamic* SDK::UKismetMaterialLibrary::CreateDynamicMaterialInstance(class UObject* WorldContextObject, class UMaterialInterface* Parent, class FName OptionalName) {
	if (SDK::GetGameVersion() >= 12.00) {
		struct {
			class UObject* WorldContextObject;
			class UMaterialInterface* Parent;
			enum class EMIDCreationFlags CreationFlags;
			uint8 Pad_186A[0x7];
			class FName OptionalName;

			class UMaterialInstanceDynamic* return_value;
		} params_CreateDynamicMaterialInstance{};

		params_CreateDynamicMaterialInstance.WorldContextObject = WorldContextObject;
		params_CreateDynamicMaterialInstance.Parent = Parent;
		params_CreateDynamicMaterialInstance.CreationFlags = EMIDCreationFlags::None;
		params_CreateDynamicMaterialInstance.OptionalName = OptionalName;

		StaticClass()->ProcessEvent(SDK::Cached::Functions::KismetMaterialLibrary::CreateDynamicMaterialInstance, &params_CreateDynamicMaterialInstance);

		return params_CreateDynamicMaterialInstance.return_value;
	}
	else {
		struct {
			class UObject* WorldContextObject;
			class UMaterialInterface* Parent;
			class FName OptionalName;

			class UMaterialInstanceDynamic* return_value;
		} params_CreateDynamicMaterialInstance{};

		params_CreateDynamicMaterialInstance.WorldContextObject = WorldContextObject;
		params_CreateDynamicMaterialInstance.Parent = Parent;
		params_CreateDynamicMaterialInstance.OptionalName = OptionalName;

		StaticClass()->ProcessEvent(SDK::Cached::Functions::KismetMaterialLibrary::CreateDynamicMaterialInstance, &params_CreateDynamicMaterialInstance);

		return params_CreateDynamicMaterialInstance.return_value;
	}
}
SDK::UClass* SDK::UKismetMaterialLibrary::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("KismetMaterialLibrary")));

	return Clss;
}

SDK::FVector SDK::UKismetMathLibrary::GetForwardVector(const FRotator& InRot) {
	struct {
		FRotator InRot;

		FVector return_value;
	} params_GetForwardVector{};

	params_GetForwardVector.InRot = InRot;

	StaticClass()->ProcessEvent(SDK::Cached::Functions::KismetMathLibrary::GetForwardVector, &params_GetForwardVector);

	return params_GetForwardVector.return_value;
}
SDK::FVector SDK::UKismetMathLibrary::GetRightVector(const FRotator& InRot) {
	struct {
		FRotator InRot;

		FVector return_value;
	} params_GetRightVector{};

	params_GetRightVector.InRot = InRot;

	StaticClass()->ProcessEvent(SDK::Cached::Functions::KismetMathLibrary::GetRightVector, &params_GetRightVector);

	return params_GetRightVector.return_value;
}
SDK::FRotator SDK::UKismetMathLibrary::FindLookAtRotation(struct FVector Start, struct FVector Target) {
	struct {
		FVector Start;
		FVector Target;

		FRotator return_value;
	} params_FindLookAtRotation{};

	params_FindLookAtRotation.Start = Start;
	params_FindLookAtRotation.Target = Target;

	StaticClass()->ProcessEvent(SDK::Cached::Functions::KismetMathLibrary::FindLookAtRotation, &params_FindLookAtRotation);

	return params_FindLookAtRotation.return_value;
}
int32 SDK::UKismetMathLibrary::FMod(float Dividend, float Divisor, float* Remainder) {
	struct {
		float Dividend;
		float Divisor;
		float Remainder;
		int32 return_value;
	} params_FMod{};

	params_FMod.Dividend = Dividend;
	params_FMod.Divisor = Divisor;
	params_FMod.Remainder = *Remainder;

	StaticClass()->ProcessEvent(SDK::Cached::Functions::KismetMathLibrary::FMod, &params_FMod);

	*Remainder = params_FMod.Remainder;

	return params_FMod.return_value;

}
SDK::UClass* SDK::UKismetMathLibrary::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("KismetMathLibrary")));

	return Clss;
}

SDK::UFont* SDK::Roboto::StaticFont() {
	static class UFont* Font = nullptr;

	if (!Font)
		Font = reinterpret_cast<SDK::UFont*>(UObject::FindObject(std::string(skCrypt("Font Roboto.Roboto"))));

	return Font;
}

void SDK::UCanvas::K2_DrawLine(const FVector2D& ScreenPositionA, const FVector2D& ScreenPositionB, float Thickness, const FLinearColor& RenderColor) {
	if (SDK::IsValidPointer(this) == false) return;

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
	if (SDK::IsValidPointer(this) == false) return FVector();

	struct {
		FVector WorldLocation;

		FVector return_value;
	} params_K2_Project{};

	params_K2_Project.WorldLocation = WorldLocation;

	this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_Project, &params_K2_Project);

	return params_K2_Project.return_value;
}
void SDK::UCanvas::K2_DrawText(FString& RenderText, FVector2D ScreenPosition, int32 FontSize, FLinearColor RenderColor, bool bCentreX, bool bCentreY, bool bOutlined) {
	if (SDK::IsValidPointer(this) == false) return;

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
	if (SDK::IsValidPointer(this) == false) return FVector2D{};

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
void SDK::UCanvas::K2_DrawBox(FVector2D ScreenPosition, FVector2D ScreenSize, float Thickness, const FLinearColor& RenderColor) {
	if (SDK::IsValidPointer(this) == false) return;

	struct {
		FVector2D ScreenPosition;
		FVector2D ScreenSize;
		float Thickness;
		FLinearColor RenderColor;
	} params_K2_DrawBox{};

	params_K2_DrawBox.ScreenPosition = ScreenPosition;
	params_K2_DrawBox.ScreenSize = ScreenSize;
	params_K2_DrawBox.Thickness = Thickness;
	params_K2_DrawBox.RenderColor = RenderColor;

	this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_DrawBox, &params_K2_DrawBox);
}

SDK::UMaterial* SDK::UMaterialInterface::GetBaseMaterial() {
	if (SDK::IsValidPointer(this) == false) return nullptr;

	struct {
		UMaterial* return_value;
	} params_GetBaseMaterial{};

	this->ProcessEvent(SDK::Cached::Functions::MaterialInterface::GetBaseMaterial, &params_GetBaseMaterial);

	return params_GetBaseMaterial.return_value;
}

SDK::UClass* SDK::UMaterialInterface::StaticClass() {
	static class UClass* Clss = nullptr;

	if (!Clss)
		Clss = UObject::FindClassFast(std::string(skCrypt("MaterialInterface")));

	return Clss;
}

void SDK::UMaterialInstanceDynamic::SetVectorParameterValue(FName ParameterName, FLinearColor Value) {
	if (SDK::IsValidPointer(this) == false) return;

	struct {
		FName ParameterName;
		FLinearColor Value;
	} params_SetVectorParameterValue{};

	params_SetVectorParameterValue.ParameterName = ParameterName;
	params_SetVectorParameterValue.Value = Value;

	this->ProcessEvent(SDK::Cached::Functions::MaterialInstanceDynamic::SetVectorParameterValue, &params_SetVectorParameterValue);
}

void SDK::UMaterialInstanceDynamic::SetScalarParameterValue(FName ParameterName, float Value) {
	if (SDK::IsValidPointer(this) == false) return;

	struct {
		FName ParameterName;
		float Value;
	} params_SetScalarParameterValue{};

	params_SetScalarParameterValue.ParameterName = ParameterName;
	params_SetScalarParameterValue.Value = Value;

	this->ProcessEvent(SDK::Cached::Functions::MaterialInstanceDynamic::SetScalarParameterValue, &params_SetScalarParameterValue);
}



// Wrapper Functions

SDK::FVector SDK::USkeletalMeshComponent::GetBonePosition(uint8_t BoneID) {
	return GetSocketLocation(Features::FortPawnHelper::Bone::GetBoneName(BoneID));
}

#if CUSTOM_W2S
SDK::FMatrix GetViewMatrix(SDK::FRotator Rotation) {
	const __m128 RotationRadians = _mm_mul_ps(_mm_set_ps(0, Rotation.Roll, Rotation.Yaw, Rotation.Pitch), _mm_set_ps1(std::numbers::pi_v<float> / 180.f));

	__m128 CosRotations;
	const __m128 SinRotations = _mm_sincos_ps(&CosRotations, RotationRadians);

	const float SinPitch = _mm_cvtss_f32(SinRotations);
	const float CosPitch = _mm_cvtss_f32(CosRotations);
	const float SinYaw = _mm_cvtss_f32(_mm_shuffle_ps(SinRotations, SinRotations, _MM_SHUFFLE(1, 1, 1, 1)));
	const float CosYaw = _mm_cvtss_f32(_mm_shuffle_ps(CosRotations, CosRotations, _MM_SHUFFLE(1, 1, 1, 1)));
	const float SinRoll = _mm_cvtss_f32(_mm_shuffle_ps(SinRotations, SinRotations, _MM_SHUFFLE(2, 2, 2, 2)));
	const float CosRoll = _mm_cvtss_f32(_mm_shuffle_ps(CosRotations, CosRotations, _MM_SHUFFLE(2, 2, 2, 2)));

	const float SinRollCosPitch = SinRoll * CosPitch;
	const float CosRollCosPitch = CosRoll * CosPitch;
	const float SinPitchSinYaw = SinPitch * SinYaw;
	const float SinPitchCosYaw = SinPitch * CosYaw;

	SDK::FMatrix ViewMatrix;
	_mm_store_ps(ViewMatrix.M[0], _mm_set_ps(0.f, SinPitch, CosPitch * SinYaw, CosPitch * CosYaw));
	_mm_store_ps(ViewMatrix.M[1], _mm_set_ps(0.f, -SinRollCosPitch, SinRoll * SinPitchSinYaw + CosRoll * CosYaw, SinRoll * SinPitchCosYaw - CosRoll * SinYaw));
	_mm_store_ps(ViewMatrix.M[2], _mm_set_ps(0.f, CosRollCosPitch, CosYaw * SinRoll - CosRoll * SinPitchSinYaw, -(CosRoll * SinPitchCosYaw + SinRoll * SinYaw)));

	return ViewMatrix;
}

bool WorldToScreen(SDK::FVector Location, SDK::FVector CamPos, SDK::FRotator CamRot, float CamFOV, SDK::FVector2D* OutScreenPos) {
	SDK::FMatrix ViewMatrix = GetViewMatrix(CamRot);

	SDK::FVector DeltaPos = Location - CamPos;
	SDK::FVector ProjectedPos = {
		DeltaPos.Dot(ViewMatrix.MVec[1]),
		DeltaPos.Dot(ViewMatrix.MVec[2]),
		DeltaPos.Dot(ViewMatrix.MVec[0])
	};

	bool OnScreen = true;
	if (ProjectedPos.Z < 1.f) {
		OnScreen = false;
		ProjectedPos.Z = 1.f;
	}

	float FOVFactor = (Game::ScreenCenterX) / std::tanf(CamFOV * (std::numbers::pi_v<float> / 360.0f)) / ProjectedPos.Z;
	OutScreenPos->X = (Game::ScreenCenterX) + ProjectedPos.X * FOVFactor;
	OutScreenPos->Y = (Game::ScreenCenterY) + -ProjectedPos.Y * FOVFactor;

	return OnScreen;
}

bool WorldToScreen(SDK::FVector Location, SDK::FVector2D* OutScreenPos) {
	SDK::APlayerCameraManager* CameraManager = SDK::GetLocalController()->PlayerCameraManager();

	SDK::FVector CameraLocation = CameraManager->GetCameraLocation();
	SDK::FRotator CameraRotation = CameraManager->GetCameraRotation();
	float CameraFOV = CameraManager->GetFOVAngle();

	return WorldToScreen(Location, CameraLocation, CameraRotation, CameraFOV, OutScreenPos);
}


#endif

SDK::FVector2D SDK::Project(FVector WorldLocation) {
#if CUSTOM_W2S
	SDK::FVector2D ReturnValue;
	bool OnScreen = WorldToScreen(WorldLocation, &ReturnValue);

	if (OnScreen) {
		return ReturnValue;
	}

	return SDK::FVector2D(-1.f, -1.f);
#else
	SDK::FVector ScreenLocation = SDK::GetLocalCanvas()->K2_Project(WorldLocation);

	if (ScreenLocation.Z > 0.f) {
		return SDK::FVector2D(ScreenLocation.X, ScreenLocation.Y);
	}

	return SDK::FVector2D(-1.f, -1.f);
#endif
}
SDK::FVector SDK::Project3D(FVector WorldLocation) {
#if CUSTOM_W2S
	SDK::FVector2D ReturnValue;
	bool OnScreen = WorldToScreen(WorldLocation, &ReturnValue);

	if (!OnScreen) {
		ReturnValue.X *= -1.f;
		ReturnValue.Y *= -1.f;

		ReturnValue.X += Game::ScreenWidth;
		ReturnValue.Y += Game::ScreenHeight;
	}

	return SDK::FVector(ReturnValue.X, ReturnValue.Y, 0.f);
#else
	SDK::FVector ReturnValue = SDK::GetLocalCanvas()->K2_Project(WorldLocation);

	// Invert X and Y if the player is behind the camera
	if (ReturnValue.Z <= 0.f) {
		ReturnValue.X *= -1.f;
		ReturnValue.Y *= -1.f;

		ReturnValue.X += Game::ScreenWidth;
		ReturnValue.Y += Game::ScreenHeight;
	}

	return ReturnValue;
#endif
}
bool SDK::IsPositionVisible(SDK::UObject* WorldContextObj, FVector TargetPosition, SDK::AActor* ActorToIgnore, SDK::AActor* ActorToIgnore2) {
	FHitResult Hit{};
	TArray<AActor*> IgnoredActors;

	if (TargetPosition == SDK::FVector()) return false;

	if (ActorToIgnore) IgnoredActors.Add(ActorToIgnore);
	if (ActorToIgnore2) IgnoredActors.Add(ActorToIgnore2);

	bool bHitSomething = SDK::UKismetSystemLibrary::LineTraceSingle(
		WorldContextObj,
		Actors::MainCamera.Position,
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

	return !bHitSomething;
}
float SDK::GetGameVersion() {
	if (!Game::GameVersion) {
		FString EngineVersion = reinterpret_cast<UKismetSystemLibrary*>(UKismetSystemLibrary::StaticClass())->GetEngineVersion();
		std::string EngineVersionStr = EngineVersion.ToString();

		size_t LastHyphenPos = EngineVersionStr.rfind('-');
		std::string VersionSubstring = EngineVersionStr.substr(LastHyphenPos + 1);
		Game::GameVersion = std::stof(VersionSubstring);

		if (!Game::GameVersion) {
			THROW_ERROR(std::string(skCrypt("Failed to determine game version!")), true);
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