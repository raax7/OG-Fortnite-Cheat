#include "CoreUObject_classes.h"

#include "../SDK.h"
#include "../../Game.h"
#include "../../../Utilities/Logger.h"
#include "../../../Utilities/SpoofCall/SpoofCall.h"

namespace SDK {
	TUObjectArray UObject::ObjectArray;

	int32_t UClass::DefaultObjectOffset;
	int32_t UClass::CastFlagsOffset;
	int32_t UStruct::SuperOffset;
	int32_t UStruct::ChildPropertiesOffset;
	int32_t UProperty::OffsetOffset;
	int32_t UFunction::FunctionFlags;



	uint32_t UObject::GetPropertyOffset(UProperty* Property) {
		return Property->Offset();
	}
	uint32_t UObject::GetPropertyOffset(FField* Field, std::string PropertyName) {
		FField* CurrentField = Field;

		do {
			if (CurrentField->Name.ToString() == PropertyName) {
				return reinterpret_cast<FProperty*>(CurrentField)->Offset;
			}

			CurrentField = CurrentField->Next;
		} while (CurrentField);

		return 0;
	}

	void UObject::ProcessEvent(void* FN, void* Params)
	{
		if (this == nullptr || FN == nullptr)
			return;

		if (this->Vft != nullptr && this->Vft[SDK::Cached::VFT::ProcessEvent] != nullptr)
		{
			using ProcessEventParams = void(*)(UObject*, void*, void*);
			auto OriginalProcessEvent = reinterpret_cast<ProcessEventParams>(this->Vft[SDK::Cached::VFT::ProcessEvent]);
			OriginalProcessEvent(this, FN, Params);
			//spoof_call<void>(OriginalProcessEvent, this, FN, Params);
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
#if 0
		for (int i = 0; i < ObjectArray.Num() && (!Functions.empty() || !Offsets.empty()); ++i) {
			UObject* Object = ObjectArray.GetByIndex(i);

			if (!Object)
				continue;

			if (Object->IsDefaultObject() || Object->HasTypeFlag(SDK::EClassCastFlags::ScriptStruct)) {
				SDK::UStruct* ObjectStruct = Object->HasTypeFlag(SDK::EClassCastFlags::ScriptStruct) ? ((SDK::UStruct*)Object) : Object->Class;

				std::string ObjectName = ObjectStruct->Name.ToString();

				for (FunctionSearch& Function : Functions) {
					if (*Function.Function || !ObjectName.ends_with(Function.className))
						continue;

					SDK::UField* child = ObjectStruct->Children;
					while (child) {
						if (child->Name.ToString() == Function.childName) {
							*Function.Function = child;
						}

						child = child->Next;
					}
				}

				for (Offset_& offset : Offsets) {
					if (*offset.pOffset || !ObjectName.ends_with(offset.className))
						continue;

					SDK::FField* child = ObjectStruct->ChildProperties;
					while (child) {
						if (child->Name.ToString() == offset.propertyName) {
							*offset.pOffset = ((SDK::FProperty*)child)->Offset;
							if (offset.pMask && !*offset.pMask) {
								*offset.pMask = ((SDK::FBoolProperty*)child)->FieldMask;
							}
						}

						child = child->Next;
					}
				}

				for (Class_& class_ : Classes) {
					if (!*class_.pAddress && ObjectName == class_.className)
						*class_.pAddress = (uintptr_t)ObjectStruct;
				}
			}
		}
#endif

		DEBUG_LOG(skCrypt("Setting up objects...").decrypt());

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
						// In later UE4 versions, they changed how they manage UProperty variables, so we add "Default__" to the beginning of the class name
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
								++it;
								continue;
							}

							uint32_t Offset = GetPropertyOffset(ChildProperties, CurrentOffset.PropertyName);

							if (Offset) {
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
			std::string OffsetsNotFoundStr = skCrypt("Offsets not found:\n").decrypt();
			for (auto& Offset : Offsets) {
				OffsetsNotFoundStr += Offset.ClassName + skCrypt(".").decrypt() + Offset.PropertyName + skCrypt("\n").decrypt();
			}

			THROW_ERROR(skCrypt("One or more offsets were not found!\n").decrypt() + OffsetsNotFoundStr, false);
		}
		if (!Functions.empty()) {
			std::string FunctionsNotFoundStr = skCrypt("Functions not found:\n").decrypt();
			for (auto& Function : Functions) {
				FunctionsNotFoundStr += Function.ClassName + skCrypt(".").decrypt() + Function.FunctionName + skCrypt("\n").decrypt();
			}

			THROW_ERROR(skCrypt("One or more functions were not found!").decrypt() + FunctionsNotFoundStr, false);
		}

		DEBUG_LOG(skCrypt("Setup objects!").decrypt());
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