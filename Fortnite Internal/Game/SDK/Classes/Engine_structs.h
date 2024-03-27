#pragma once
#include "../SDK.h"

#include "Basic.h"

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

namespace SDK {
	enum class ETraceTypeQuery : uint8 {
		TraceTypeQuery1 = 0,
		TraceTypeQuery2 = 1,
		TraceTypeQuery3 = 2,
		TraceTypeQuery4 = 3,
		TraceTypeQuery5 = 4,
		TraceTypeQuery6 = 5,
		TraceTypeQuery7 = 6,
		TraceTypeQuery8 = 7,
		TraceTypeQuery9 = 8,
		TraceTypeQuery10 = 9,
		TraceTypeQuery11 = 10,
		TraceTypeQuery12 = 11,
		TraceTypeQuery13 = 12,
		TraceTypeQuery14 = 13,
		TraceTypeQuery15 = 14,
		TraceTypeQuery16 = 15,
		TraceTypeQuery17 = 16,
		TraceTypeQuery18 = 17,
		TraceTypeQuery19 = 18,
		TraceTypeQuery20 = 19,
		TraceTypeQuery21 = 20,
		TraceTypeQuery22 = 21,
		TraceTypeQuery23 = 22,
		TraceTypeQuery24 = 23,
		TraceTypeQuery25 = 24,
		TraceTypeQuery26 = 25,
		TraceTypeQuery27 = 26,
		TraceTypeQuery28 = 27,
		TraceTypeQuery29 = 28,
		TraceTypeQuery30 = 29,
		TraceTypeQuery31 = 30,
		TraceTypeQuery32 = 31,
		TraceTypeQuery_MAX = 32,
		ETraceTypeQuery_MAX = 33,
	};

	enum class ECollisionChannel : uint8 {
		ECC_WorldStatic = 0,
		ECC_WorldDynamic = 1,
		ECC_Pawn = 2,
		ECC_Visibility = 3,
		ECC_Camera = 4,
		ECC_PhysicsBody = 5,
		ECC_Vehicle = 6,
		ECC_Destructible = 7,
		ECC_EngineTraceChannel1 = 8,
		ECC_EngineTraceChannel2 = 9,
		ECC_EngineTraceChannel3 = 10,
		ECC_EngineTraceChannel4 = 11,
		ECC_EngineTraceChannel5 = 12,
		ECC_EngineTraceChannel6 = 13,
		ECC_GameTraceChannel1 = 14,
		ECC_GameTraceChannel2 = 15,
		ECC_GameTraceChannel3 = 16,
		ECC_GameTraceChannel4 = 17,
		ECC_GameTraceChannel5 = 18,
		ECC_GameTraceChannel6 = 19,
		ECC_GameTraceChannel7 = 20,
		ECC_GameTraceChannel8 = 21,
		ECC_GameTraceChannel9 = 22,
		ECC_GameTraceChannel10 = 23,
		ECC_GameTraceChannel11 = 24,
		ECC_GameTraceChannel12 = 25,
		ECC_GameTraceChannel13 = 26,
		ECC_GameTraceChannel14 = 27,
		ECC_GameTraceChannel15 = 28,
		ECC_GameTraceChannel16 = 29,
		ECC_GameTraceChannel17 = 30,
		ECC_GameTraceChannel18 = 31,
		ECC_OverlapAll_Deprecated = 32,
		ECC_MAX = 33,
	};

	enum class EDrawDebugTrace : uint8 {
		None = 0,
		ForOneFrame = 1,
		ForDuration = 2,
		Persistent = 3,
		EDrawDebugTrace_MAX = 4,
	};

	enum class EMIDCreationFlags : uint8 {
		None = 0,
		Transient = 1,
		EMIDCreationFlags_MAX = 2,
	};

	enum class ENetRole : uint8
	{
		ROLE_None = 0,
		ROLE_SimulatedProxy = 1,
		ROLE_AutonomousProxy = 2,
		ROLE_Authority = 3,
		ROLE_MAX = 4,
	};

	struct FHitResult
	{
	public:
		char UnknownData[0x100]; // Dummy data

		// Values

		SDK::FVector TraceStart() {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::HitResult::TraceStart == -0x1) return SDK::FVector();
			return *(SDK::FVector*)((uintptr_t)this + SDK::Cached::Offsets::HitResult::TraceStart);
		}

		void SetTraceStart(FVector NewTraceStart) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::HitResult::TraceStart == -0x1) return;
			*(SDK::FVector*)((uintptr_t)this + SDK::Cached::Offsets::HitResult::TraceStart) = NewTraceStart;
		}

		void SetDistance(float NewDistance) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::HitResult::Distance == -0x1) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::HitResult::Distance) = NewDistance;
		}
	};

	struct FMinimalViewInfo {
	public:
		// VALUES

		void SetLocation(SDK::FVector NewLocation) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::MinimalViewInfo::Location == -0x1) return;
			*(SDK::FVector*)((uintptr_t)this + SDK::Cached::Offsets::MinimalViewInfo::Location) = NewLocation;
		}

		void SetRotation(SDK::FRotator NewRotation) {
			if (SDK::IsValidPointer(this) == false || SDK::Cached::Offsets::MinimalViewInfo::Rotation == -0x1) return;
			*(SDK::FRotator*)((uintptr_t)this + SDK::Cached::Offsets::MinimalViewInfo::Rotation) = NewRotation;
		}
	};

	enum class EStereoscopicPass
	{
		eSSP_FULL,
		eSSP_LEFT_EYE,
		eSSP_RIGHT_EYE,
		eSSP_LEFT_EYE_SIDE,
		eSSP_RIGHT_EYE_SIDE,
	};

	struct FKey
	{
	public:
		class FName KeyName;
		uint8 Pad_1196[0x10];
	};
}