#ifndef ENGINE_FIXUPS_H
#define ENGINE_FIXUPS_H

#include "CoreUObject_classes.h"

namespace SDK {
	// Forward Declarations
	class TUObjectArray;
	class Chunked_TUObjectArray;
	class Fixed_TUObjectArray;
	struct FUObjectItem;



	struct FUObjectItem
	{
		class UObject* Object;
		uint8 Pad_0[0x10];

	};

	class Chunked_TUObjectArray
	{
	public:
		enum
		{
			ElementsPerChunk = 0x10000,
		};

	public:
		static inline auto DecryptPtr = [](void* ObjPtr) -> uint8*
			{
				return reinterpret_cast<uint8*>(ObjPtr);
			};

		FUObjectItem** Objects;
		uint8 Pad_0[0x08];
		int32 MaxElements;
		int32 NumElements;
		int32 MaxChunks;
		int32 NumChunks;


	public:
		inline int32 Num() const
		{
			return NumElements;
		}

		inline FUObjectItem** GetDecrytedObjPtr() const
		{
			return reinterpret_cast<FUObjectItem**>(DecryptPtr(Objects));
		}

		inline class UObject* GetByIndex(const int32 Index) const
		{
			if (Index < 0 || Index > NumElements)
				return nullptr;

			const int32 ChunkIndex = Index / ElementsPerChunk;
			const int32 InChunkIdx = Index % ElementsPerChunk;

			return GetDecrytedObjPtr()[ChunkIndex][InChunkIdx].Object;
		}
	};
	class Fixed_TUObjectArray
	{
	public:
		static inline auto DecryptPtr = [](void* ObjPtr) -> uint8*
			{
				return reinterpret_cast<uint8*>(ObjPtr);
			};

	public:
		FUObjectItem* Objects;
		int32 MaxElements;
		int32 NumElements;

	public:
		inline int Num() const
		{
			return NumElements;
		}

		inline FUObjectItem* GetDecrytedObjPtr() const
		{
			return reinterpret_cast<FUObjectItem*>(DecryptPtr(Objects));
		}

		inline class UObject* GetByIndex(const int32 Index) const
		{
			if (Index < 0 || Index > NumElements)
				return nullptr;

			return GetDecrytedObjPtr()[Index].Object;
		}
	};

	class TUObjectArray {
	public:
		bool IsChunked;
		Chunked_TUObjectArray* ChunkedObjects;
		Fixed_TUObjectArray* FixedObjects;

		int32 Num() {
			if (IsChunked && ChunkedObjects) {
				return ChunkedObjects->Num();
			}
			else if (!IsChunked && FixedObjects) {
				return FixedObjects->Num();
			}

			return 0;
		}

		UObject* GetByIndex(int32 Index) {
			if (IsChunked && ChunkedObjects) {
				return ChunkedObjects->GetByIndex(Index);
			}
			else if (!IsChunked && FixedObjects) {
				return FixedObjects->GetByIndex(Index);
			}

			return nullptr;
		}
	};
}

#endif