#pragma once
#include <string>
#include "types.h"

namespace upgun {
	namespace ue4 {
		//https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/FName/
		struct FName {
			__int32 ComparisonIndex;
			__int32 Number;

			const std::wstring ToString(void);
		};

		//https://docs.unrealengine.com/4.27/en-US/API/Runtime/CoreUObject/UObject/UObjectBase/
		struct UObject {
			void** VTable;
			int32 ObjectFlags;
			int32 InternalIndex;
			UObject* ClassPrivate;
			FName NamePrivate;
			UObject* OuterPrivate;
		};

		//https://docs.unrealengine.com/4.26/en-US/API/Runtime/CoreUObject/UObject/FUObjectItem/
		struct FUObjectItem {
			UObject* Object;
			// Internal flags
			int32_t Flags;
			// UObject Owner Cluster Index
			int32_t ClusterRootIndex;
			// Weak Object Pointer Serial number associated with the object
			int32_t SerialNumber;
		};

		//https://docs.unrealengine.com/4.26/en-US/API/Runtime/CoreUObject/UObject/FChunkedFixedUObjectArray/
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
	
		//https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/TArrays/
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