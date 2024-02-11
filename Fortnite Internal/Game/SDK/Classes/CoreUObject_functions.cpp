#include "../SDK.h"
#include "CoreUObject_classes.h"
#include "../../../Utilities/Logger.h"

namespace SDK {
	TUObjectArray UObject::ObjectArray;

	int32_t UClass::DefaultObjectOffset;
	int32_t UClass::CastFlagsOffset;
	int32_t UStruct::SuperOffset;
	int32_t UProperty::OffsetOffset;



	int32_t UObject::GetPropertyOffset(UProperty* Property) {
		return Property->Offset();
	}
	int32_t UObject::GetPropertyOffset(FField* Field, std::string PropertyName) {
		FField* CurrentField = Field;

		do {
			if (CurrentField->Name.ToString() == PropertyName) {
				return reinterpret_cast<FProperty*>(CurrentField)->Offset;
			}

			CurrentField = CurrentField->Next;
		} while (CurrentField);

		return 0;
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

	void UObject::SetupObjects(std::vector<FunctionSearch>& Functions, std::vector<OffsetSearch>& Offsets) {
		for (int i = 0; i < ObjectArray.Num() && (!Functions.empty() || !Offsets.empty()); ++i) {
			UObject* Object = ObjectArray.GetByIndex(i);

			if (!Object)
				continue;

			std::string ObjectName = Object->GetName();

			if (ObjectName.empty())
				continue;

			if (!Functions.empty()) {
				if (Object->HasTypeFlag(EClassCastFlags::Function)) {
					for (auto it = Functions.begin(); it != Functions.end(); ) {
						FunctionSearch& CurrentFunction = *it;

						if (CurrentFunction.FunctionName == ObjectName) {

							std::string combinedString = CurrentFunction.ClassName + "." + CurrentFunction.FunctionName;

							std::string FullObjectName = Object->GetFullName();

							if (FullObjectName.length() >= combinedString.length() &&
								FullObjectName.substr(FullObjectName.length() - combinedString.length()) == combinedString) {
								*CurrentFunction.Function = Object;
								it = Functions.erase(it);
							}
							else {
								++it;
							}
						}
						else {
							++it;
						}
					}
				}
			}

			if (!Offsets.empty()) {
				for (auto it = Offsets.begin(); it != Offsets.end();) {
					OffsetSearch& CurrentOffset = *it;

					if (Game::GameVersion >= 12.00) {
						// In later UE4 versions, they changed how they manage child variables, so we add "Default__" to the beginning of the class name
						// to find the default object of the class. There is no default object for structs, so we just use the class name
						std::string DefaultName;
						if (CurrentOffset.Type == OffsetType::Class) {
							DefaultName = skCrypt("Default__").decrypt() + CurrentOffset.ClassName;
						}
						else {
							DefaultName = CurrentOffset.ClassName;
						}

						if (ObjectName == DefaultName) {
							FField* ChildProperties = Object->Class->ChildProperties();

							if (!ChildProperties) {
								continue;
							}

							int32 Offset = GetPropertyOffset(ChildProperties, CurrentOffset.PropertyName);

							if (Offset) {
								DEBUG_LOG("Found property: " + CurrentOffset.PropertyName + " - " + std::to_string(Offset) + " - " + ObjectName);
								*CurrentOffset.Offset = Offset;
								it = Offsets.erase(it);
							}
							else {
								++it;
							}
						}
						else {
							++it;
						}
					}
					else {
						if (CurrentOffset.PropertyName == ObjectName) {
							std::string combinedString = CurrentOffset.ClassName + "." + CurrentOffset.PropertyName;

							std::string FullObjectName = Object->GetFullName();

							if (FullObjectName.length() >= combinedString.length() &&
								FullObjectName.substr(FullObjectName.length() - combinedString.length()) == combinedString) {
								UProperty* Property = reinterpret_cast<SDK::UProperty*>(Object);
								*CurrentOffset.Offset = GetPropertyOffset(Property);
								it = Offsets.erase(it);
							}
							else {
								++it;
							}
						}
						else {
							++it;
						}
					}
				}
			}
		}

		if (!Offsets.empty()) {
			std::string OffsetsNotFoundStr = skCrypt("Offsets not found: ").decrypt();
			for (auto& Offset : Offsets) {
				OffsetsNotFoundStr += Offset.ClassName + skCrypt(".").decrypt() + Offset.PropertyName + skCrypt("\n").decrypt();
			}

			THROW_ERROR(skCrypt("One or more offsets were not found!\n").decrypt() + OffsetsNotFoundStr, false);
		}
		if (!Functions.empty()) {
			std::string FunctionsNotFoundStr = skCrypt("Functions not found: ").decrypt();
			for (auto& Function : Functions) {
				FunctionsNotFoundStr += Function.ClassName + skCrypt(".").decrypt() + Function.FunctionName + skCrypt("\n").decrypt();
			}

			THROW_ERROR(skCrypt("One or more functions were not found!").decrypt() + FunctionsNotFoundStr, false);
		}
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