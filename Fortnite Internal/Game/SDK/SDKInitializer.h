#ifndef SDKINITIALIZER_H
#define SDKINITIALIZER_H

#include "SDK.h"
#include "../../Utilitys/Memory.h"

class SDKInitializer {
private:
	/**
	 * @brief Finds the first wildcard (question mark) in a given pattern.
	 *
	 * @param Pattern The pattern to search for wildcard (narrow character)
	 *
	 * @return Returns the index the wildcard was found divided by 3 (2 characters for byte, 1 for space)
	 */
	static const int FindFirstWildCard(const char* Pattern) {
		int Position = 0;
		const char* CurrentChar = Pattern;

		while (*CurrentChar != '\0') {
			if (*CurrentChar == '?') {
				// divide by 3 to account for the 2 characters for one byte + 1 character for one space
				return Position / 3;
			}

			++CurrentChar;
			++Position;
		}

		THROW_ERROR(skCrypt("Failed to extract first wildcard!").decrypt(), true);
	}

	/**
	 * @brief Initializes a VFT (Virtual Function Table) index using a search string and ranged pattern scanning.
	 *
	 * @param VFTName The name of the Virtual Function Table.
	 * @param PossibleSigs A vector of possible signatures for pattern scanning.
	 * @param SearchString The search string (wide character) used to find the address.
	 * @param VFTIndex Reference to the variable that will store the found VFT index.
	 */
	static void InitVFTIndex(const char* VFTName, std::vector<const char*> PossibleSigs, const wchar_t* SearchString, uintptr_t& VFTIndex, int SearchRange);

	/**
	 * @brief Initializes a VFT (Virtual Function Table) index using a search string and ranged pattern scanning.
	 *
	 * @param VFTName The name of the Virtual Function Table.
	 * @param PossibleSigs A vector of possible signatures for pattern scanning.
	 * @param SearchString The search string (wide character) used to find the address.
	 * @param VFTIndex Reference to the variable that will store the found VFT index.
	 */
	static void InitFunctionOffset(const char* FunctionName, std::vector<const char*> PossibleSigs, const wchar_t* SearchString, uintptr_t& FunctionOffset, int SearchRange = 0x600, int SearchBytesBehind = 0x0);

	/**
	 * @brief Initializes a VFT (Virtual Function Table) index using a search string and ranged pattern scanning.
	 *
	 * @param VFTName The name of the Virtual Function Table.
	 * @param PossibleSigs A vector of possible signatures for pattern scanning.
	 * @param SearchString The search string (wide character) used to find the address.
	 * @param VFTIndex Reference to the variable that will store the found VFT index.
	 */
	static void InitFunctionOffset(const char* FunctionName, std::vector<const char*> PossibleSigs, const char* SearchString, uintptr_t& FunctionOffset, int SearchRange = 0x600, int SearchBytesBehind = 0x0);
public:
	static void InitGObjects();
	static void InitAppendString();
	static void InitGetBoneMatrix();
	static void InitPRIndex();
	static void InitPEIndex();
	static void InitGPVIndex();
	static void InitGVIndex();

	static int32_t FindCastFlagsOffset() {
		std::vector<std::pair<void*, SDK::EClassCastFlags>> infos = {
			{ SDK::UObject::FindObjectFast(skCrypt("Actor").decrypt()), SDK::EClassCastFlags::Actor},
			{ SDK::UObject::FindObjectFast(skCrypt("Class").decrypt()), SDK::EClassCastFlags::Field | SDK::EClassCastFlags::Struct | SDK::EClassCastFlags::Class }
		};
		return Memory::FindOffset(infos);
	}
	static int32_t FindDefaultObjectOffset() {
		std::vector<std::pair<void*, void*>> infos = {
			{ SDK::UObject::FindObjectFast(skCrypt("Object").decrypt()), SDK::UObject::FindObjectFast(skCrypt("Default__Object").decrypt()) },
			{ SDK::UObject::FindObjectFast(skCrypt("Field").decrypt()), SDK::UObject::FindObjectFast(skCrypt("Default__Field").decrypt()) }
		};
		return Memory::FindOffset(infos);
	}
	static int32_t FindSuperOffset() {
		std::vector<std::pair<void*, void*>> infos = {
			{ SDK::UObject::FindObjectFast(skCrypt("Struct").decrypt()), SDK::UObject::FindObjectFast(skCrypt("Field").decrypt()) },
			{ SDK::UObject::FindObjectFast(skCrypt("Class").decrypt()), SDK::UObject::FindObjectFast(skCrypt("Struct").decrypt()) }
		};

		// Thanks to the ue4 dev who decided UStruct should be spelled Ustruct
		if (infos[0].first == nullptr) {
			infos[0].first = infos[1].second = SDK::UObject::FindObjectFast(skCrypt("struct").decrypt());
		}

		return Memory::FindOffset(infos);
	}
};

#endif // SDKINITIALIZER_H