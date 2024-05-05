#pragma once

#include <Windows.h>
#include <vector>
#include <string>

#include "../External-Libs/LazyImporter.h"

#include "../Utilities/Logger.h"

// Huge thanks to Dumper-7 for making this possible
// Without their open source dumper I wouldn't have been able to do this
// Pretty much all of this is pasted from Dumper-7, so make sure to check out his amazing work
//
// Later down the line, I will probably recode this in my own way, but for now, it's not a very big priority
// since this works perfectly fine

namespace Memory {
	// The worlds gayest pattern scanner. Improve later
	/*
	* @brief Scans for a pattern in a module
	* 
	* @param ModuleBaseAddress - The base address of the module to scan
	* @param Signature - The pattern to scan for (example: "48 8B 05 ? ? ? ? 48 8B 0C C8")
	* @param PointerIndex - The index in the signature to return
	* @param RelativeAddress - If the address should be relative
	* 
	* @return The address of the pattern
	*/
	inline uintptr_t PatternScan(uintptr_t ModuleBaseAddress, const char* Signature, int PointerIndex = 0, bool RelativeAddress = false) {
		static auto patternToByte = [](const char* pattern) { auto bytes = std::vector<int>{}; const auto start = const_cast<char*>(pattern); const auto end = const_cast<char*>(pattern) + strlen(pattern); for (auto current = start; current < end; ++current) { if (*current == '?') { ++current; if (*current == '?') ++current; bytes.push_back(-1); } else bytes.push_back(strtoul((const char*)current, &current, 16)); } return bytes; };

		const auto DOSHeader = (PIMAGE_DOS_HEADER)ModuleBaseAddress;
		const auto NtHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)ModuleBaseAddress + DOSHeader->e_lfanew);

		const auto SizeOfImage = NtHeaders->OptionalHeader.SizeOfImage;
		auto PatternBytes = patternToByte(Signature);
		const auto ScanBytes = reinterpret_cast<std::uint8_t*>(ModuleBaseAddress);

		const auto Size = PatternBytes.size();
		const auto Data = PatternBytes.data();

		for (auto i = 0ul; i < SizeOfImage - Size; ++i) {
			bool Found = true; for (auto j = 0ul; j < Size; ++j) { if (ScanBytes[i + j] != Data[j] && Data[j] != -1) { Found = false; break; } }

			if (Found)
			{
				if (RelativeAddress)
				{
					return ((uintptr_t)((UINT_PTR)(reinterpret_cast<uintptr_t>(&ScanBytes[i])) + *(PINT)((UINT_PTR)(reinterpret_cast<uintptr_t>(&ScanBytes[i])) + ((PointerIndex)-sizeof(INT))) + (PointerIndex)));
				}
				else
				{
					return reinterpret_cast<uintptr_t>(&ScanBytes[i]);
				}
			}
		}

		return NULL;
	}



	// thanks dumper-7.
	// 
	// ive been doing this project for like 4 months now and i just want
	// to get it done. so yes, this is all pasted from dumper-7 for now.
	// i will recode this all as my own later on.

	inline bool IsInProcessRange(uintptr_t Address) {
		uintptr_t ImageBase = SDK::GetBaseAddress();
		PIMAGE_NT_HEADERS NtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(ImageBase + reinterpret_cast<PIMAGE_DOS_HEADER>(ImageBase)->e_lfanew);

		return Address > ImageBase && Address < (NtHeader->OptionalHeader.SizeOfImage + ImageBase);
	}

	template<typename StringType>
	size_t GetStringLen(StringType str) {
		if constexpr (std::is_same<StringType, const char*>::value) {
			return std::strlen(str);
		}
		else if constexpr (std::is_same<StringType, const wchar_t*>::value) {
			return std::wcslen(str);
		}
	}

	template<typename StringType>
	int CompareStrings(StringType str1, StringType str2, size_t len) {
		if constexpr (std::is_same<StringType, const char*>::value) {
			return std::strncmp(str1, str2, len);
		}
		else if constexpr (std::is_same<StringType, const wchar_t*>::value) {
			return std::wcsncmp(str1, str2, len);
		}
	}

	static inline void* FindPatternInRange(std::vector<int>&& Signature, uint8_t* Start, uintptr_t Range, bool bRelative = false, uint32_t Offset = 0, int SkipCount = 0)
	{
		const auto PatternLength = Signature.size();
		const auto PatternBytes = Signature.data();

		for (int i = 0; i < (Range - PatternLength); i++)
		{
			bool bFound = true;
			int CurrentSkips = 0;

			for (auto j = 0ul; j < PatternLength; ++j)
			{
				if (Start[i + j] != PatternBytes[j] && PatternBytes[j] != -1)
				{
					bFound = false;
					break;
				}
			}
			if (bFound)
			{
				if (CurrentSkips != SkipCount)
				{
					CurrentSkips++;
					continue;
				}

				uintptr_t Address = uintptr_t(Start + i);
				if (bRelative)
				{
					if (Offset == -1)
						Offset = (uint32_t)PatternLength;

					Address = ((Address + Offset + 4) + *(int32_t*)(Address + Offset));
				}
				return (void*)Address;
			}
		}

		return nullptr;
	}
	static inline void* FindPatternInRange(const char* Signature, uint8_t* Start, uintptr_t Range, bool bRelative = false, uint32_t Offset = 0)
	{
		static auto patternToByte = [](const char* pattern) -> std::vector<int>
			{
				auto Bytes = std::vector<int>{};
				const auto Start = const_cast<char*>(pattern);
				const auto End = const_cast<char*>(pattern) + strlen(pattern);

				for (auto Current = Start; Current < End; ++Current)
				{
					if (*Current == '?')
					{
						++Current;
						if (*Current == '?') ++Current;
						Bytes.push_back(-1);
					}
					else { Bytes.push_back(strtoul(Current, &Current, 16)); }
				}
				return Bytes;
			};

		return FindPatternInRange(patternToByte(Signature), Start, Range, bRelative, Offset);
	}
	inline void* RelativePattern(uint8_t* Address, const char* Pattern, int32_t Range, int32_t Relative = 0)
	{
		if (!Address)
			return nullptr;

		return FindPatternInRange(Pattern, Address, Range, Relative != 0, Relative);
	}

	template<typename Type = const char*>
	inline uint8_t* FindByStringInAllSections(Type RefStr)
	{
		uintptr_t ImageBase = SDK::GetBaseAddress();
		PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)(ImageBase);
		PIMAGE_NT_HEADERS NtHeader = (PIMAGE_NT_HEADERS)(ImageBase + DosHeader->e_lfanew);

		const DWORD SizeOfImage = NtHeader->OptionalHeader.SizeOfImage;

		uint8_t* SearchStart = (uint8_t*)ImageBase;
		DWORD SearchRange = SizeOfImage;

		for (int i = 0; i < (int)SearchRange; i++)
		{
			if ((SearchStart[i] == uint8_t(0x4C) || SearchStart[i] == uint8_t(0x48)) && SearchStart[i + 1] == uint8_t(0x8D))
			{
				const uint8_t* StrPtr = *(int32_t*)(SearchStart + i + 3) + 7 + SearchStart + i;

				if (!IsInProcessRange((uintptr_t)StrPtr))
					continue;

				if constexpr (std::is_same<Type, const char*>())
				{
					if (strcmp((const char*)RefStr, (const char*)StrPtr) == 0)
					{
						return { SearchStart + i };
					}
				}
				else
				{
					auto a = std::wstring((const wchar_t*)StrPtr);

					if (wcscmp((const wchar_t*)RefStr, (const wchar_t*)StrPtr) == 0)
					{
						return { SearchStart + i };
					}
				}
			}
		}

		return nullptr;
	}

	inline uint8_t* FindByStringInAllSections_constchar(const char* RefStr, uint8_t* SearchStart, DWORD SearchRange) {
		for (int i = 0; i < (int)SearchRange; i++)
		{
			if ((SearchStart[i] == uint8_t(0x4C) || SearchStart[i] == uint8_t(0x48)) && SearchStart[i + 1] == uint8_t(0x8D))
			{
				const uint8_t* StrPtr = *(int32_t*)(SearchStart + i + 3) + 7 + SearchStart + i;

				if (!IsInProcessRange((uintptr_t)StrPtr))
					continue;

				if (strcmp(RefStr, (const char*)StrPtr) == 0)
				{
					return { SearchStart + i };
				}
			}
		}

		return nullptr;
	}

	template<int Alignement = 4, typename T>
	inline int32_t FindOffset(std::vector<std::pair<void*, T>>& ObjectValuePair, int MinOffset = 0x28, int MaxOffset = 0x1A0)
	{
		int32_t HighestFoundOffset = MinOffset;

		for (int i = 0; i < ObjectValuePair.size(); i++)
		{
			uint8_t* BytePtr = (uint8_t*)(ObjectValuePair[i].first);

			for (int j = HighestFoundOffset; j < MaxOffset; j += Alignement)
			{
				if ((*(T*)(BytePtr + j)) == ObjectValuePair[i].second && j >= HighestFoundOffset)
				{
					if (j > HighestFoundOffset)
					{
						HighestFoundOffset = j;
						i = 0;
					}
					j = MaxOffset;
				}
			}
		}
		return HighestFoundOffset;
	}

	static bool IsBadReadPtr(void* p)
	{
		MEMORY_BASIC_INFORMATION mbi;

		if (LI_FN(VirtualQuery).safe()(p, &mbi, sizeof(mbi)))
		{
			constexpr DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
			bool b = !(mbi.Protect & mask);
			if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
				b = true;

			return b;
		}

		return true;
	};

	template<bool bCheckForVft = true>
	inline int32_t GetValidPointerOffset(uint8_t* ObjA, uint8_t* ObjB, int32_t StartingOffset, int32_t MaxOffset)
	{
		if (IsBadReadPtr(ObjA) || IsBadReadPtr(ObjB))
			return -1;

		for (int j = StartingOffset; j <= MaxOffset; j += 0x8)
		{
			const bool bIsAValid = !IsBadReadPtr(*reinterpret_cast<void**>(ObjA + j)) && (bCheckForVft ? !IsBadReadPtr(**reinterpret_cast<void***>(ObjA + j)) : true);
			const bool bIsBValid = !IsBadReadPtr(*reinterpret_cast<void**>(ObjB + j)) && (bCheckForVft ? !IsBadReadPtr(**reinterpret_cast<void***>(ObjB + j)) : true);

			if (bIsAValid && bIsBValid)
				return j;
		}

		return -1;
	};

	inline bool IsFunctionRet(uint8_t* Address)
	{
		int Align = 0x10 - (uintptr_t(Address) % 0x10);
		//if (Opcode == RET && (OpcodeBefore is a POP opcode || OpcodeTwoBefore is a different POP Opcode)
		return Address[0] == 0xC3 && Address[Align] == 0x40 && ((Address[-1] >= 0x58 && Address[-1] <= 0x5F) || (Address[-2] == 0x41 && (Address[-1] >= 0x58 && Address[-1] <= 0x5F)));
	}

	inline uint8_t* FindFunctionEnd(uint8_t* Address)
	{
		if (!Address)
			return nullptr;

		int Align = 0x10 - (uintptr_t(Address) % 0x10);

		for (int i = 0; i < 0xFFFF; i++)
		{
			if (IsFunctionRet(Address + i))
			{
				return Address + i;
			}
			if ((uintptr_t(Address + i) % 0x10 == 0) && (Address[i] == 0x40 && (Address[i + 1] >= 0x50 && Address[i + 1] <= 0x57) && (Address[i + 2] >= 0x50 && Address[i + 2] <= 0x57)))
			{
				return Address + i;
			}
		}

		return nullptr;
	}

	inline uintptr_t FindNextFunctionStart(uint8_t* Address)
	{
		if (!Address)
			return 0x0;

		uintptr_t FuncEnd = (uintptr_t)FindFunctionEnd(Address);

		return FuncEnd % 0x10 != 0 ? FuncEnd + (0x10 - (FuncEnd % 0x10)) : FuncEnd;
	}

	inline uintptr_t ResolveRelativeAddress(uintptr_t Address, int Offset)
	{
		return Address + Offset + 4 + *(int32_t*)(Address + Offset);
	}
}