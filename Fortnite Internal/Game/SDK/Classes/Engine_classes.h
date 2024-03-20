#pragma once
#include "../SDK.h"
#include "Engine_Structs.h"

#include "CoreUObject_Classes.h"

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

namespace SDK {
	// Classes

	class USceneComponent : public UObject {
	public:
		// VALUES

		FVector GetPosition() {
			if (SDK::IsValidPointer(this) == false) return FVector{};
			return *(FVector*)((uintptr_t)this + SDK::Cached::Offsets::SceneComponent::RelativeLocation);
		}



		// FUNCTIONS

		void SetPhysicsLinearVelocity(FVector NewVel, bool bAddToCurrent, FName BoneName);
	};
	class UMovementComponent : public UObject {
	public:
		// FUNCTIONS

		void StopMovementImmediately();
	};
	class UPawnMovementComponent : public UMovementComponent {
	public:

	};
	class AActor : public UObject {
	public:
		// VALUES

		USceneComponent* GetRootComponent() {
			if (SDK::IsValidPointer(this) == false) return nullptr;
			return (USceneComponent*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Actor::RootComponent));
		}


		// FUNCTIONS

		bool K2_TeleportTo(const struct FVector& DestLocation, const struct FRotator& DestRotation);

		bool K2_SetActorRotation(const struct FRotator& NewRotation, bool bTeleportPhysics);

		bool K2_SetActorLocation(const struct FVector& NewLocation, bool bSweep, struct FHitResult* SweepHitResult, bool bTeleport);

		void SetActorEnableCollision(bool bNewActorEnableCollision);
	};
	class USkeletalMeshComponent : public UObject {
	public:
		// FUNCTIONS

		FName GetBoneName(int32 BoneIndex);

		FVector GetSocketLocation(FName InSocketName);



		// WRAPPER FUNCTIONS

		FVector GetBonePosition(uint8_t BoneID);
	};
	class APawn : public AActor {
	public:
		UPawnMovementComponent* GetMovementComponent();
	};
	class ACharacter : public APawn {
	public:
		// VALUES

		USkeletalMeshComponent* Mesh() {
			if (SDK::IsValidPointer(this) == false) return nullptr;
			return (USkeletalMeshComponent*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Character::Mesh));
		}
	};
	class APlayerState : public AActor {
	public:
		// FUNCTIONS

		FString GetPlayerName();
	};
	class APlayerCameraManager : public UObject {
	public:
		// FUNCTIONS

		SDK::FVector GetCameraLocation();

		SDK::FRotator GetCameraRotation();

		float GetFOVAngle();
	};
	class APlayerController : public AActor {
	public:
		// VALUES

		APawn* AcknowledgedPawn() {
			if (SDK::IsValidPointer(this) == false) return nullptr;
			return (APawn*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::PlayerController::AcknowledgedPawn));
		}

		APlayerCameraManager* PlayerCameraManager() {
			if (SDK::IsValidPointer(this) == false) return nullptr;
			return (APlayerCameraManager*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::PlayerController::PlayerCameraManager));
		}



		// FUNCTIONS

		void ClientSetRotation(FRotator& NewRotation, bool bResetCamera);

		void SetControlRotation(FRotator NewRotation);

		bool WasInputKeyJustReleased(FKey& Key);

		bool WasInputKeyJustPressed(FKey& Key);

		bool IsInputKeyDown(FKey& Key);

		bool GetMousePosition(float* LocationX, float* LocationY);



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class UPlayer : public UObject {
	public:
		// VALUES

		APlayerController* PlayerController() {
			if (SDK::IsValidPointer(this) == false) return nullptr;
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
			if (SDK::IsValidPointer(this) == false) return TArray<ULocalPlayer*>{};
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
			if (SDK::IsValidPointer(this) == false) return nullptr;
			return (UWorld*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::GameViewportClient::World));
		}

		UGameInstance* GameInstance() {
			if (SDK::IsValidPointer(this) == false) return nullptr;
			return (UGameInstance*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::GameViewportClient::GameInstance));
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class UEngine : public UObject {
	public:
		// VALUES

		UGameViewportClient* GameViewport() {
			if (SDK::IsValidPointer(this) == false) return nullptr;
			return (UGameViewportClient*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Engine::GameViewport));
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();

		static UEngine* GetDefaultObj();
	};
	class UGameplayStatics : public UObject {
	public:
		// FUNCTIONS

		TArray<AActor*> GetAllActorsOfClass(UObject* WorldContextObject, UObject* ActorClass);



		// STATIC FUNCTIONS

		static UGameplayStatics* StaticClass();
	};
	class UKismetSystemLibrary : public UObject
	{
	public:
		// FUNCTIONS

		FString GetEngineVersion();

		static bool LineTraceSingle(
			class UObject* WorldContextObject,
			const struct FVector& Start,
			const struct FVector& End,
			enum class ETraceTypeQuery TraceChannel,
			bool bTraceComplex,
			TArray<class AActor*>& ActorsToIgnore,
			enum class EDrawDebugTrace DrawDebugType,
			struct FHitResult& OutHit,
			bool bIgnoreSelf,
			const struct FLinearColor& TraceColor,
			const struct FLinearColor& TraceHitColor,
			float DrawTime);



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class UKismetMathLibrary : public UObject {
	public:
		// FUNCTIONS

		FVector GetForwardVector(const FRotator& InRot);

		SDK::FVector GetRightVector(const FRotator& InRot);

		FRotator FindLookAtRotation(struct FVector Start, struct FVector Target);
	


		// STATIC FUNCTIONS

		static UKismetMathLibrary* StaticClass();
	};
	class UFont : public UObject {
	public:
		// VALUES

		void SetFontSize(int32 NewFontSize) {
			if (SDK::IsValidPointer(this) == false) return;
			*(int32*)((uintptr_t)this + SDK::Cached::Offsets::Font::LegacyFontSize) = NewFontSize;
		}

		int32 GetFontSize() {
			if (SDK::IsValidPointer(this) == false) return 0;
			return *(int32*)((uintptr_t)this + SDK::Cached::Offsets::Font::LegacyFontSize);
		}
	};
	class Roboto : public UFont {
	public:
		// STATIC FUNCTIONS

		static UFont* StaticFont();
	};
	class UCanvas : public UObject {
	public:
		// VALUES

		int32 SizeX() {
			if (SDK::IsValidPointer(this) == false) return 0;
			return *(int32*)((uintptr_t)this + SDK::Cached::Offsets::Canvas::SizeX);;
		}

		int32 SizeY() {
			if (SDK::IsValidPointer(this) == false) return 0;
			return *(int32*)((uintptr_t)this + SDK::Cached::Offsets::Canvas::SizeY);
		}



		// FUNCTIONS

		void K2_DrawLine(const FVector2D& ScreenPositionA, const FVector2D& ScreenPositionB, float Thickness, const FLinearColor& RenderColor);

		FVector K2_Project(FVector& WorldLocation);

		void K2_DrawText(FString& RenderText, FVector2D ScreenPosition, int32 FontSize, FLinearColor RenderColor, bool bCentreX, bool bCentreY, bool bOutlined);

		FVector2D K2_TextSize(const FString& RenderText, int32 FontSize);

		void K2_DrawBox(FVector2D ScreenPosition, FVector2D ScreenSize, float Thickness, const FLinearColor& RenderColor);
	};



	// Wrapper Functions

	/*
	* @brief Wrapper for K2_Project
	*
	* @param WorldLocation - The world location to project
	*
	* @return The projected location as FVector2D (without Z)
	*/
	FVector2D Project(FVector WorldLocation);
	/*
	* @brief Wrapper for K2_Project
	*
	* @param WorldLocation - The world location to project
	*
	* @return The projected location as FVector (with Z)
	*/
	FVector Project3D(FVector WorldLocation);
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
	bool IsPositionVisible(SDK::UObject* WorldContextObj, FVector CameraPosition, FVector TargetPosition, SDK::AActor* ActorToIgnore = nullptr, SDK::AActor* ActorToIgnore2 = nullptr);
	/*
	* @brief Get the game version
	*
	* @return The game version as a float (e.g. 7.40)
	*/
	float GetGameVersion();
	/*
	* @brief Find the current local player instance
	*
	* @return The current local player instance
	*/
	ULocalPlayer* GetLocalPlayer();
	/*
	* @brief Find the current APlayerController instance
	*
	* @return The current APlayerController instance
	*/
	APlayerController* GetLocalController();
	/*
	* @brief Find the local player pawn
	* 
	* @return The local player pawn
	*/
	APawn* GetLocalPawn();
	/*
	* @brief Find the current Canvas instance
	*
	* @return The current Canvas instance
	*/
	UCanvas* GetLocalCanvas();



	// Engine Functions

	/*
	* @brief Find the current engine instance
	*
	* @return The current engine instance
	*/
	UEngine* GetEngine();
	/*
	* @brief Find the current loaded world
	*
	* @return The current loaded world
	*/
	UWorld* GetWorld();
}