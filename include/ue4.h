#pragma once
#include <string>
#include "types.h"

namespace upgun {
	struct UClass;
	struct UMaterialInstanceDynamic;

	namespace ue4 {

		enum class EMIDCreationFlags : uint8 {
			None = 0,
			Transient = 1,
			EMIDCreationFlags_MAX = 2
		};

		//https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/FName/
		struct FName {
			__int32 ComparisonIndex;
			__int32 Number;

			static FName Empty() {
				return { 0, 0 };
			}

			operator bool() {
				return this->ComparisonIndex != 0;
			}

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
				Max = Count = *other ? (int32)std::wcslen(other) + 1 : 0;

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

		struct UKismetStringLibrary_Conv_StringToName_Params
		{
			FString inString;
			FName ReturnValue;
		};		
		
		struct UKismetMaterialLibrary_CreateDynamicMaterialInstance_Params
		{
			UObject* WorldContextObject;
			struct UMaterialInterface* Parent;
			FName OptionalName;
			EMIDCreationFlags CreationFlags;
			UMaterialInstanceDynamic* ReturnValue;
		};

		struct UMaterialInstanceDynamic_SetTextureParameterValue_Params
		{
			FName ParameterName;
			UObject* Value;
		};

		struct KismetRenderingLibrary {
			static struct UTexture2D* ImportFileAsTexture2D(const wchar_t* Filename);

			static UClass* StaticClass();
		};

		struct KismetStringLibrary {
			static FName StringToName(const wchar_t* String);

			static UClass* StaticClass();
		};

		struct KismetMaterialLibrary {
			static UMaterialInstanceDynamic* CreateDynamicMaterialInstance(UObject* WorldContextObject, struct UMaterialInterface* Parent, FName OptionalName, EMIDCreationFlags CreationFlags);

			static UClass* StaticClass();
		};

		struct FUpGunOSSItemId {
			struct FString ID; // 0x00(0x10)
		};

		struct FUpGunInventoryItem {
			struct FUpGunOSSItemId InstanceID; // 0x00(0x10)
			struct FUpGunOSSItemId ItemId; // 0x10(0x10)
			int32_t Quantity; // 0x20(0x04)
			char Flags; // 0x24(0x01)
			char pad_25[0x3]; // 0x25(0x03)
		};

		struct UpGunInventorySubsystem {
			static TArray<FUpGunInventoryItem> GetItems(void);

			static UClass* StaticClass();
		};

		enum class ESpawnActorCollisionHandlingMethod : uint8
		{
			/** Fall back to default settings. */
			Undefined,								
			/** Actor will spawn in desired location, regardless of collisions. */
			AlwaysSpawn,								
			/** Actor will try to find a nearby non-colliding location (based on shape components), but will always spawn even if one cannot be found. */
			AdjustIfPossibleButAlwaysSpawn,
			/** Actor will try to find a nearby non-colliding location (based on shape components), but will NOT spawn unless one is found. */
			AdjustIfPossibleButDontSpawnIfColliding,
			/** Actor will fail to spawn. */
			DontSpawnIfColliding,
		};

		enum class ESpawnActorScaleMethod : uint8
		{
			/** Ignore the default scale in the actor's root component and hard-set it to the value of SpawnTransform Parameter */
			OverrideRootScale,
			/** Multiply value of the SpawnTransform Parameter with the default scale in the actor's root component */
			MultiplyWithRoot,
			SelectDefaultAtRuntime,
		};


		struct FActorSpawnParameters {
			FName Name;

			/* An Actor to use as a template when spawning the new Actor. The spawned Actor will be initialized using the property values of the template Actor. If left NULL the class default object (CDO) will be used to initialize the spawned Actor. */
			UObject* Template;

			/* The Actor that spawned this Actor. (Can be left as NULL). */
			UObject* Owner;

			/* The APawn that is responsible for damage done by the spawned Actor. (Can be left as NULL). */
			UObject* Instigator;

			/* The ULevel to spawn the Actor in, i.e. the Outer of the Actor. If left as NULL the Outer of the Owner is used. If the Owner is NULL the persistent level is used. */
			UObject* OverrideLevel;

			UObject* OverrideParentComponent;

			ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride;

			/** Determines whether to multiply or override root component with provided spawn transform */
			ESpawnActorScaleMethod TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		};

		static_assert(offsetof(UStruct, ChildProperties) == 0x50);
		static_assert(offsetof(FField, Next) == 0x20);
		static_assert(offsetof(FField, NamePrivate) == 0x28);
		static_assert(offsetof(FProperty, Offset) == 0x4C);

	}
}