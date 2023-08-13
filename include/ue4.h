#pragma once
#include <string>
#include "types.h"

namespace upgun {
	namespace ue4 {
		struct FName {
			__int32 ComparisonIndex;
			__int32 Number;

			const std::wstring ToString(void);
		};

		struct UObject {
			void** VTable;
			int32 ObjectFlags;
			int32 InternalIndex;
			UObject* ClassPrivate;
			FName NamePrivate;
			UObject* OuterPrivate;
		};

		struct FUObjectItem {
			UObject* Object;
			// Internal flags
			int32_t Flags;
			// UObject Owner Cluster Index
			int32_t ClusterRootIndex;
			// Weak Object Pointer Serial number associated with the object
			int32_t SerialNumber;
		};

		struct TUObjectArray {
			enum
			{
				NumElementsPerChunk = 64 * 1024,
			};

			/** Primary table to chunks of pointers **/
			struct FUObjectItem** Objects;
			/** If requested, a contiguous memory where all objects are allocated **/
			struct FUObjectItem* PreAllocatedObjects;
			/** Maximum number of elements **/
			int32_t MaxElements;
			/** Number of elements we currently have **/
			int32_t NumElements;
			/** Maximum number of chunks **/
			int32_t MaxChunks;
			/** Number of chunks we currently have **/
			int32_t NumChunks;

			ue4::FUObjectItem* GetObjectPtr(int32 Index);
		};
	
		template <typename T>
		struct TArray {
			T* Data;
			int32 Count;
			int32 Max;
		};

		struct FString : TArray<wchar_t> {
		public:
			//default constructor for nullallocated FString
			FString() {
				this->Data = nullptr;
				this->Count = this->Max = 0;
			}

			const std::wstring ToString(void);
		};
	}
}