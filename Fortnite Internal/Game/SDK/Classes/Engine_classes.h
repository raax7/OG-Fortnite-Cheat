#ifndef ENGINE_CLASSES_H
#define ENGINE_CLASSES_H

#include "../SDK.h"
#include "../../../Configs/Config.h"
#include "Basic.h"
#include "Engine_structs.h"

namespace SDK {
	class USceneComponent : public UObject {
	public:
		FVector GetPosition() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return FVector{};
			return *(FVector*)((uintptr_t)this + SDK::Cached::Offsets::SceneComponent::RelativeLocation);
		}
	};

	class AActor : public UObject {
	public:
		USceneComponent* GetRootComponent() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (USceneComponent*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Actor::RootComponent));
		}
	};




	class USkinnedMeshComponent : public UObject
	{
	public:

	};

	class USkeletalMeshComponent : public USkinnedMeshComponent
	{
	public:
		FVector GetBonePosition(int BoneID) {
			static uintptr_t Function_GetBoneMatrix;
			if (!Function_GetBoneMatrix) {
				Function_GetBoneMatrix = (uintptr_t)(SDK::GetBaseAddress() + SDK::GetBoneMatrix);
				if (!Function_GetBoneMatrix)
					return { 0, 0, 0 };
			}

			static auto GetBoneMatrix = reinterpret_cast<FMatrix * (*)(USkinnedMeshComponent*, FMatrix*, INT)>(Function_GetBoneMatrix);
			if (!GetBoneMatrix) return { 0, 0, 0 };

			FMatrix matrix;
			GetBoneMatrix(this, &matrix, BoneID);

			return FVector(matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]);
		}
	};

	class APawn : public AActor {
	public:

	};

	class ACharacter : public APawn {
	public:
		USkeletalMeshComponent* Mesh() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (USkeletalMeshComponent*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Character::Mesh));
		}
	};

	class APlayerCameraManager : public UObject {
	public:
		SDK::FVector GetCameraLocation() {
			if (!this) return SDK::FVector{};

			struct {
				SDK::FVector        return_value;
			} params;

			this->ProcessEvent(SDK::Cached::Functions::PlayerCameraManager::GetCameraLocation, &params);

			return params.return_value;
		}
	};

	class APlayerController : public UObject {
	public:
		APawn* AcknowledgedPawn() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (APawn*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::PlayerController::AcknowledgedPawn));
		}

		APlayerCameraManager* PlayerCameraManager() {
			if (!this) return nullptr;
			return (APlayerCameraManager*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::PlayerController::PlayerCameraManager));
		}
	};

	class UPlayer : public UObject {
	public:
		APlayerController* PlayerController() {
			if (!this) return nullptr;
			return (APlayerController*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Player::PlayerController));
		}
	};

	class ULocalPlayer : public UPlayer {
	public:

	};

	class UGameInstance : public AActor {
	public:
		TArray<ULocalPlayer*> LocalPlayers() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return TArray<ULocalPlayer*>{};
			return *(TArray<ULocalPlayer*>*)((uintptr_t)this + SDK::Cached::Offsets::GameInstance::LocalPlayers);
		}
	};

	class UWorld : public UObject {
	public:

	};

	class UGameViewportClient : public UObject {
	public:
		UWorld* World() {
			if (!this) return nullptr;
			return (UWorld*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::GameViewportClient::World));
		}

		UGameInstance* GameInstance() {
			if (!this) return nullptr;
			return (UGameInstance*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::GameViewportClient::GameInstance));
		}
	};

	class UEngine : public UObject {
	public:
		UGameViewportClient* GameViewport() {
			if (!this) return nullptr;
			return (UGameViewportClient*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Engine::GameViewport));
		}
		


		static UClass* StaticClass()
		{
			static class UClass* Clss = nullptr;

			if (!Clss)
				Clss = UObject::FindClassFast(skCrypt("Engine").decrypt());

			return Clss;
		}
		static UEngine* GetDefaultObj()
		{
			static class UEngine* Default = nullptr;

			if (!Default)
				Default = static_cast<UEngine*>(UEngine::StaticClass()->DefaultObject());

			return Default;
		}
	};

	class UGameplayStatics : public UObject
	{
	public:
		TArray<AActor*> GetAllActorsOfClass(UObject* WorldContextObject, UObject* ActorClass) {
			if (!SDK::IsValidPointer((uintptr_t)this)) return TArray<AActor*>{};

			struct {
				UObject* WorldContextObject;
				UObject* ActorClass;
				TArray<AActor*> OutActors;
			} params{};

			params.WorldContextObject = WorldContextObject;
			params.ActorClass = ActorClass;

			this->ProcessEvent(SDK::Cached::Functions::GameplayStatics::GetAllActorsOfClass, &params);

			return params.OutActors;
		}



		static UGameplayStatics* StaticClass()
		{
			static class UClass* Clss = nullptr;

			if (!Clss)
				Clss = UObject::FindClassFast(skCrypt("GameplayStatics").decrypt());

			return reinterpret_cast<UGameplayStatics*>(Clss);
		}
	};

	static float GetGameVersion();

	class UKismetSystemLibrary : public UObject
	{
	public:
		FString GetEngineVersion() {
			if (!this) return FString{};

			struct {
				FString return_value;
			} params{};

			this->ProcessEvent(SDK::Cached::Functions::KismetSystemLibrary::GetEngineVersion, &params);

			return params.return_value;
		}

		static bool LineTraceSingle(
			class UObject* WorldContextObject,
			const struct FVector& Start,
			const struct FVector& End,
			enum class ETraceTypeQuery TraceChannel,
			bool bTraceComplex,
			TArray<class AActor*>& ActorsToIgnore,
			enum class EDrawDebugTrace DrawDebugType,
			struct FHitResult* OutHit,
			bool bIgnoreSelf,
			const struct FLinearColor& TraceColor,
			const struct FLinearColor& TraceHitColor,
			float DrawTime) {
			if (SDK::GetGameVersion() < 7.00) {
				struct Params {
					class UObject* WorldContextObject;
					struct FVector                               Start;
					struct FVector                               End;
					enum class ETraceTypeQuery                   TraceChannel;
					bool                                         bTraceComplex;
					uint8                                        Pad_1977[0x6];
					TArray<class AActor*>                        ActorsToIgnore;
					enum class EDrawDebugTrace                   DrawDebugType;
					uint8                                        Pad_1978[0x7];
					struct FHitResult                            OutHit;
					bool                                         bIgnoreSelf;
					uint8                                        Pad_1979[0x3];
					struct FLinearColor                          TraceColor;
					struct FLinearColor                          TraceHitColor;
					float                                        DrawTime;
					bool                                         ReturnValue;
					uint8                                        Pad_197A[0x7];
				} params;

				params.WorldContextObject = WorldContextObject;
				params.Start = Start;
				params.End = End;
				params.TraceChannel = TraceChannel;
				params.bTraceComplex = bTraceComplex;
				params.ActorsToIgnore = ActorsToIgnore;
				params.DrawDebugType = DrawDebugType;
				params.bIgnoreSelf = bIgnoreSelf;
				params.TraceColor = TraceColor;
				params.TraceHitColor = TraceHitColor;
				params.DrawTime = DrawTime;

				StaticClass()->ProcessEvent(SDK::Cached::Functions::KismetSystemLibrary::LineTraceSingle, &params);

				if (OutHit != nullptr)
					*OutHit = std::move(params.OutHit);

				return params.ReturnValue;
			}
			else {
				struct Params {
					class UObject* WorldContextObject;
					struct FVector                               Start;
					struct FVector                               End;
					enum class ETraceTypeQuery                   TraceChannel;
					bool                                         bTraceComplex;
					uint8                                        Pad_1977[0x6];
					TArray<class AActor*>                        ActorsToIgnore;
					enum class EDrawDebugTrace                   DrawDebugType;
					uint8                                        Pad_1978[0x3];
					struct FHitResult                            OutHit;
					bool                                         bIgnoreSelf;
					uint8                                        Pad_1979[0x3];
					struct FLinearColor                          TraceColor;
					struct FLinearColor                          TraceHitColor;
					float                                        DrawTime;
					bool                                         ReturnValue;
					uint8                                        Pad_197A[0x3];
				} params;

				params.WorldContextObject = WorldContextObject;
				params.Start = Start;
				params.End = End;
				params.TraceChannel = TraceChannel;
				params.bTraceComplex = bTraceComplex;
				params.ActorsToIgnore = ActorsToIgnore;
				params.DrawDebugType = DrawDebugType;
				params.bIgnoreSelf = bIgnoreSelf;
				params.TraceColor = TraceColor;
				params.TraceHitColor = TraceHitColor;
				params.DrawTime = DrawTime;

				StaticClass()->ProcessEvent(SDK::Cached::Functions::KismetSystemLibrary::LineTraceSingle, &params);

				if (OutHit != nullptr)
					*OutHit = std::move(params.OutHit);

				return params.ReturnValue;
			}
		}

		static bool IsPositionVisible(SDK::UObject* WorldContextObj, FVector CameraPosition, FVector TargetPosition, SDK::AActor* Pawn) {
			FHitResult Hit{};
			TArray<AActor*> IgnoredActors;

			IgnoredActors.Add(Pawn);

			Hit.TraceStart = CameraPosition;
			Hit.TraceEnd = TargetPosition;

			bool bHitSomething = LineTraceSingle(
				WorldContextObj,
				CameraPosition,
				TargetPosition,
				static_cast<ETraceTypeQuery>(Config::test), //ETraceTypeQuery::TraceTypeQuery6
				true,
				IgnoredActors,
				EDrawDebugTrace::None,
				&Hit,
				false,
				FLinearColor(1.f, 0.f, 0.f, 1.f),
				FLinearColor(1.f, 0.f, 0.f, 1.f),
				10.f
			);

			return !(bHitSomething);
		}



		static UClass* StaticClass()
		{
			static class UClass* Clss = nullptr;

			if (!Clss)
				Clss = UObject::FindClassFast(skCrypt("KismetSystemLibrary").decrypt());

			return Clss;
		}
	};

	class UFont : public UObject {
	public:
		void SetFontSize(int32 NewFontSize) {
			if (!SDK::IsValidPointer((uintptr_t)this)) return;
			*(int32*)((uintptr_t)this + SDK::Cached::Offsets::Font::LegacyFontSize) = NewFontSize;
		}

		int32 GetFontSize() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return 0;
			return *(int32*)((uintptr_t)this + SDK::Cached::Offsets::Font::LegacyFontSize);
		}
	};

	class Roboto : public UFont {
	public:
		static UFont* StaticFont()
		{
			static class UFont* Font = nullptr;

			if (!Font)
				Font = reinterpret_cast<SDK::UFont*>(UObject::FindObject(skCrypt("Font Roboto.Roboto").decrypt()));

			return Font;
		}
	};

	class UCanvas : public UObject {
	public:
		void K2_DrawLine(
			const struct FVector2D& ScreenPositionA,
			const struct FVector2D& ScreenPositionB,
			float Thickness, const struct
			FLinearColor& RenderColor) {
			if (!SDK::IsValidPointer((uintptr_t)this)) return;

			struct
			{
				FVector2D			ScreenPositionA;
				FVector2D			ScreenPositionB;
				float               Thickness;
				FLinearColor        RenderColor;
			} params;

			params.ScreenPositionA = ScreenPositionA;
			params.ScreenPositionB = ScreenPositionB;
			params.Thickness = Thickness;
			params.RenderColor = RenderColor;

			this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_DrawLine, &params);
		}

		FVector2D K2_Project(FVector& WorldLocation) {
			if (!SDK::IsValidPointer((uintptr_t)this)) return FVector2D{};

			struct {
				FVector WorldLocation;

				FVector return_value;
			} params{};

			params.WorldLocation = WorldLocation;

			this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_Project, &params);

			if (params.return_value.Z > 0) {
				return FVector2D(params.return_value.X, params.return_value.Y);
			}

			return FVector2D(0, 0);
		}

		void K2_DrawText(FString& RenderText, FVector2D ScreenPosition, int32 FontSize, FLinearColor RenderColor, bool bCentreX, bool bCentreY, bool bOutlined) {
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
				} params{};

				params.RenderFont = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont());
				params.RenderText = RenderText;
				params.ScreenPosition = { ScreenPosition.X, ScreenPosition.Y };
				params.RenderColor = RenderColor;
				params.Kerning = false;
				params.ShadowColor = { 0.f, 0.f, 0.f, 0.f };
				params.ShadowOffset = { ScreenPosition.X + 1.5f, ScreenPosition.Y + 1.5f };
				params.bCentreX = bCentreX;
				params.bCentreY = bCentreY;
				params.bOutlined = bOutlined;
				params.OutlineColor = { 0.f, 0.f, 0.f, 1.f };

				this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_DrawText, & params);
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
				} params{};

				params.RenderFont = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont());
				params.RenderText = RenderText;
				params.ScreenPosition = { ScreenPosition.X, ScreenPosition.Y };
				params.Scale = { 1.f, 1.f };
				params.RenderColor = RenderColor;
				params.Kerning = false;
				params.ShadowColor = { 0.f, 0.f, 0.f, 0.f };
				params.ShadowOffset = { ScreenPosition.X + 1.5f, ScreenPosition.Y + 1.5f };
				params.bCentreX = bCentreX;
				params.bCentreY = bCentreY;
				params.bOutlined = bOutlined;
				params.OutlineColor = { 0.f, 0.f, 0.f, 1.f };

				this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_DrawText, &params);
			}

			reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->SetFontSize(OriginalFontSize);

			return;
		}



		int32 SizeX() {
			return 1920;
		}
		int32 SizeY() {
			return 1080;
		}
	};


	static class UEngine* GetEngine()
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
	static class APlayerController* GetLocalController() {
		return SDK::GetEngine()->GameViewport()->GameInstance()->LocalPlayers()[0]->PlayerController();
	}
	static class UWorld* GetWorld()
	{
		if (UEngine* Engine = GetEngine())
		{
			if (!Engine->GameViewport())
				return nullptr;

			return Engine->GameViewport()->World();
		}

		return nullptr;
	}
}

static float SDK::GetGameVersion() {
	static float version;

	if (!version) {
		FString EngineVersion = reinterpret_cast<UKismetSystemLibrary*>(UKismetSystemLibrary::StaticClass())->GetEngineVersion();
		std::string EngineVersionStr = EngineVersion.ToString();

		size_t lastHyphenPos = EngineVersionStr.rfind('-');
		std::string versionSubstring = EngineVersionStr.substr(lastHyphenPos + 1);
		version = std::stof(versionSubstring);

		if (!version) {
			THROW_ERROR(skCrypt("Failed to determine game version!").decrypt(), true);
		}
	}

	return version;
}

#endif