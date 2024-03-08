#include "CoreUObject_classes.h"

#include "../SDK.h"
#include "../../Game.h"
#include "../../../Utilities/Logger.h"
#include "../../../Utilities/Memory.h"
#include "../../../Utilities/SpoofCall/SpoofCall.h"

namespace SDK {
	TUObjectArray UObject::ObjectArray;

	uint32 UClass::DefaultObjectOffset;
	uint32 UClass::CastFlagsOffset;
	uint32 UStruct::SuperOffset;
	uint32 UStruct::ChildPropertiesOffset;
	uint32 UStruct::ChildrenOffset;
	uint32 UField::NextOffset;
	uint32 UProperty::OffsetOffset;
	uint32 UBoolProperty::ByteMaskOffset;
	uint32 UFunction::FunctionFlagsOffset;



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
		if (SDK::IsValidPointer(this) == false || SDK::IsValidPointer(FN) == false)
			return;

		if (SDK::IsValidPointer(this->Vft) && SDK::IsValidPointer(this->Vft[SDK::Cached::VFT::ProcessEvent]))
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

	bool FField::HasTypeFlag(EClassCastFlags TypeFlag) const
	{
		if (Class == nullptr)
			return false;

		return TypeFlag != EClassCastFlags::None ? Class->CastFlags & TypeFlag : true;
	}



	void UObject::SetupObjects(std::vector<FunctionSearch>& Functions, std::vector<OffsetSearch>& Offsets) {
		DEBUG_LOG(LOG_OFFSET, skCrypt("Setting up objects...").decrypt());

		std::vector<FunctionSearch> FunctionsNotFound = Functions;
		std::vector<OffsetSearch> OffsetsNotFound = Offsets;



		for (int i = 0; i < ObjectArray.Num() && (Functions.empty() == false || Offsets.empty() == false); i++) {
			UObject* Object = ObjectArray.GetByIndex(i);

			if (!Object)
				continue;

			// In later UE4 versions, they changed how they managed properties
			// so we need to add these addional checks to make sure we only get the default object or ScriptStruct
			if (Game::GameVersion >= 12.00 && Object->IsDefaultObject() == false && Object->HasTypeFlag(SDK::EClassCastFlags::ScriptStruct) == false)
				continue;
			
			SDK::UStruct* ObjectStruct = Object->HasTypeFlag(SDK::EClassCastFlags::ScriptStruct) ? ((SDK::UStruct*)Object) : Object->Class;
			std::string ObjectName = ObjectStruct->Name.ToString();

			for (FunctionSearch& Function : Functions) {
				if (*Function.Function || ObjectName.ends_with(Function.ClassName) == false)
					continue;

				SDK::UField* Children = ObjectStruct->Children();
				while (Children) {
					if (Children->HasTypeFlag(SDK::EClassCastFlags::Function) && Children->Name.ToString() == Function.FunctionName) {
						*Function.Function = Children;

						FunctionsNotFound.erase(std::remove(FunctionsNotFound.begin(), FunctionsNotFound.end(), Function), FunctionsNotFound.end());
					}

					Children = Children->Next();
				}
			}

			for (OffsetSearch& Offset : Offsets) {
				if ((Offset.Offset == nullptr && Offset.Mask == nullptr) || ObjectName.ends_with(Offset.ClassName) == false)
					continue;

				if (Game::GameVersion >= 12.00) {
					SDK::FField* ChildProperty = ObjectStruct->ChildProperties();
					while (ChildProperty) {
						if (ChildProperty->Name.ToString() == Offset.PropertyName) {
							if (Offset.Offset != nullptr && *Offset.Offset == 0x0 && ChildProperty->HasTypeFlag(SDK::EClassCastFlags::Property)) {
								*Offset.Offset = ((SDK::FProperty*)ChildProperty)->Offset;
							}

							if (Offset.Mask != nullptr && *Offset.Mask == 0x0 && ChildProperty->HasTypeFlag(SDK::EClassCastFlags::BoolProperty)) {
								*Offset.Mask = ((SDK::FBoolProperty*)ChildProperty)->FieldMask;
							}

							OffsetsNotFound.erase(std::remove(OffsetsNotFound.begin(), OffsetsNotFound.end(), Offset), OffsetsNotFound.end());
						}

						ChildProperty = ChildProperty->Next;
					}
				}
				else {
					if (ObjectName.ends_with(Offset.ClassName)) {
						SDK::UField* Child = ObjectStruct->Children();
						while (Child) {
							if (Child->Name.ToString() == Offset.PropertyName) {
								if (Offset.Offset != nullptr && *Offset.Offset == 0x0 && Child->HasTypeFlag(SDK::EClassCastFlags::Property)) {
									*Offset.Offset = GetPropertyOffset((SDK::UProperty*)Child);
								}

								if (Offset.Mask != nullptr && *Offset.Mask == 0x0 && Child->HasTypeFlag(SDK::EClassCastFlags::BoolProperty)) {
									*Offset.Mask = ((SDK::UBoolProperty*)Child)->ByteMaskOffset;
								}

								OffsetsNotFound.erase(std::remove(OffsetsNotFound.begin(), OffsetsNotFound.end(), Offset), OffsetsNotFound.end());
							}

							Child = Child->Next();
						}
					}
				}
			}
		}



		if (OffsetsNotFound.empty() == false || FunctionsNotFound.empty() == false) {
			std::string MissingOffsets;
			for (OffsetSearch& Offset : OffsetsNotFound) {
				MissingOffsets += Offset.ClassName + " " + Offset.PropertyName + "\n";
			}

			std::string MissingFunctions;
			for (FunctionSearch& Function : FunctionsNotFound) {
				MissingFunctions += Function.ClassName + " " + Function.FunctionName + "\n";
			}

			std::string Output = skCrypt("").decrypt();

			if (MissingOffsets.empty() == false) {
				Output += skCrypt("Missing offsets: \n").decrypt();
				Output += MissingOffsets;
			}

			if (MissingFunctions.empty() == false) {
				Output += skCrypt("\nMissing functions: \n").decrypt();
				Output += MissingFunctions;
			}
			
			THROW_ERROR(skCrypt("Failed to find all offsets and functions!\n\n").decrypt() + Output, CRASH_ON_NOT_FOUND);
		}

		DEBUG_LOG(LOG_OFFSET, skCrypt("Setup objects!").decrypt());
	}
}