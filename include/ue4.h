#pragma once
#include <string>
#include "types.h"

namespace upgun {
	struct UClass;

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

			T operator[](int32 Index)
			{
				if (Index >= Count) return T(0);

				return Data[Index];
			}
		};

		struct FString : TArray<wchar_t> {
		public:
			//default constructor for nullallocated FString
			FString() {
				this->Data = nullptr;
				this->Count = this->Max = 0;
			}

			FString(const wchar_t* other)
			{
				Max = Count = *other ? std::wcslen(other) + 1 : 0;

				if (Count)
				{
					Data = const_cast<wchar_t*>(other);
				}
			}

			const std::wstring ToString(void);
		};

		//https://docs.unrealengine.com/4.27/en-US/API/Runtime/CoreUObject/UObject/UField/
		struct UField : public UObject {
			UField* Next;
		};

		//https://docs.unrealengine.com/4.27/en-US/API/Runtime/CoreUObject/UObject/UStruct/
		struct UStruct : UField {
			char pad[0x10];
			UStruct* SuperStruct;
			UField* Children;
			struct FField* ChildProperties;

			static UClass* StaticClass();
		};

		//https://docs.unrealengine.com/4.27/en-US/API/Runtime/CoreUObject/UObject/FField/
		struct FField {
			char pad[0x20];
			FField* Next;
			FName NamePrivate;
			int32 FlagsPrivate;
		};

		struct FProperty : FField {
			char pad_1[0x14];
			uint16 Offset;
		};

		struct UKismetRenderingLibrary_ImportFileAsTexture2D_Params
		{
			UObject* WorldContextObject;
			FString Filename;
			UObject* ReturnValue;
		};

		struct KismetRenderingLibrary {
			static struct UTexture2D* ImportFileAsTexture2D(const wchar_t* Filename);

			static UClass* StaticClass();
		};

		static_assert(offsetof(UStruct, ChildProperties) == 0x50);
		static_assert(offsetof(FField, Next) == 0x20);
		static_assert(offsetof(FField, NamePrivate) == 0x28);
		static_assert(offsetof(FProperty, Offset) == 0x4C);
	}
}