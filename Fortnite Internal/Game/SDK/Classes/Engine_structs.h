#ifndef ENGINE_STRUCTS_H
#define ENGINE_STRUCTS_H

#include "Engine_classes.h"

namespace SDK {
	enum class ETraceTypeQuery : uint8
	{
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

	enum class EDrawDebugTrace : uint8
	{
		None = 0,
		ForOneFrame = 1,
		ForDuration = 2,
		Persistent = 3,
		EDrawDebugTrace_MAX = 4,
	};

	struct FHitResult
	{
	public:
		uint8                                        bBlockingHit : 1;                                  // Mask: 0x1, PropSize: 0x10x0(0x1)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                        bStartPenetrating : 1;                             // Mask: 0x2, PropSize: 0x10x0(0x1)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                        BitPad_1D0 : 6;                                    // Fixing Bit-Field Size  [ Dumper-7 ]
		uint8                                        Pad_2126[0x3];                                     // Fixing Size After Last Property  [ Dumper-7 ]
		int32                                        FaceIndex;                                         // 0x4(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                        Time;                                              // 0x8(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                        Distance;                                          // 0xC(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct SDK::FVector							 Location;                                          // 0x10(0xC)(NoDestructor, NativeAccessSpecifierPublic)
		struct SDK::FVector							 ImpactPoint;                                       // 0x1C(0xC)(NoDestructor, NativeAccessSpecifierPublic)
		struct SDK::FVector							 Normal;                                            // 0x28(0xC)(NoDestructor, NativeAccessSpecifierPublic)
		struct SDK::FVector							 ImpactNormal;                                      // 0x34(0xC)(NoDestructor, NativeAccessSpecifierPublic)
		struct SDK::FVector							 TraceStart;                                        // 0x40(0xC)(NoDestructor, NativeAccessSpecifierPublic)
		struct SDK::FVector							 TraceEnd;                                          // 0x4C(0xC)(NoDestructor, NativeAccessSpecifierPublic)
		float                                        PenetrationDepth;                                  // 0x58(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32                                        Item;                                              // 0x5C(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		char										 PhysMaterial[0x8];                                 // 0x60(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		char										 Actor[0x8];                                        // 0x68(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		char										 Component[0x8];                                    // 0x70(0x8)(ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		SDK::FName                                   BoneName;                                          // 0x78(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		SDK::FName                                   MyBoneName;                                        // 0x80(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};
}

enum class EFortItemTier : uint8
{
	No_Tier = 0,
	I = 1,
	II = 2,
	III = 3,
	IV = 4,
	V = 5,
	VI = 6,
	VII = 7,
	VIII = 8,
	IX = 9,
	X = 10,
	NumItemTierValues = 11,
	EFortItemTier_MAX = 12,
};

#endif // ENGINE_STRUCTS_H