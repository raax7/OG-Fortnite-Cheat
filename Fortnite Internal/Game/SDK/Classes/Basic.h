#ifndef BASIC_H
#define BASIC_H

#include <Windows.h>
#include <string>
#include "CoreUObject_classes.h"
#include "../../../Utilitys/skCrypter.h"

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

namespace SDK {
	inline uintptr_t AppendStringOffset;
	inline uintptr_t GetBoneMatrix;

	template<class T>
	class TArray
	{
	protected:
		T* Data;
		int32 NumElements;
		int32 MaxElements;

	public:

		inline TArray()
			:NumElements(0), MaxElements(0), Data(nullptr)
		{
		}

		inline TArray(int32 Size)
			: NumElements(0), MaxElements(Size), Data(reinterpret_cast<T*>(malloc(sizeof(T)* Size)))
		{
		}

		inline T& operator[](uint32 Index)
		{
			return Data[Index];
		}
		inline const T& operator[](uint32 Index) const
		{
			return Data[Index];
		}

		inline int32 Num()
		{
			return NumElements;
		}

		inline int32 Max()
		{
			return MaxElements;
		}

		inline int32 GetSlack()
		{
			return MaxElements - NumElements;
		}

		inline bool IsValid()
		{
			return Data != nullptr;
		}

		inline bool IsValidIndex(int32 Index)
		{
			return Index >= 0 && Index < NumElements;
		}

		inline void ResetNum()
		{
			NumElements = 0;
		}
	};

	class FString : public TArray<wchar_t>
	{
	public:
		inline FString() = default;

		using TArray::TArray;

		inline FString(const wchar_t* WChar)
		{
			MaxElements = NumElements = *WChar ? std::wcslen(WChar) + 1 : 0;

			if (NumElements)
			{
				Data = const_cast<wchar_t*>(WChar);
			}
		}

		inline FString operator=(const wchar_t*&& Other)
		{
			return FString(Other);
		}

		inline std::wstring ToWString()
		{
			if (IsValid())
			{
				return Data;
			}

			return L"";
		}

		inline std::string ToString()
		{
			if (IsValid())
			{
				std::wstring WData(Data);
				return std::string(WData.begin(), WData.end());
			}

			return "";
		}
	};

	class FName
	{
	public:
		// Members of FName - depending on configuration [WITH_CASE_PRESERVING_NAME | FNAME_OUTLINE_NUMBER]
		int32 ComparisonIndex;
		int32 Number;


		// GetDisplayIndex - returns the Id of the string depending on the configuration [default: ComparisonIndex, WITH_CASE_PRESERVING_NAME: DisplayIndex]
		inline int32 GetDisplayIndex() const
		{
			return ComparisonIndex;
		}

		// GetRawString - returns an unedited string as the engine uses it
		inline std::string GetRawString() const
		{
			if (!this) return skCrypt("None").decrypt();
			if (!SDK::AppendStringOffset) return skCrypt("AppendString not found").decrypt() + std::to_string(SDK::AppendStringOffset);

			thread_local FString TempString(1024);
			static void(*AppendString)(const FName*, FString&) = nullptr;

			if (!AppendString)
				AppendString = reinterpret_cast<void(*)(const FName*, FString&)>(uintptr_t(*(uintptr_t*)(__readgsqword(0x60) + 0x10)) + SDK::AppendStringOffset);

			AppendString(this, TempString);

			std::string OutputString = TempString.ToString();
			TempString.ResetNum();

			return OutputString;
		}

		// ToString - returns an edited string as it's used by most SDKs ["/Script/CoreUObject" -> "CoreUObject"]
		inline std::string ToString() const
		{
			std::string OutputString = GetRawString();

			size_t pos = OutputString.rfind('/');

			if (pos == std::string::npos)
				return OutputString;

			return OutputString.substr(pos + 1);
		}

		inline bool operator==(const FName& Other) const
		{
			return ComparisonIndex == Other.ComparisonIndex && Number == Other.Number;
		}

		inline bool operator!=(const FName& Other) const
		{
			return ComparisonIndex != Other.ComparisonIndex || Number != Other.Number;
		}
	};

	enum class EClassCastFlags : uint64_t
	{
		None				= 0x0000000000000000,

		Field				= 0x0000000000000001,
		Int8Property		= 0x0000000000000002,
		Enum				= 0x0000000000000004,
		Struct				= 0x0000000000000008,
		ScriptStruct		= 0x0000000000000010,
		Class				= 0x0000000000000020,
		ByteProperty		= 0x0000000000000040,
		IntProperty			= 0x0000000000000080,
		FloatProperty		= 0x0000000000000100,
		UInt64Property		= 0x0000000000000200,
		ClassProperty		= 0x0000000000000400,
		UInt32Property		= 0x0000000000000800,
		InterfaceProperty	= 0x0000000000001000,
		NameProperty		= 0x0000000000002000,
		StrProperty			= 0x0000000000004000,
		Property			= 0x0000000000008000,
		ObjectProperty		= 0x0000000000010000,
		BoolProperty		= 0x0000000000020000,
		UInt16Property		= 0x0000000000040000,
		Function			= 0x0000000000080000,
		StructProperty		= 0x0000000000100000,
		ArrayProperty		= 0x0000000000200000,
		Int64Property		= 0x0000000000400000,
		DelegateProperty	= 0x0000000000800000,
		NumericProperty		= 0x0000000001000000,
		MulticastDelegateProperty = 0x0000000002000000,
		ObjectPropertyBase	= 0x0000000004000000,
		WeakObjectProperty	= 0x0000000008000000,
		LazyObjectProperty	= 0x0000000010000000,
		SoftObjectProperty	= 0x0000000020000000,
		TextProperty		= 0x0000000040000000,
		Int16Property		= 0x0000000080000000,
		DoubleProperty		= 0x0000000100000000,
		SoftClassProperty	= 0x0000000200000000,
		Package				= 0x0000000400000000,
		Level				= 0x0000000800000000,
		Actor				= 0x0000001000000000,
		PlayerController	= 0x0000002000000000,
		Pawn				= 0x0000004000000000,
		SceneComponent		= 0x0000008000000000,
		PrimitiveComponent	= 0x0000010000000000,
		SkinnedMeshComponent = 0x0000020000000000,
		SkeletalMeshComponent = 0x0000040000000000,
		Blueprint			= 0x0000080000000000,
		DelegateFunction	= 0x0000100000000000,
		StaticMeshComponent = 0x0000200000000000,
		MapProperty			= 0x0000400000000000,
		SetProperty			= 0x0000800000000000,
		EnumProperty		= 0x0001000000000000,
	};

	inline bool operator&(EClassCastFlags Left, EClassCastFlags Right)
	{
		using CastFlagsType = std::underlying_type<EClassCastFlags>::type;
		return (static_cast<CastFlagsType>(Left) & static_cast<CastFlagsType>(Right)) == static_cast<CastFlagsType>(Right);
	}
	inline constexpr SDK::EClassCastFlags operator|(SDK::EClassCastFlags Left, SDK::EClassCastFlags Right)
	{
		return (SDK::EClassCastFlags)((std::underlying_type<SDK::EClassCastFlags>::type)(Left) | (std::underlying_type<SDK::EClassCastFlags>::type)(Right));
	}


	// In UE5, most structs use doubles (8 bytes) instead of floats (4 bytes)
	struct FMatrix
	{
	public:
		float M[4][4];
	};

	struct FVector
	{
	public:
		float                                        X;                                                 // 0x0(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                        Y;                                                 // 0x4(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                        Z;                                                 // 0x8(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

		inline FVector()
			: X(0.0), Y(0.0), Z(0.0)
		{
		}

		inline FVector(decltype(X) Value)
			: X(Value), Y(Value), Z(Value)
		{
		}

		inline FVector(decltype(X) x, decltype(Y) y, decltype(Z) z)
			: X(x), Y(y), Z(z)
		{
		}

		inline bool operator==(const FVector& Other) const
		{
			return X == Other.X && Y == Other.Y && Z == Other.Z;
		}

		inline bool operator!=(const FVector& Other) const
		{
			return X != Other.X || Y != Other.Y || Z != Other.Z;
		}

		FVector operator+(const FVector& Other) const;

		FVector operator-(const FVector& Other) const;

		FVector operator*(decltype(X) Scalar) const;

		FVector operator/(decltype(X) Scalar) const;
	};

	struct FVector2D
	{
	public:
		float                                        X;                                                 // 0x0(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                        Y;                                                 // 0x4(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

		inline FVector2D()
			: X(0.0), Y(0.0)
		{
		}

		inline FVector2D(decltype(X) Value)
			: X(Value), Y(Value)
		{
		}

		inline FVector2D(decltype(X) x, decltype(Y) y)
			: X(x), Y(y)
		{
		}

		inline bool operator==(const FVector2D& Other) const
		{
			return X == Other.X && Y == Other.Y;
		}

		inline bool operator!=(const FVector2D& Other) const
		{
			return X != Other.X || Y != Other.Y;
		}

		FVector2D operator+(const FVector2D& Other) const;

		FVector2D operator-(const FVector2D& Other) const;

		FVector2D operator*(decltype(X) Scalar) const;

		FVector2D operator/(decltype(X) Scalar) const;
	};



	struct FLinearColor
	{
	public:
		float                                        R;                                                 // 0x0(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                        G;                                                 // 0x4(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                        B;                                                 // 0x8(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                        A;                                                 // 0xC(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};
}

#endif