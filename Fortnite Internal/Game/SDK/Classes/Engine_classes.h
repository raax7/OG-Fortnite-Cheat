#pragma once
#include "../SDK.h"
#include "../../../Configs/Config.h"
#include "Basic.h"
#include "Engine_structs.h"
#include "../../../Utilities/Math.h"

namespace SDK {
	// Forward declarations

	class USceneComponent;
	class AActor;
	class USkeletalMeshComponent;
	class APawn;
	class ACharacter;
	class APlayerState;
	class APlayerCameraManager;
	class APlayerController;
	class UPlayer;
	class ULocalPlayer;
	class UGameInstance;
	class UWorld;
	class UGameViewportClient;
	class UEngine;
	class UGameplayStatics;
	class UKismetSystemLibrary;
	class UFont;
	class Roboto;
	class UCanvas;

	static FVector2D Project(FVector& WorldLocation);
	static bool IsPositionVisible(SDK::UObject* WorldContextObj, FVector CameraPosition, FVector TargetPosition, SDK::AActor* ActorToIgnore);
	static float GetGameVersion();
	static class ULocalPlayer* GetLocalPlayer();
	static class APlayerController* GetLocalController();
	static class UCanvas* GetLocalCanvas();
	static class UEngine* GetEngine();
	static class UWorld* GetWorld();



	// Classes

	class USceneComponent : public UObject {
	public:
		// VALUES

		FVector GetPosition() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return FVector{};
			return *(FVector*)((uintptr_t)this + SDK::Cached::Offsets::SceneComponent::RelativeLocation);
		}
	};

	class AActor : public UObject {
	public:
		// VALUES

		USceneComponent* GetRootComponent() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (USceneComponent*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Actor::RootComponent));
		}
	};

	class USkeletalMeshComponent : public UObject
	{
	public:
		// FUNCTIONS

		FVector GetBonePosition(int BoneID) {
			static uintptr_t Function_GetBoneMatrix;
			if (!Function_GetBoneMatrix) {
				Function_GetBoneMatrix = (uintptr_t)(SDK::GetBaseAddress() + SDK::GetBoneMatrix);

				return SDK::FVector();
			}

			static auto GetBoneMatrix = reinterpret_cast<FMatrix * (*)(USkeletalMeshComponent*, FMatrix*, INT)>(Function_GetBoneMatrix);
			if (!GetBoneMatrix) return { 0, 0, 0 };

			FMatrix Matrix;
			GetBoneMatrix(this, &Matrix, BoneID);

			return FVector(Matrix.M[3][0], Matrix.M[3][1], Matrix.M[3][2]);
		}
	};

	class APawn : public AActor {
	public:

	};

	class ACharacter : public APawn {
	public:
		// VALUES

		USkeletalMeshComponent* Mesh() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (USkeletalMeshComponent*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Character::Mesh));
		}
	};

	class APlayerState : public AActor {
	public:
		// FUNCTIONS

		FString GetPlayerName() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return FString{};

			struct {
				FString return_value;
			} params{};

			this->ProcessEvent(SDK::Cached::Functions::PlayerState::GetPlayerName, &params);

			return params.return_value;
		}
	};

	class APlayerCameraManager : public UObject {
	public:
		// FUNCTIONS

		SDK::FVector GetCameraLocation() {
			if (!this) return SDK::FVector();

			struct {
				SDK::FVector        return_value;
			} params;

			this->ProcessEvent(SDK::Cached::Functions::PlayerCameraManager::GetCameraLocation, &params);

			return params.return_value;
		}

		SDK::FRotator GetCameraRotation() {
			if (!this) return SDK::FRotator();

			struct {
				SDK::FRotator        return_value;
			} params;

			this->ProcessEvent(SDK::Cached::Functions::PlayerCameraManager::GetCameraRotation, &params);

			return params.return_value;
		}

		float GetFOVAngle() {
			if (!this) return 0.f;

			struct {
				float				return_value;
			} params;

			this->ProcessEvent(SDK::Cached::Functions::PlayerCameraManager::GetFOVAngle, &params);

			return params.return_value;
		}
	};

	class APlayerController : public AActor {
	public:
		// VALUES

		APawn* AcknowledgedPawn() {
			if (!SDK::IsValidPointer((uintptr_t)this)) return nullptr;
			return (APawn*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::PlayerController::AcknowledgedPawn));
		}

		APlayerCameraManager* PlayerCameraManager() {
			if (!this) return nullptr;
			return (APlayerCameraManager*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::PlayerController::PlayerCameraManager));
		}



		// FUNCTIONS

		void ClientSetRotation(FRotator& NewRotation, bool bResetCamera) {
			if (!SDK::IsValidPointer((uintptr_t)this)) return;

			struct {
				FRotator NewRotation;
				bool bResetCamera;
			} params{};

			params.NewRotation = NewRotation;
			params.bResetCamera = bResetCamera;

			this->ProcessEvent(SDK::Cached::Functions::PlayerController::ClientSetRotation, &params);

			return;
		}
	};

	class UPlayer : public UObject {
	public:
		// VALUES

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
		// VALUES

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
		// VALUES

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
		// VALUES

		UGameViewportClient* GameViewport() {
			if (!this) return nullptr;
			return (UGameViewportClient*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Engine::GameViewport));
		}
		


		// STATIC FUNCTIONS

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
		// FUNCTIONS

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



		// STATIC FUNCTIONS

		static UGameplayStatics* StaticClass()
		{
			static class UClass* Clss = nullptr;

			if (!Clss)
				Clss = UObject::FindClassFast(skCrypt("GameplayStatics").decrypt());

			return reinterpret_cast<UGameplayStatics*>(Clss);
		}
	};

	class UKismetSystemLibrary : public UObject
	{
	public:
		// FUNCTIONS

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
					float DrawTime)>(SDK::LineTraceSingle)
				(WorldContextObject,
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



		// STATIC FUNCTIONS

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
		// VALUES

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
		// STATIC FUNCTIONS

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
		// VALUES

		int32 SizeX() {
			if (!this) return 0;
			Game::ScreenWidth = *(int32*)((uintptr_t)this + SDK::Cached::Offsets::Canvas::SizeX);
			return Game::ScreenWidth;
		}

		int32 SizeY() {
			if (!this) return 0;
			Game::ScreenHeight = *(int32*)((uintptr_t)this + SDK::Cached::Offsets::Canvas::SizeY);
			return Game::ScreenHeight;
		}



		// FUNCTIONS

		void K2_DrawLine(
			const FVector2D& ScreenPositionA,
			const FVector2D& ScreenPositionB,
			float Thickness,
			const FLinearColor& RenderColor) {
			if (!SDK::IsValidPointer((uintptr_t)this)) return;

			struct
			{
				FVector2D			ScreenPositionA;
				FVector2D			ScreenPositionB;
				float               Thickness;
				FLinearColor        RenderColor;
			} params3;

			params3.ScreenPositionA = ScreenPositionA;
			params3.ScreenPositionB = ScreenPositionB;
			params3.Thickness = Thickness;
			params3.RenderColor = RenderColor;

			this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_DrawLine, &params3);
		}

		FVector2D K2_Project(FVector& WorldLocation) {
			if (!SDK::IsValidPointer((uintptr_t)this)) return FVector2D{};

			struct {
				FVector WorldLocation;

				FVector return_value;
			} params4{};

			params4.WorldLocation = WorldLocation;

			this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_Project, &params4);

			if (params4.return_value.Z > 0) {
				return FVector2D(params4.return_value.X, params4.return_value.Y);
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
				} params5{};

				params5.RenderFont = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont());
				params5.RenderText = RenderText;
				params5.ScreenPosition = { ScreenPosition.X, ScreenPosition.Y };
				params5.RenderColor = RenderColor;
				params5.Kerning = false;
				params5.ShadowColor = { 0.f, 0.f, 0.f, 0.f };
				params5.ShadowOffset = { ScreenPosition.X + 1.5f, ScreenPosition.Y + 1.5f };
				params5.bCentreX = bCentreX;
				params5.bCentreY = bCentreY;
				params5.bOutlined = bOutlined;
				params5.OutlineColor = { 0.f, 0.f, 0.f, 1.f };

				this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_DrawText, &params5);
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
				} params6{};

				params6.RenderFont = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont());
				params6.RenderText = RenderText;
				params6.ScreenPosition = { ScreenPosition.X, ScreenPosition.Y };
				params6.Scale = { 1.f, 1.f };
				params6.RenderColor = RenderColor;
				params6.Kerning = false;
				params6.ShadowColor = { 0.f, 0.f, 0.f, 0.f };
				params6.ShadowOffset = { ScreenPosition.X + 1.5f, ScreenPosition.Y + 1.5f };
				params6.bCentreX = bCentreX;
				params6.bCentreY = bCentreY;
				params6.bOutlined = bOutlined;
				params6.OutlineColor = { 0.f, 0.f, 0.f, 1.f };

				this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_DrawText, &params6);
			}

			reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->SetFontSize(OriginalFontSize);

			return;
		}

		FVector2D K2_TextSize(const FString& RenderText, int32 FontSize) {
			if (!SDK::IsValidPointer((uintptr_t)this)) return FVector2D{};

			FVector2D return_value{};

			int32 OriginalFontSize = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->GetFontSize();
			reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->SetFontSize(FontSize);

			if (SDK::GetGameVersion() < 7.00) {
				struct {
					UFont* RenderFont;
					FString RenderText;

					FVector2D return_value;
				} params{};

				params.RenderFont = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont());
				params.RenderText = RenderText;

				this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_TextSize, &params);

				return_value = params.return_value;
			}
			else {
				struct {
					UFont* RenderFont;
					FString RenderText;
					FVector2D Scale;

					FVector2D return_value;
				} params{};

				params.RenderFont = reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont());
				params.RenderText = RenderText;
				params.Scale = { 1.f, 1.f };

				this->ProcessEvent(SDK::Cached::Functions::Canvas::K2_TextSize, &params);

				return_value = params.return_value;
			}

			reinterpret_cast<SDK::Roboto*>(Roboto::StaticFont())->SetFontSize(OriginalFontSize);

			return return_value;
		}
	};



	

	// Wrapper Functions

	/*
	* @brief Find the current local player instance
	*/
	static class ULocalPlayer* GetLocalPlayer() {
		return SDK::GetEngine()->GameViewport()->GameInstance()->LocalPlayers()[0];
	}

	/*
	* @brief Find the current canvas instance
	*/
	static class APlayerController* GetLocalController() {
		return GetLocalPlayer()->PlayerController();
	}

	/*
	* @brief Find the current canvas instance
	*
	* @return The current canvas instance
	*/
	static class UCanvas* GetLocalCanvas() {
		return reinterpret_cast<UCanvas*>(Game::CurrentCanvas);
	}

	/*
	* @brief Wrapper for K2_Project
	* 
	* @param WorldLocation - The world location to project
	* 
	* @return The projected location
	*/
	static FVector2D Project(FVector& WorldLocation) {
		return reinterpret_cast<SDK::UCanvas*>(Game::CurrentCanvas)->K2_Project(WorldLocation);
	}

	/*
	* @brief Wrapper for LineTraceSingle
	* 
	* @param WorldContextObject - The world context object
	* @param Start - The start of the line trace
	* @param End - The end of the line trace
	* @param TraceChannel - The trace channel
	* @param ActorToIgnore - The actor to ignore (optional)
	* 
	* @return Whether the line trace hit something
	*/
	static bool IsPositionVisible(SDK::UObject* WorldContextObj, FVector CameraPosition, FVector TargetPosition, SDK::AActor* ActorToIgnore) {
		FHitResult Hit{};
		TArray<AActor*> IgnoredActors;

		IgnoredActors.Add(ActorToIgnore);

		Hit.TraceStart = CameraPosition;
		Hit.TraceEnd = TargetPosition;

		bool bHitSomething = SDK::UKismetSystemLibrary::LineTraceSingle(
			WorldContextObj,
			CameraPosition,
			TargetPosition,
			static_cast<ETraceTypeQuery>(Config::test),
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



	// Functions

	/*
	* @brief Get the game version
	* 
	* @return The game version as a float (e.g. 7.40)
	*/
	static float SDK::GetGameVersion() {
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

	/*
	* @brief Find the current engine instance
	*
	* @return The current engine instance
	*/
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

	/*
	* @brief Find the current loaded world
	* 
	* @return The current loaded world
	*/
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