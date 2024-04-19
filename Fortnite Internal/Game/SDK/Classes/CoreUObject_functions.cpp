#include "CoreUObject_classes.h"

#include "../SDK.h"
#include "../../Game.h"

#include "../../../Utilities/Logger.h"
#include "../../../Utilities/Error.h"

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
		if (SDK::IsValidPointer(this) == false || SDK::IsValidPointer(FN) == false || SDK::IsValidPointer(Params) == false)
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
		DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("Setting up objects...")));

		std::vector<FunctionSearch> FunctionsNotFound = Functions;
		std::vector<OffsetSearch> OffsetsNotFound = Offsets;

		for (int i = 0; i < ObjectArray.Num() && (Functions.empty() == false || Offsets.empty() == false); i++) {
			UObject* Object = ObjectArray.GetByIndex(i);

			if (Object == nullptr)
				continue;

			// Skip default objects and non ScriptStructs
			bool IsScriptStruct = Object->HasTypeFlag(SDK::EClassCastFlags::ScriptStruct);
			if (Object->IsDefaultObject() == false && IsScriptStruct == false)
				continue;
			
			// Get the object's name and ScriptStruct
			SDK::UStruct* ObjectStruct = IsScriptStruct ? (SDK::UStruct*)Object : Object->Class;
			SDK::FName ObjectName = ObjectStruct->Name;

			// Search for functions and offsets in versions below 12.00
			SDK::UField* Children = ObjectStruct->Children();
			while (Children) {
				// Search for functions
				if (Children->HasTypeFlag(SDK::EClassCastFlags::Function)) {
					for (FunctionSearch& Function : Functions) {
						if (*Function.Function || ObjectName != Function.ClassName)
							continue;

						if (Children->Name == Function.FunctionName) {
							*Function.Function = Children;

							DEBUG_LOG(LOG_INFO, std::string(skCrypt("Found function: ")) + Function.ClassName.ToString() + "::" + Function.FunctionName.ToString() + " at " + std::to_string((uintptr_t)*Function.Function));

							FunctionsNotFound.erase(std::remove(FunctionsNotFound.begin(), FunctionsNotFound.end(), Function), FunctionsNotFound.end());
						}
					}
				}

				bool IsProperty = Children->HasTypeFlag(SDK::EClassCastFlags::Property);
				bool IsBoolProperty = Children->HasTypeFlag(SDK::EClassCastFlags::BoolProperty);
				if (IsProperty || IsBoolProperty) {
					for (OffsetSearch& Offset : Offsets) {
						bool HasOffset = Offset.Offset != nullptr && (*Offset.Offset == -0x1 || *Offset.Offset == 0x0);
						bool HasMask = Offset.Mask != nullptr && (*Offset.Mask == -0x1 || *Offset.Mask == 0x0);

						if ((HasOffset == false && HasMask == false) || ObjectName != Offset.ClassName)
							continue;

						if (Children->Name == Offset.PropertyName) {
							if (HasOffset && IsProperty) {
								*Offset.Offset = GetPropertyOffset((SDK::UProperty*)Children);

								DEBUG_LOG(LOG_INFO, std::string(skCrypt("Found offset: ")) + Offset.ClassName.ToString() + "::" + Offset.PropertyName.ToString() + " at " + std::to_string(*Offset.Offset));
							}

							if (HasMask && IsBoolProperty) {
								*Offset.Mask = ((SDK::UBoolProperty*)Children)->ByteMask();

								DEBUG_LOG(LOG_INFO, std::string(skCrypt("Found mask: ")) + Offset.ClassName.ToString() + "::" + Offset.PropertyName.ToString() + " at " + std::to_string(*Offset.Mask));
							}

							OffsetsNotFound.erase(std::remove(OffsetsNotFound.begin(), OffsetsNotFound.end(), Offset), OffsetsNotFound.end());
						}
					}
				}

				Children = Children->Next();
			}

			// Search for offsets in versions above 12.00
			if (Game::GameVersion >= 12.00) {
				SDK::FField* ChildProperty = ObjectStruct->ChildProperties();
				while (ChildProperty) {
					bool IsProperty = ChildProperty->HasTypeFlag(SDK::EClassCastFlags::Property);
					bool IsBoolProperty = ChildProperty->HasTypeFlag(SDK::EClassCastFlags::BoolProperty);

					if (IsProperty || IsBoolProperty) {
						for (OffsetSearch& Offset : Offsets) {
							bool HasOffset = Offset.Offset != nullptr && (*Offset.Offset == -0x1 || *Offset.Offset == 0x0);
							bool HasMask = Offset.Mask != nullptr && (*Offset.Mask == -0x1 || *Offset.Mask == 0x0);

							if ((HasOffset == false && HasMask == false) || ObjectName != Offset.ClassName)
								continue;

							if (ChildProperty->Name == Offset.PropertyName) {
								if (HasOffset && IsProperty) {
									*Offset.Offset = ((SDK::FProperty*)ChildProperty)->Offset;

									DEBUG_LOG(LOG_INFO, std::string(skCrypt("Found offset: ")) + Offset.ClassName.ToString() + "::" + Offset.PropertyName.ToString() + " at " + std::to_string(*Offset.Offset));
								}

								if (HasMask && IsBoolProperty) {
									*Offset.Mask = ((SDK::FBoolProperty*)ChildProperty)->FieldMask;

									DEBUG_LOG(LOG_INFO, std::string(skCrypt("Found mask: ")) + Offset.ClassName.ToString() + "::" + Offset.PropertyName.ToString() + " at " + std::to_string(*Offset.Mask));
								}

								OffsetsNotFound.erase(std::remove(OffsetsNotFound.begin(), OffsetsNotFound.end(), Offset), OffsetsNotFound.end());
							}
						}
					}

					ChildProperty = ChildProperty->Next;
				}
			}
		}



		if (OffsetsNotFound.empty() == false || FunctionsNotFound.empty() == false) {
			std::string MissingOffsets;
			for (OffsetSearch& Offset : OffsetsNotFound) {
				MissingOffsets += Offset.ClassName.ToString() + " " + Offset.PropertyName.ToString() + "\n";
			}

			std::string MissingFunctions;
			for (FunctionSearch& Function : FunctionsNotFound) {
				MissingFunctions += Function.ClassName.ToString() + " " + Function.FunctionName.ToString() + "\n";
			}

			std::string Output = "";

			if (MissingOffsets.empty() == false) {
				Output += std::string(skCrypt("Missing offsets: \n"));
				Output += MissingOffsets;
			}

			if (MissingFunctions.empty() == false) {
				Output += std::string(skCrypt("\nMissing functions: \n"));
				Output += MissingFunctions;
			}
			
			THROW_ERROR(std::string(skCrypt("Failed to find all offsets and functions!\n\n")) + Output, false);
		}

		DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("Setup objects!")));
	}
}