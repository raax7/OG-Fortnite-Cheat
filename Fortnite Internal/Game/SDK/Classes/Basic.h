#pragma once
#include <string>

#include "../../../Globals.h"

#include "../SDK.h"

#include "../../../External-Libs/skCrypter.h"

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

namespace SDK
{
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



        // CUSTOM FUNCTIONS

        inline T GetByIndex(int32 Index)
        {
            if (IsValidIndex(Index))
            {
                return Data[Index];
            }

            return T();
        }

        inline void Add(const T& Element)
        {
            if (NumElements < MaxElements)
            {
                Data[NumElements++] = Element;
            }
        }

        inline void Add(T&& Element)
        {
            if (NumElements < MaxElements)
            {
                Data[NumElements++] = std::move(Element);
            }
        }

        inline void Add(T* Element)
        {
            if (NumElements < MaxElements)
            {
                Data[NumElements++] = *Element;
            }
        }
    };

    class FString : public TArray<wchar_t>
    {
    public:
        inline FString() = default;

        using TArray::TArray;

        inline FString(const wchar_t* WChar)
        {
            MaxElements = NumElements = *WChar ? (int32)std::wcslen(WChar) + 1 : 0;

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
        FName()
            : ComparisonIndex(0)
#if SEASON_20_PLUS == false
            , Number(0)
#endif
        {
        }

        FName(const wchar_t* Name)
            : ComparisonIndex(0)
#if SEASON_20_PLUS == false
            , Number(0)
#endif
        {
            if (this == nullptr || SDK::Cached::Functions::FNameConstructor == 0x0) return;

            static void(*FNameConstructor)(FName*, const wchar_t*, bool) = nullptr;

            if (!FNameConstructor)
                FNameConstructor = reinterpret_cast<void(*)(FName*, const wchar_t*, bool)>(SDK::GetBaseAddress() + SDK::Cached::Functions::FNameConstructor);

            FNameConstructor(this, Name, true);
        }
    public:
        // Members of FName - depending on configuration [WITH_CASE_PRESERVING_NAME | FNAME_OUTLINE_NUMBER]
        int32 ComparisonIndex;
#if SEASON_20_PLUS == false
        int32 Number;
#endif


        // GetDisplayIndex - returns the Id of the string depending on the configuration [default: ComparisonIndex, WITH_CASE_PRESERVING_NAME: DisplayIndex]
        inline int32 GetDisplayIndex() const
        {
            return ComparisonIndex;
        }

        // GetRawString - returns an unedited string as the engine uses it
        inline std::string GetRawString() const
        {
            if (this == nullptr || SDK::Cached::Functions::AppendString == 0x0) return skCrypt("Failed!").decrypt();

            static void(*AppendString)(const FName*, FString*) = nullptr;

            if (!AppendString)
                AppendString = reinterpret_cast<void(*)(const FName*, FString*)>(SDK::GetBaseAddress() + SDK::Cached::Functions::AppendString);

            static FString TempString;
            AppendString(const_cast<SDK::FName*>(this), &TempString);

            return TempString.ToString();
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
#if SEASON_20_PLUS
            return ComparisonIndex == Other.ComparisonIndex;
#else
            return ComparisonIndex == Other.ComparisonIndex && Number == Other.Number;
#endif
        }

        inline bool operator!=(const FName& Other) const
        {
#if SEASON_20_PLUS
            return ComparisonIndex != Other.ComparisonIndex;
#else
            return ComparisonIndex != Other.ComparisonIndex || Number != Other.Number;
#endif
        }
    };

    class FTextData
    {
    public:
        uint8 Pad[0x28];
        wchar_t* Name;
        int32 Length;
    };

    class FText
    {
    public:
        FTextData* Data;
        uint8 Pad[0x10];

        wchar_t* Get() const
        {
            if (Data)
                return Data->Name;

            return nullptr;
        }

        std::string ToString()
        {
            if (Data && Data->Name)
            {
                std::wstring Temp(Data->Name);
                return std::string(Temp.begin(), Temp.end());
            }

            return "";
        }
    };



    enum class EFunctionFlags : uint32
    {
        None = 0x00000000,

        Final = 0x00000001,
        RequiredAPI = 0x00000002,
        BlueprintAuthorityOnly = 0x00000004,
        BlueprintCosmetic = 0x00000008,
        Net = 0x00000040,
        NetReliable = 0x00000080,
        NetRequest = 0x00000100,
        Exec = 0x00000200,
        Native = 0x00000400,
        Event = 0x00000800,
        NetResponse = 0x00001000,
        Static = 0x00002000,
        NetMulticast = 0x00004000,
        UbergraphFunction = 0x00008000,
        MulticastDelegate = 0x00010000,
        Public = 0x00020000,
        Private = 0x00040000,
        Protected = 0x00080000,
        Delegate = 0x00100000,
        NetServer = 0x00200000,
        HasOutParms = 0x00400000,
        HasDefaults = 0x00800000,
        NetClient = 0x01000000,
        DLLImport = 0x02000000,
        BlueprintCallable = 0x04000000,
        BlueprintEvent = 0x08000000,
        BlueprintPure = 0x10000000,
        EditorOnly = 0x20000000,
        Const = 0x40000000,
        NetValidate = 0x80000000,

        AllFlags = 0xFFFFFFFF,
    };
    inline bool operator&(EFunctionFlags Left, EFunctionFlags Right)
    {
        using CastFlagsType = std::underlying_type<EFunctionFlags>::type;
        return (static_cast<CastFlagsType>(Left) & static_cast<CastFlagsType>(Right)) == static_cast<CastFlagsType>(Right);
    }
    inline constexpr SDK::EFunctionFlags operator|(SDK::EFunctionFlags Left, SDK::EFunctionFlags Right)
    {
        return (SDK::EFunctionFlags)((std::underlying_type<SDK::EFunctionFlags>::type)(Left) | (std::underlying_type<SDK::EFunctionFlags>::type)(Right));
    }

    enum class EClassCastFlags : uint64_t
    {
        None = 0x0000000000000000,

        Field = 0x0000000000000001,
        Int8Property = 0x0000000000000002,
        Enum = 0x0000000000000004,
        Struct = 0x0000000000000008,
        ScriptStruct = 0x0000000000000010,
        Class = 0x0000000000000020,
        ByteProperty = 0x0000000000000040,
        IntProperty = 0x0000000000000080,
        FloatProperty = 0x0000000000000100,
        UInt64Property = 0x0000000000000200,
        ClassProperty = 0x0000000000000400,
        UInt32Property = 0x0000000000000800,
        InterfaceProperty = 0x0000000000001000,
        NameProperty = 0x0000000000002000,
        StrProperty = 0x0000000000004000,
        Property = 0x0000000000008000,
        ObjectProperty = 0x0000000000010000,
        BoolProperty = 0x0000000000020000,
        UInt16Property = 0x0000000000040000,
        Function = 0x0000000000080000,
        StructProperty = 0x0000000000100000,
        ArrayProperty = 0x0000000000200000,
        Int64Property = 0x0000000000400000,
        DelegateProperty = 0x0000000000800000,
        NumericProperty = 0x0000000001000000,
        MulticastDelegateProperty = 0x0000000002000000,
        ObjectPropertyBase = 0x0000000004000000,
        WeakObjectProperty = 0x0000000008000000,
        LazyObjectProperty = 0x0000000010000000,
        SoftObjectProperty = 0x0000000020000000,
        TextProperty = 0x0000000040000000,
        Int16Property = 0x0000000080000000,
        DoubleProperty = 0x0000000100000000,
        SoftClassProperty = 0x0000000200000000,
        Package = 0x0000000400000000,
        Level = 0x0000000800000000,
        Actor = 0x0000001000000000,
        PlayerController = 0x0000002000000000,
        Pawn = 0x0000004000000000,
        SceneComponent = 0x0000008000000000,
        PrimitiveComponent = 0x0000010000000000,
        SkinnedMeshComponent = 0x0000020000000000,
        SkeletalMeshComponent = 0x0000040000000000,
        Blueprint = 0x0000080000000000,
        DelegateFunction = 0x0000100000000000,
        StaticMeshComponent = 0x0000200000000000,
        MapProperty = 0x0000400000000000,
        SetProperty = 0x0000800000000000,
        EnumProperty = 0x0001000000000000,
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
#if SEASON_20_PLUS
        double M[4][4];
#else
        float M[4][4];
#endif
    };

    struct FVector
    {
    public:
#if SEASON_20_PLUS
        double X;
        double Y;
        double Z;
#else
        float X;
        float Y;
        float Z;
#endif
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

        FVector operator+(const FVector& Other) const
        {
            return FVector(X + Other.X, Y + Other.Y, Z + Other.Z);
        }

        FVector operator-(const FVector& Other) const
        {
            return FVector(X - Other.X, Y - Other.Y, Z - Other.Z);
        }

        FVector operator*(decltype(X) Scalar) const
        {
            return FVector(X * Scalar, Y * Scalar, Z * Scalar);
        }

        FVector operator/(decltype(X) Scalar) const
        {
            if (Scalar != 0.0)
            {
                return FVector(X / Scalar, Y / Scalar, Z / Scalar);
            }
            else
            {
                return FVector();
            }
        }



        float operator|(const FVector& V) const
        {
            return X * V.X + Y * V.Y + Z * V.Z;
        }

        FVector operator-() const
        {
            return FVector(-X, -Y, -Z);
        }

        FVector operator^ (const FVector& V) const
        {
            return FVector
            (
                Y * V.Z - Z * V.Y,
                Z * V.X - X * V.Z,
                X * V.Y - Y * V.X
            );
        }

        inline float Distance(FVector v)
        {
            return float(sqrt(pow(v.X - X, 2.0) + pow(v.Y - Y, 2.0) + pow(v.Z - Z, 2.0)));
        }

        inline float Dot(const FVector& Other) const
        {
            return X * Other.X + Y * Other.Y + Z * Other.Z;
        }

        bool Normalize(float Tolerance = 1.e-8f);
    };

    struct FQuat
    {
    public:
#if SEASON_20_PLUS
        double X;
        double Y;
        double Z;
        double W;
#else
        float X;
        float Y;
        float Z;
        float W;
#endif
    };

    struct FRotator
    {
    public:
#if SEASON_20_PLUS
        double Pitch;
        double Yaw;
        double Roll;
#else
        float Pitch;
        float Yaw;
        float Roll;
#endif

        inline FRotator()
            : Pitch(0.0), Yaw(0.0), Roll(0.0)
        {
        }

        inline FRotator(decltype(Pitch) Value)
            : Pitch(Value), Yaw(Value), Roll(Value)
        {
        }

        inline FRotator(decltype(Pitch) pitch, decltype(Yaw) yaw, decltype(Roll) roll)
            : Pitch(pitch), Yaw(yaw), Roll(roll)
        {
        }

        inline bool operator==(const FRotator& Other) const
        {
            return Pitch == Other.Pitch && Yaw == Other.Yaw && Roll == Other.Roll;
        }

        inline bool operator!=(const FRotator& Other) const
        {
            return Pitch != Other.Pitch || Yaw != Other.Yaw || Roll != Other.Roll;
        }

        FRotator operator+(const FRotator& Other) const
        {
            return FRotator(Pitch + Other.Pitch, Yaw + Other.Yaw, Roll + Other.Roll);
        }

        FRotator operator-(const FRotator& Other) const
        {
            return FRotator(Pitch - Other.Pitch, Yaw - Other.Yaw, Roll - Other.Roll);
        }

        FRotator operator*(decltype(Pitch) Scalar) const
        {
            return FRotator(Pitch * Scalar, Yaw * Scalar, Roll * Scalar);
        }

        FRotator operator/(decltype(Pitch) Scalar) const
        {
            return FRotator(Pitch / Scalar, Yaw / Scalar, Roll / Scalar);
        }

        inline float GetPitchDistance(const FRotator& Other) const
        {
            float delta = (float)fmod(Pitch - Other.Pitch, 360.0);
            while (delta > 180.0)
            {
                delta -= 360.0;
            }
            while (delta < -180.0)
            {
                delta += 360.0;
            }
            return delta;
        }

        inline float GetYawDistance(const FRotator& Other) const
        {
            float delta = (float)fmod(Yaw - Other.Yaw, 360.0);
            while (delta > 180.0)
            {
                delta -= 360.0;
            }
            while (delta < -180.0)
            {
                delta += 360.0;
            }
            return delta;
        }
    };

    struct FVector2D
    {
    public:
#if SEASON_20_PLUS
        double X;
        double Y;
#else
        float X;
        float Y;
#endif

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

        FVector2D operator+(const FVector2D& Other) const
        {
            return FVector2D(X + Other.X, Y + Other.Y);
        }

        FVector2D operator-(const FVector2D& Other) const
        {
            return FVector2D(X - Other.X, Y - Other.Y);
        }

        FVector2D operator*(decltype(X) Scalar) const
        {
            return FVector2D(X * Scalar, Y * Scalar);
        }

        FVector2D operator/(decltype(X) Scalar) const
        {
            return FVector2D(X / Scalar, Y / Scalar);
        }

        inline float Distance(FVector2D v)
        {
            return float(sqrt(pow(v.X - X, 2.0) + pow(v.Y - Y, 2.0)));
        }
    };

    struct FTransform
    {
    public:
        FQuat Rotation;
        FVector Translation;
        uint8 Pad_AE[0x4];
        FVector Scale3D;
        uint8 Pad_AF[0x4];
    };



    struct FLinearColor
    {
    public:
        float                                        R;                                                 // 0x0(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        float                                        G;                                                 // 0x4(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        float                                        B;                                                 // 0x8(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        float                                        A;                                                 // 0xC(0x4)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

        inline FLinearColor()
            : R(0.0f), G(0.0f), B(0.0f), A(1.0f)
        {
        }

        inline FLinearColor(float InR, float InG, float InB, float InA)
            : R(InR), G(InG), B(InB), A(InA)
        {
        }
    };



    class FWeakObjectPtr
    {
    protected:
        int32		ObjectIndex;
        int32		ObjectSerialNumber;

    public:
        class UObject* Get() const;

        class UObject* operator->() const;

        bool operator==(const FWeakObjectPtr& Other) const;
        bool operator!=(const FWeakObjectPtr& Other) const;

        bool operator==(const class UObject* Other) const;
        bool operator!=(const class UObject* Other) const;
    };

    template<typename UEType>
    class TWeakObjectPtr : FWeakObjectPtr
    {
    public:
        UEType* Get() const
        {
            return static_cast<UEType*>(FWeakObjectPtr::Get());
        }

        UEType* operator->() const
        {
            return static_cast<UEType*>(FWeakObjectPtr::Get());
        }
    };
}