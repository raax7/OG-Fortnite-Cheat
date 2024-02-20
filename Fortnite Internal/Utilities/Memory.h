#ifndef MEMORY_H
#define MEMORY_H

// Huge thanks to Dumper-7 for making this possible
// Without their open source dumper I wouldn't have been able to do this
// Pretty much all of this is pasted from Dumper-7, so make sure to star their repository

#include <Windows.h>
#include <vector>
#include <string>

namespace Memory {
	// The worlds gayest pattern scanner. Improve later
	inline uintptr_t PatternScan(uintptr_t pModuleBaseAddress, const char* sSignature, int pIndex, bool sRelativeAdr = false) {
		static auto patternToByte = [](const char* pattern) { auto bytes = std::vector<int>{}; const auto start = const_cast<char*>(pattern); const auto end = const_cast<char*>(pattern) + strlen(pattern); for (auto current = start; current < end; ++current) { if (*current == '?') { ++current; if (*current == '?') ++current; bytes.push_back(-1); } else bytes.push_back(strtoul((const char*)current, &current, 16)); } return bytes; };

		const auto dosHeader = (PIMAGE_DOS_HEADER)pModuleBaseAddress;
		const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)pModuleBaseAddress + dosHeader->e_lfanew);

		const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = patternToByte(sSignature);
		const auto scanBytes = reinterpret_cast<std::uint8_t*>(pModuleBaseAddress);

		const auto s = patternBytes.size();
		const auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i) {
			bool found = true; for (auto j = 0ul; j < s; ++j) { if (scanBytes[i + j] != d[j] && d[j] != -1) { found = false; break; } }
			if (found)
			{
				if (sRelativeAdr)
				{
					return ((uintptr_t)((UINT_PTR)(reinterpret_cast<uintptr_t>(&scanBytes[i])) + *(PINT)((UINT_PTR)(reinterpret_cast<uintptr_t>(&scanBytes[i])) + ((pIndex)-sizeof(INT))) + (pIndex)));
				}
				else
				{
					return reinterpret_cast<uintptr_t>(&scanBytes[i]);
				}
			}
		}

		return NULL;
	}



	// Everything below is from Dumper-7

	inline bool IsInProcessRange(uintptr_t Address)
	{
		uintptr_t ImageBase = SDK::GetBaseAddress();
		PIMAGE_NT_HEADERS NtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(ImageBase + reinterpret_cast<PIMAGE_DOS_HEADER>(ImageBase)->e_lfanew);

		return Address > ImageBase && Address < (NtHeader->OptionalHeader.SizeOfImage + ImageBase);
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
}

#endif