#pragma once

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

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
			else if (IsChunked == false && FixedObjects) {
				return FixedObjects->Num();
			}

			return 0;
		}

		UObject* GetByIndex(int32 Index) {
			if (IsChunked && ChunkedObjects) {
				return ChunkedObjects->GetByIndex(Index);
			}
			else if (IsChunked == false && FixedObjects) {
				return FixedObjects->GetByIndex(Index);
			}

			return nullptr;
		}
	};
}