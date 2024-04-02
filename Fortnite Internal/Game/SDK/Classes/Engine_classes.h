#pragma once
#include "../SDK.h"
#include "Engine_Structs.h"

#include "CoreUObject_Classes.h"

#include "../../Features/Features.h"

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

	class UMaterialInterface : public UObject {
	public:
		// FUNCTIONS

		class UMaterial* GetBaseMaterial();



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class UMaterial : public UMaterialInterface {
	public:
		// VALUES

		void SetbDisableDepthTest(bool NewValue, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false || (SDK::Cached::Offsets::Material::bDisableDepthTest == -0x1 && SDK::Cached::Masks::Material::bDisableDepthTest == -0x1)) return;

			if (SDK::Cached::Masks::Material::bDisableDepthTest != -0x1) {
				if (AutoRevertFeature) {
					Features::CreateAutoRevertBitFeature(((uint8_t*)(uintptr_t)this + SDK::Cached::Offsets::Material::bDisableDepthTest), SDK::Cached::Masks::Material::bDisableDepthTest, AutoRevertFeature);
				}

				uint8* BitField = (uint8*)((uintptr_t)this + SDK::Cached::Offsets::Material::bDisableDepthTest);
				*BitField = NewValue ? *BitField | SDK::Cached::Masks::Material::bDisableDepthTest : *BitField & ~SDK::Cached::Masks::Material::bDisableDepthTest;
			}
			else {
				if (AutoRevertFeature) {
					Features::CreateAutoRevertFeature<bool>((bool*)((uintptr_t)this + SDK::Cached::Offsets::Material::bDisableDepthTest), AutoRevertFeature);
				}

				*(bool*)((uintptr_t)this + SDK::Cached::Offsets::Material::bDisableDepthTest) = NewValue;
			}
		}

		void SetBlendMode(EBlendMode NewValue, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Material::BlendMode == -0x1) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<bool>((bool*)((uintptr_t)this + SDK::Cached::Offsets::Material::BlendMode), AutoRevertFeature);
			}

			*(EBlendMode*)((uintptr_t)this + SDK::Cached::Offsets::Material::BlendMode) = NewValue;
		}

		void SetWireFrame(bool NewValue, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false || (SDK::Cached::Offsets::Material::Wireframe == -0x1 && SDK::Cached::Masks::Material::Wireframe == -0x1)) return;

			if (SDK::Cached::Masks::Material::Wireframe != -0x1) {
				if (AutoRevertFeature) {
					Features::CreateAutoRevertBitFeature(((uint8_t*)(uintptr_t)this + SDK::Cached::Offsets::Material::Wireframe), SDK::Cached::Masks::Material::Wireframe, AutoRevertFeature);
				}

				uint8* BitField = (uint8*)((uintptr_t)this + SDK::Cached::Offsets::Material::Wireframe);
				*BitField = NewValue ? *BitField | SDK::Cached::Masks::Material::Wireframe : *BitField & ~SDK::Cached::Masks::Material::Wireframe;
			}
			else {
				if (AutoRevertFeature) {
					Features::CreateAutoRevertFeature<bool>((bool*)((uintptr_t)this + SDK::Cached::Offsets::Material::Wireframe), AutoRevertFeature);
				}

				*(bool*)((uintptr_t)this + SDK::Cached::Offsets::Material::Wireframe) = NewValue;
			}
		}
	};
	class UMaterialInstanceDynamic : public UMaterialInterface {
	public:
		// FUNCTIONS

		void SetVectorParameterValue(FName ParameterName, FLinearColor Value);

		void SetScalarParameterValue(FName ParameterName, float Value);
	};
	class UActorComponent : public UObject {
	public:

	};
	class USceneComponent : public UActorComponent {
	public:
		// VALUES

		FVector RelativeLocation() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::SceneComponent::RelativeLocation == -0x1) return FVector{};
			return *(FVector*)((uintptr_t)this + SDK::Cached::Offsets::SceneComponent::RelativeLocation);
		}



		// FUNCTIONS

		void SetPhysicsLinearVelocity(FVector NewVel, bool bAddToCurrent, FName BoneName);
	};
	class UPrimitiveComponent : public USceneComponent {
	public:
		// FUNCTIONS

		UMaterialInstanceDynamic* CreateDynamicMaterialInstance(int32 ElementIndex, class UMaterialInterface* SourceMaterial, class FName OptionalName);

		void SetMaterial(int32 ElementIndex, UMaterialInterface* Material);
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

		USceneComponent* RootComponent() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Actor::RootComponent == -0x1) return nullptr;
			return (USceneComponent*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Actor::RootComponent));
		}

		void SetNetRole(ENetRole NewRole) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Actor::Role == -0x1) return;
			*(ENetRole*)((uintptr_t)this + SDK::Cached::Offsets::Actor::Role) = NewRole;
		}

		TArray<UActorComponent*>* InstanceComponents() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Actor::InstanceComponents == -0x1) return nullptr;
			return (TArray<UActorComponent*>*)((uintptr_t)this + SDK::Cached::Offsets::Actor::InstanceComponents);
		}

		TArray<UActorComponent*>* BlueprintCreatedComponents() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Actor::BlueprintCreatedComponents == -0x1) return nullptr;
			return (TArray<UActorComponent*>*)((uintptr_t)this + SDK::Cached::Offsets::Actor::BlueprintCreatedComponents);
		}


		// FUNCTIONS

		bool K2_TeleportTo(const struct FVector& DestLocation, const struct FRotator& DestRotation);

		bool K2_SetActorRotation(const struct FRotator& NewRotation, bool bTeleportPhysics);

		bool K2_SetActorLocation(const struct FVector& NewLocation, bool bSweep, struct FHitResult* SweepHitResult, bool bTeleport);

		void SetActorEnableCollision(bool bNewActorEnableCollision);
	};
	class UMeshComponent : public UPrimitiveComponent {
	public:
		// FUNCTIONS

		TArray<UMaterialInterface*> GetMaterials();



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class USkeletalMesh : public UObject {
	public:
		// VALUES

		TArray<UMaterialInterface*> Materials() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::SkeletalMesh::Materials == -0x1) return TArray<UMaterialInterface*>{};
			return *(TArray<UMaterialInterface*>*)((uintptr_t)this + SDK::Cached::Offsets::SkeletalMesh::Materials);
		}
	};
	class USkeletalMeshComponent : public UMeshComponent {
	public:
		// VALUES

		USkeletalMesh* SkeletalMesh() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::SkeletalMeshComponent::SkeletalMesh == -0x1) return nullptr;
			return (USkeletalMesh*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::SkeletalMeshComponent::SkeletalMesh));
		}



		// FUNCTIONS

		FName GetBoneName(int32 BoneIndex);

		FVector GetSocketLocation(FName InSocketName);



		// WRAPPER FUNCTIONS

		FVector GetBonePosition(uint8_t BoneID);
	};
	class UStaticMeshComponent : public UMeshComponent {
	public:

	};
	class USkeletalMeshComponentBudgeted : public USkeletalMeshComponent
	{
	public:
		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class APlayerState : public AActor {
	public:
		// FUNCTIONS

		FString GetPlayerName();
	};
	class APawn : public AActor {
	public:
		// VALUES

		APlayerState* PlayerState() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Pawn::PlayerState == -0x1) return nullptr;
			return (APlayerState*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Pawn::PlayerState));
		}



		// FUNCTIONS

		UPawnMovementComponent* GetMovementComponent();
	};
	class ACharacter : public APawn {
	public:
		// VALUES

		USkeletalMeshComponent* Mesh() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Character::Mesh == -0x1) return nullptr;
			return (USkeletalMeshComponent*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Character::Mesh));
		}
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
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::PlayerController::AcknowledgedPawn == -0x1) return nullptr;
			return (APawn*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::PlayerController::AcknowledgedPawn));
		}

		APlayerCameraManager* PlayerCameraManager() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::PlayerController::PlayerCameraManager == -0x1) return nullptr;
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
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Player::PlayerController == -0x1) return nullptr;
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
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::GameInstance::LocalPlayers == -0x1) return TArray<ULocalPlayer*>{};
			return *(TArray<ULocalPlayer*>*)((uintptr_t)this + SDK::Cached::Offsets::GameInstance::LocalPlayers);
		}
	};
	class AGameState : public AActor {
	public:

	};
	class UWorld : public UObject {
	public:
		// VALUES

		AGameState* GameState() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::World::GameState == -0x1) return nullptr;
			return (AGameState*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::World::GameState));
		}
	};
	class UGameViewportClient : public UObject {
	public:
		// VALUES

		UWorld* World() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::GameViewportClient::World == -0x1) return nullptr;
			return (UWorld*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::GameViewportClient::World));
		}

		UGameInstance* GameInstance() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::GameViewportClient::GameInstance == -0x1) return nullptr;
			return (UGameInstance*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::GameViewportClient::GameInstance));
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class UEngine : public UObject {
	public:
		// VALUES

		UGameViewportClient* GameViewport() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Engine::GameViewport == -0x1) return nullptr;
			return (UGameViewportClient*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::Engine::GameViewport));
		}



		// STATIC FUNCTIONS

		static UClass* StaticClass();

		static UEngine* GetDefaultObj();
	};
	class UGameplayStatics : public UObject {
	public:
		// FUNCTIONS

		static TArray<AActor*> GetAllActorsOfClass(UObject* WorldContextObject, UObject* ActorClass);



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class UKismetSystemLibrary : public UObject
	{
	public:
		// FUNCTIONS

		static FString GetEngineVersion();

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
	class UKismetMaterialLibrary : public UObject {
	public:
		// FUNCTIONS

		static UMaterialInstanceDynamic* CreateDynamicMaterialInstance(class UObject* WorldContextObject, class UMaterialInterface* Parent, class FName OptionalName);



		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class UKismetMathLibrary : public UObject {
	public:
		// FUNCTIONS

		static FVector GetForwardVector(const FRotator& InRot);

		static SDK::FVector GetRightVector(const FRotator& InRot);

		static FRotator FindLookAtRotation(struct FVector Start, struct FVector Target);

		static int32 FMod(float Dividend, float Divisor, float* Remainder);
	


		// STATIC FUNCTIONS

		static UClass* StaticClass();
	};
	class UFont : public UObject {
	public:
		// VALUES

		void SetFontSize(int32 NewFontSize) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Font::LegacyFontSize == -0x1) return;
			*(int32*)((uintptr_t)this + SDK::Cached::Offsets::Font::LegacyFontSize) = NewFontSize;
		}

		int32 GetFontSize() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Font::LegacyFontSize == -0x1) return 0;
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
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Canvas::SizeX == -0x1) return 0;
			return *(int32*)((uintptr_t)this + SDK::Cached::Offsets::Canvas::SizeX);;
		}

		int32 SizeY() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::Canvas::SizeY == -0x1) return 0;
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