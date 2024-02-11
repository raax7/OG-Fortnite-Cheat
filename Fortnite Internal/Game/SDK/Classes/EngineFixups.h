#pragma once
#include "CoreUObject_classes.h"
#include "../../Game.h"
#include "../../../Utilities/Math.h"

inline SDK::FVector2D ProjectWorldToScreen(SDK::FVector WorldLocation, SDK::FVector CameraLocation, SDK::FRotator CameraRotation, float CameraFOV)
{
	SDK::FVector2D Screenlocation = SDK::FVector2D(0, 0);

	float radPitch = (CameraRotation.Pitch * float(M_PI) / 180.f);
	float radYaw = (CameraRotation.Yaw * float(M_PI) / 180.f);
	float radRoll = (CameraRotation.Roll * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	SDK::FVector vAxisX, vAxisY, vAxisZ;
	vAxisX = SDK::FVector(CP * CY, CP * SY, SP);
	vAxisY = SDK::FVector(SR * SP * CY - CR * SY, SR * SP * SY + CR * CY, -SR * CP);
	vAxisZ = SDK::FVector(-(CR * SP * CY + SR * SY), CY * SR - CR * SP * SY, CR * CP);

	SDK::FVector vDelta = WorldLocation - CameraLocation;
	SDK::FVector vTransformed = SDK::FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.Z < 1.f)
		vTransformed.Z = 1.f;

	float FovAngle = CameraFOV;
	float ScreenCenterX = (float)Game::ScreenWidth / 2.0f; // REMOVE HARD CODED
	float ScreenCenterY = (float)Game::ScreenHeight / 2.0f; // REMOVE HARD CODED

	Screenlocation.X = ScreenCenterX + vTransformed.X * (ScreenCenterX / tan(FovAngle * (float)M_PI / 360.f)) / vTransformed.Z;
	Screenlocation.Y = ScreenCenterY - vTransformed.Y * (ScreenCenterX / tan(FovAngle * (float)M_PI / 360.f)) / vTransformed.Z;

	return Screenlocation;
}

namespace SDK {
	// Forward Declarations

	class TUObjectArray;
	class Chunked_TUObjectArray;
	class Fixed_TUObjectArray;
	struct FUObjectItem;



	struct FUObjectItem
	{
		class UObject* Object;
		uint8 Pad_0[0x10];

	};

	class Chunked_TUObjectArray
	{
	public:
		enum
		{
			ElementsPerChunk = 0x10000,
		};

	public:
		static inline auto DecryptPtr = [](void* ObjPtr) -> uint8*
			{
				return reinterpret_cast<uint8*>(ObjPtr);
			};

		FUObjectItem** Objects;
		uint8 Pad_0[0x08];
		int32 MaxElements;
		int32 NumElements;
		int32 MaxChunks;
		int32 NumChunks;


	public:
		inline int32 Num() const
		{
			return NumElements;
		}

		inline FUObjectItem** GetDecrytedObjPtr() const
		{
			return reinterpret_cast<FUObjectItem**>(DecryptPtr(Objects));
		}

		inline class UObject* GetByIndex(const int32 Index) const
		{
			if (Index < 0 || Index > NumElements)
				return nullptr;

			const int32 ChunkIndex = Index / ElementsPerChunk;
			const int32 InChunkIdx = Index % ElementsPerChunk;

			return GetDecrytedObjPtr()[ChunkIndex][InChunkIdx].Object;
		}
	};
	class Fixed_TUObjectArray
	{
	public:
		static inline auto DecryptPtr = [](void* ObjPtr) -> uint8*
			{
				return reinterpret_cast<uint8*>(ObjPtr);
			};

	public:
		FUObjectItem* Objects;
		int32 MaxElements;
		int32 NumElements;

	public:
		inline int Num() const
		{
			return NumElements;
		}

		inline FUObjectItem* GetDecrytedObjPtr() const
		{
			return reinterpret_cast<FUObjectItem*>(DecryptPtr(Objects));
		}

		inline class UObject* GetByIndex(const int32 Index) const
		{
			if (Index < 0 || Index > NumElements)
				return nullptr;

			return GetDecrytedObjPtr()[Index].Object;
		}
	};

	class TUObjectArray {
	public:
		bool IsChunked;
		Chunked_TUObjectArray* ChunkedObjects;
		Fixed_TUObjectArray* FixedObjects;

		int32 Num() {
			if (IsChunked && ChunkedObjects) {
				return ChunkedObjects->Num();
			}
			else if (!IsChunked && FixedObjects) {
				return FixedObjects->Num();
			}

			return 0;
		}

		UObject* GetByIndex(int32 Index) {
			if (IsChunked && ChunkedObjects) {
				return ChunkedObjects->GetByIndex(Index);
			}
			else if (!IsChunked && FixedObjects) {
				return FixedObjects->GetByIndex(Index);
			}

			return nullptr;
		}
	};
}