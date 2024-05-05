#pragma once
#include <vector>

#include "Basic.h"
#include "EngineFixups.h"

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

struct FunctionSearch
{
    SDK::FName ClassName;		// The name of the class	
    SDK::FName FunctionName;	// The name of the function
    void** Function;			// A pointer to save the function address to

    bool operator==(const FunctionSearch& rhs)
    {
        return ClassName == rhs.ClassName
            && FunctionName == rhs.FunctionName
            && Function == rhs.Function;
    }
};
struct OffsetSearch
{
    SDK::FName ClassName;		// The name of the class
    SDK::FName PropertyName;	// The name of the property
    uintptr_t* Offset;			// A pointer to save the offset to
    uintptr_t* Mask;			// A pointer to save the bitfield mask to

    bool operator==(const OffsetSearch& rhs)
    {
        return ClassName == rhs.ClassName
            && PropertyName == rhs.PropertyName
            && Offset == rhs.Offset
            && Mask == rhs.Mask;
    }
};

namespace SDK
{
    // Forward Declarations

    class UObject;
    class FField;
    class FProperty;
    class UStruct;
    class UProperty;
    class UClass;



    class UObject
    {
    public:
        static TUObjectArray						 ObjectArray;
        void** Vft;                                               // (0x00[0x08]) NOT AUTO-GENERATED PROPERTY
        int32                                        Flags;                                             // (0x08[0x04]) NOT AUTO-GENERATED PROPERTY
        int32                                        Index;                                             // (0x0C[0x04]) NOT AUTO-GENERATED PROPERTY
        class UClass* Class;                                             // (0x10[0x08]) NOT AUTO-GENERATED PROPERTY
        class FName                                  Name;                                              // (0x18[0x08]) NOT AUTO-GENERATED PROPERTY
        class UObject* Outer;                                             // (0x20[0x08]) NOT AUTO-GENERATED PROPERTY

        void ProcessEvent(void* fn, void* parms);

        bool IsDefaultObject() const
        {
            return (Flags & 0x10) == 0x10;
        }

        bool HasTypeFlag(EClassCastFlags TypeFlag) const;

        std::string GetName() const;
        std::string GetFullName() const;


        static uint32_t GetPropertyOffset(UProperty* Property);
        static uint32_t GetPropertyOffset(FField* Field, std::string PropertyName);

        template<typename UEType = UObject>
        static UEType* FindObject(const std::string& FullName, EClassCastFlags RequiredType = EClassCastFlags::None)
        {
            for (int i = 0; i < ObjectArray.Num(); ++i)
            {
                UObject* Object = ObjectArray.GetByIndex(i);

                if (!Object)
                    continue;

                if (Object->HasTypeFlag(RequiredType) && Object->GetFullName() == FullName)
                {
                    return static_cast<UEType*>(Object);
                }
            }

            return nullptr;
        }

        template<typename UEType = UObject>
        static UEType* FindObjectFast(const std::string& Name, EClassCastFlags RequiredType = EClassCastFlags::None)
        {
            for (int i = 0; i < ObjectArray.Num(); ++i)
            {
                UObject* Object = ObjectArray.GetByIndex(i);

                if (!Object)
                    continue;

                if (Object->HasTypeFlag(RequiredType) && Object->GetName() == Name)
                {
                    return static_cast<UEType*>(Object);
                }
            }

            return nullptr;
        }

        template<typename UEType = UObject>
        static UEType* FindObjectFastInOuter(std::string Name, std::string Outer)
        {
            for (int i = 0; i < ObjectArray.Num(); ++i)
            {
                UObject* Object = ObjectArray.GetByIndex(i);

                if (!Object)
                    continue;

                if (Object->GetName() == Name && Object->Outer->GetName() == Outer)
                {
                    return reinterpret_cast<UEType*>(Object);
                }
            }

            return nullptr;
        }


        static class UClass* FindClass(const std::string& ClassFullName)
        {
            return FindObject<class UClass>(ClassFullName, EClassCastFlags::Class);
        }

        static class UClass* FindClassFast(const std::string& ClassName)
        {
            return FindObjectFast<class UClass>(ClassName, EClassCastFlags::Class);
        }



        static void SetupObjects(std::vector<FunctionSearch>& Functions, std::vector<OffsetSearch>& Offsets);


        bool IsA(class UClass* Clss) const;
    };

    template <typename T, bool ForceCast = false>
    static inline T* Cast(UObject* Object)
    {
        if (ForceCast || (IsValidPointer(Object) && Object->IsA(T::StaticClass())))
        {
            return (T*)Object;
        }

        return nullptr;
    }


#ifdef _MSC_VER
#pragma pack(push, 0x1)
#endif
    class FFieldClass
    {
    public:
        FName                                        Name;                                              // (0x00[0x08]) NOT AUTO-GENERATED PROPERTY
        uint64                                       Id;                                                // (0x08[0x08]) NOT AUTO-GENERATED PROPERTY
        EClassCastFlags                              CastFlags;                                         // (0x10[0x08]) NOT AUTO-GENERATED PROPERTY
        int32										 ClassFlags;                                        // (0x18[0x04]) NOT AUTO-GENERATED PROPERTY
        uint8                                        Pad_74C3[0x4];                                     // Fixing Size After Last (Predefined) Property  [ Dumper-7 ]
        FFieldClass* SuperClass;                                        // (0x20[0x08]) NOT AUTO-GENERATED PROPERTY
    };
#ifdef _MSC_VER
#pragma pack(pop)
#endif

#ifdef _MSC_VER
#pragma pack(push, 0x1)
#endif
    class FField
    {
    public:
        void* Vft;                                               // (0x00[0x08]) NOT AUTO-GENERATED PROPERTY
        FFieldClass* Class;                                             // (0x08[0x08]) NOT AUTO-GENERATED PROPERTY
        char										 Owner[0x10];                                       // (0x10[0x10]) NOT AUTO-GENERATED PROPERTY
        FField* Next;                                              // (0x20[0x08]) NOT AUTO-GENERATED PROPERTY
        FName                                        Name;                                              // (0x28[0x08]) NOT AUTO-GENERATED PROPERTY
        int32                                        Flags;                                             // (0x30[0x04]) NOT AUTO-GENERATED PROPERTY

        bool HasTypeFlag(EClassCastFlags TypeFlag) const;
    };
#ifdef _MSC_VER
#pragma pack(pop)
#endif



#ifdef _MSC_VER
#pragma pack(push, 0x1)
#endif
    class FProperty : public FField
    {
    public:
        uint8                                        Pad_74C4[0x8];                                     // Fixing Size After Last (Predefined) Property  [ Dumper-7 ]
        int32                                        ElementSize;                                       // (0x3C[0x04]) NOT AUTO-GENERATED PROPERTY
        uint64                                       PropertyFlags;                                     // (0x40[0x08]) NOT AUTO-GENERATED PROPERTY
        uint8                                        Pad_74C5[0x4];                                     // Fixing Size After Last (Predefined) Property  [ Dumper-7 ]
        int32                                        Offset;                                            // (0x4C[0x04]) NOT AUTO-GENERATED PROPERTY
    };
#ifdef _MSC_VER
#pragma pack(pop)
#endif

#ifdef _MSC_VER
#pragma pack(push, 0x1)
#endif
    class FBoolProperty : public FProperty
    {
    public:
        uint8                                        Pad_74C7[0x28];                                    // Fixing Size After Last (Predefined) Property  [ Dumper-7 ]
        uint8                                        FieldSize;                                         // (0x78[0x01]) NOT AUTO-GENERATED PROPERTY
        uint8                                        ByteOffset;                                        // (0x79[0x01]) NOT AUTO-GENERATED PROPERTY
        uint8                                        ByteMask;                                          // (0x7A[0x01]) NOT AUTO-GENERATED PROPERTY
        uint8                                        FieldMask;                                         // (0x7B[0x01]) NOT AUTO-GENERATED PROPERTY
    };
#ifdef _MSC_VER
#pragma pack(pop)
#endif

    class UField : public UObject
    {
    public:
        static uint32 NextOffset;

        class UField* Next()
        {
            if (SDK::IsValidPointer(this) == false) return nullptr;
            return (UField*)(*(uintptr_t*)((uintptr_t)this + NextOffset));
        }
    };

    class UStruct : public UObject
    {
    public:
        static uint32 SuperOffset;
        static uint32 ChildPropertiesOffset;
        static uint32 ChildrenOffset;

        class UStruct* Super()
        {
            if (SDK::IsValidPointer(this) == false) return nullptr;
            return (UStruct*)(*(uintptr_t*)((uintptr_t)this + SuperOffset));
        }

        class UField* Children()
        {
            if (SDK::IsValidPointer(this) == false) return nullptr;
            return (UField*)(*(uintptr_t*)((uintptr_t)this + ChildrenOffset));
        }

        class FField* ChildProperties()
        {
            if (SDK::IsValidPointer(this) == false) return nullptr;
            return (FField*)(*(uintptr_t*)((uintptr_t)this + ChildPropertiesOffset));
        }
    };

    class UProperty : public UObject
    {
    public:
        static uint32 OffsetOffset;

        int32 Offset()
        {
            if (SDK::IsValidPointer(this) == false) return 0;
            return *(int32*)((uintptr_t)this + OffsetOffset);
        }
    };

    class UBoolProperty : public UProperty
    {
    public:
        static uint32 ByteMaskOffset;

        uint8 ByteMask()
        {
            if (SDK::IsValidPointer(this) == false) return 0;
            return *(uint8*)((uintptr_t)this + ByteMaskOffset);
        }
    };

    class UClass : public UStruct
    {
    public:
        static uint32 CastFlagsOffset;
        static uint32 DefaultObjectOffset;

        enum class EClassCastFlags CastFlags()
        {
            if (SDK::IsValidPointer(this) == false) return EClassCastFlags::None;
            return *(EClassCastFlags*)((uintptr_t)this + CastFlagsOffset);
        }

        class UObject* DefaultObject()
        {
            if (SDK::IsValidPointer(this) == false) return nullptr;
            return (UObject*)(*(uintptr_t*)((uintptr_t)this + DefaultObjectOffset));
        }
    };

    class UFunction : public UStruct
    {
    public:
        static uint32 FunctionFlagsOffset;
    };
}