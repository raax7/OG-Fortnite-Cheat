#ifndef COREUOBJECT_H
#define COREUOBJECT_H

#include <Windows.h>
#include "Basic.h"
#include "EngineFixups.h"
#include "../../../Utilitys/Logger.h"

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

namespace SDK {
	class UObject
	{
	public:
		static TUObjectArray						 ObjectArray;
		void*										 Vft;                                               // (0x00[0x08]) NOT AUTO-GENERATED PROPERTY
		int32                                        Flags;                                             // (0x08[0x04]) NOT AUTO-GENERATED PROPERTY
		int32                                        Index;                                             // (0x0C[0x04]) NOT AUTO-GENERATED PROPERTY
		class UClass*								 Class;                                             // (0x10[0x08]) NOT AUTO-GENERATED PROPERTY
		class FName                                  Name;                                              // (0x18[0x08]) NOT AUTO-GENERATED PROPERTY
		class UObject*								 Outer;                                             // (0x20[0x08]) NOT AUTO-GENERATED PROPERTY

		void ProcessEvent(void* fn, void* parms);

		bool IsDefaultObject() const
		{
			return (Flags & 0x10) == 0x10;
		}

		bool HasTypeFlag(EClassCastFlags TypeFlag) const;

		std::string GetName() const;
		std::string GetFullName() const;


		static int32_t GetOffset(const std::string className, const std::string varName);

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


		static class UClass* FindClass(const std::string& ClassFullName)
		{
			return FindObject<class UClass>(ClassFullName, EClassCastFlags::Class);
		}

		static class UClass* FindClassFast(const std::string& ClassName)
		{
			return FindObjectFast<class UClass>(ClassName, EClassCastFlags::Class);
		}


		bool IsA(class UClass* Clss) const;
	};



	class FField {
	public:
		class FField* Next() {
			if (!this) return nullptr;
			return (FField*)(*(uintptr_t*)((uintptr_t)this + 0x20));
		}

		FName Name() {
			if (!this) {
				DEBUG_LOG("BLAH BLAH BLAH");
				return FName{};
			}
			return (FName)(*(uintptr_t*)((uintptr_t)this + 0x28));
		}
	};

	class FProperty : public FField {
	public:
		int32 Offset() {
			if (!this) return 0;
			return *(int32*)((uintptr_t)this + 0x4C);
		}
	};

	class UStruct : public UObject {
	public:
		static int32_t SuperOffset;

		class UStruct* Super() {
			if (!this) return nullptr;
			return (UStruct*)(*(uintptr_t*)((uintptr_t)this + SuperOffset));
		}

		class FField* ChildProperties() {
			if (!this) return nullptr;
			return (FField*)(*(uintptr_t*)((uintptr_t)this + 0x50));
		}
	};

	class UProperty : public UObject
	{
	public:
		static int32_t OffsetOffset;

		int32 Offset() {
			if (!this) return 0;
			return *(int32*)((uintptr_t)this + OffsetOffset);
		}
	};

	class UClass : public UStruct {
	public:
		static int32_t CastFlagsOffset;
		static int32_t DefaultObjectOffset;

		enum class EClassCastFlags CastFlags() {
			if (!this) return EClassCastFlags::None;
			return *(EClassCastFlags*)((uintptr_t)this + CastFlagsOffset);
		}
		class UObject* DefaultObject() {
			if (!this) return nullptr;
			return (UObject*)(*(uintptr_t*)((uintptr_t)this + DefaultObjectOffset));
		}
	};
}

#endif