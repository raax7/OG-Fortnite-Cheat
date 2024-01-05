#include "../SDK.h"
#include "CoreUObject_classes.h"
#include "../../../Utilitys/Logger.h"

namespace SDK {
	TUObjectArray UObject::ObjectArray;

	int32_t UClass::DefaultObjectOffset;
	int32_t UClass::CastFlagsOffset;

	int32_t UStruct::SuperOffset;

	int32_t UProperty::OffsetOffset;



	int32_t UObject::GetOffset(const std::string className, const std::string varName) {
		// OFFSET FINDING TYPE 1
		std::string combinedString = className + "." + varName;

		for (int i = 0; i < ObjectArray.Num(); ++i)
		{
			UObject* Object = ObjectArray.GetByIndex(i);

			if (!Object)
				continue;

			std::string objName = Object->GetFullName();

			if (objName.length() < combinedString.length())
				continue;

			if (objName.substr(objName.length() - combinedString.length()) == combinedString) {
				UProperty* property = reinterpret_cast<SDK::UProperty*>(Object);
				return property->Offset();
			}
		}

		return 0;





		/*SDK::UClass* Class = reinterpret_cast<SDK::UClass*>(SDK::UObject::FindObject("Class Engine.Engine"));
		DEBUG_LOG("Class Offset: " + std::to_string(*reinterpret_cast<uintptr_t*>(Class) - SDK::BaseAddress));
		DEBUG_LOG("Class Name: " + Class->GetFullName());

		FField* CurrentProperty = Class->ChildProperties();
		if (!CurrentProperty) {
			DEBUG_LOG("!!! NO CHILD PROPERTIES !!!");
			return NULL;
		}

		DEBUG_LOG("CurrentProperty First Offset: " + std::to_string(*reinterpret_cast<uintptr_t*>(CurrentProperty) - SDK::BaseAddress));

		int PropertyCount = 0;

		for (CurrentProperty;
			!IsBadReadPtr((LPVOID)CurrentProperty,(UINT_PTR)8);
			CurrentProperty = CurrentProperty->Next()) {
			DEBUG_LOG("CurrentProperty Offset: " + std::to_string(*reinterpret_cast<uintptr_t*>(CurrentProperty) - SDK::BaseAddress));

			//FName Name = CurrentProperty->Name();
			//if (!Name || IsBadReadPtr((LPVOID)Name, (UINT_PTR)8)) {
			//	DEBUG_LOG("!!! NAME IS BAD POINTER !!!");
			//	continue;
			//}

			DEBUG_LOG("Defined FName");
			//DEBUG_LOG("ComparisonIndex: " + Name.ComparisonIndex);
			//DEBUG_LOG("Number: " + Name.Number);
			//DEBUG_LOG("Name String: " + Name.ToString());

			PropertyCount++;
		}

		DEBUG_LOG("Property Count: " + std::to_string(PropertyCount));

		return NULL; //Return NULL if checked all properties of the class.





		// OFFSET FINDING TYPE 2
		for (int i = 0; i < ObjectArray.Num(); ++i)
		{
			UObject* Object = ObjectArray.GetByIndex(i);

			if (!Object)
				continue;

			std::string objName = Object->GetName();

			bool found = false;
			if (objName.find("Engine") != std::string::npos) {
				found = true;
				DEBUG_LOG("found - " + objName);
			}

			if (objName != className) {
				if (found) {
					DEBUG_LOG("found but not it - " + objName);
				}

				continue;
			}

			return NULL; //Return NULL if checked all properties of the class.





			UClass* ObjecAsClass = reinterpret_cast<UClass*>(Object);

			FField* ChildProperties = ObjecAsClass->ChildProperties();
			if (!ChildProperties) DEBUG_LOG("no child prop");
			if (ChildProperties) DEBUG_LOG("yes child prop - " + std::to_string(reinterpret_cast<uintptr_t>(ChildProperties)));
			FField* Property = ChildProperties->Next();
			if (!Property) DEBUG_LOG("no Property");
			if (Property) DEBUG_LOG("yes Property - " + std::to_string(reinterpret_cast<uintptr_t>(ChildProperties)));

			while (true) {
				DEBUG_LOG("Property!!: " + std::to_string(reinterpret_cast<uintptr_t>(Property)));
				DEBUG_LOG("calling name");
				FName name = Property->Name();
				DEBUG_LOG("called name");
				DEBUG_LOG("prop: " + name.ToString());

				if (Property->Name().ToString() == varName) {
					DEBUG_LOG("equels: " + Property->Name().ToString());
					return reinterpret_cast<FProperty*>(Property)->Offset();
				}

				Property = Property->Next();
				if (!Property) {
					DEBUG_LOG("no prop, leaving: " + std::to_string(reinterpret_cast<uintptr_t>(Property)));
					break;
				}

				DEBUG_LOG("");
			}

			DEBUG_LOG("left!!");
		}*/
	}

	void UObject::ProcessEvent(void* fn, void* parms)
	{
		if (this == nullptr || fn == nullptr)
			return;

		auto vtable = *reinterpret_cast<void***>(this);
		if (vtable != nullptr && vtable[SDK::Cached::VFT::ProcessEvent] != nullptr)
		{
			reinterpret_cast<void(*)(void*, void*, void*)>(vtable[SDK::Cached::VFT::ProcessEvent])(this, fn, parms);
		}
	}

	bool UObject::HasTypeFlag(EClassCastFlags TypeFlag) const
	{
		return TypeFlag != EClassCastFlags::None ? Class->CastFlags() & TypeFlag : true;
	}

	std::string UObject::GetName() const
	{
		return this ? Name.ToString() : "None";
	}
	std::string UObject::GetFullName() const
	{
		if (Class)
		{
			std::string Temp;

			for (UObject* NextOuter = Outer; NextOuter; NextOuter = NextOuter->Outer)
			{
				Temp = NextOuter->GetName() + "." + Temp;
			}

			std::string Name = Class->GetName();
			Name += " ";
			Name += Temp;
			Name += GetName();

			return Name;
		}

		return "None";
	}

	bool UObject::IsA(class UClass* Clss) const
	{
		for (UStruct* Super = Class; Super; Super = Super->Super())
		{
			if (Super == Clss)
			{
				return true;
			}
		}

		return false;
	}
}