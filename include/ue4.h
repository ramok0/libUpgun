#pragma once
#include <string>
#include "types.h"
#include "native.h"

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

			const std::wstring ToString(void) const;
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

		struct FKey {
			struct FName KeyName; // 0x00(0x08)
			char pad_8[0x10]; // 0x08(0x10)
		};

		//https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/TArrays/
		template <typename T>
		struct TArray {
			T* Data;
			int32 Count;
			int32 Max;

			T operator[](int32 Index)
			{
				T buffer{};

				if (Index >= Count) return buffer;

				buffer = Data[Index];
				return buffer;
			}

			void release()
			{
				Count = Max = 0;
				native::FMemoryFree(this->Data);
				this->Data = nullptr;
			}

			bool contains(T element)
			{
				for (int i = 0; i < Count; i++) {
					if ((*this)[i] == element)
					{
						return true;
					}
				}

				return false;
			}

			void push(T element)
			{
				size_t NewSize = sizeof(T) * (Count+1);

				if (!this->Data)
				{
					Count = Max = 0;
					this->Data = (T*)native::FMemoryMalloc(NewSize, 32);
				}
				else {
					this->Data = (T*)native::FMemoryRealloc(this->Data, NewSize, 32);					
				}

				this->Data[Count] = element;
				this->Count++;
				this->Max++;
			}
		};

		struct FString : TArray<wchar_t> {
		public:
			inline FString() {};

			FString(int i) {

			}

			FString(const wchar_t* other)
			{
				Max = Count = *other ? (int32)std::wcslen(other) + 1 : 0;

				if (Count)
				{
					Data = const_cast<wchar_t*>(other);
				}
			}

			bool operator==(FString other)
			{
				if (Data == other.Data) return true;

				return wcscmp(Data, other.Data) == 0;
			}


			const std::wstring ToString(void);
		};

		struct FText
		{
			char data[0x18];
		};

		struct FUpGunChatRoomId {
			char ID; // 0x00(0x01)
		};

		struct FUpGunChatRoom {
			struct FUpGunChatRoomId ID; // 0x00(0x01)
			char pad_1[0x7]; // 0x01(0x07)
			struct FString Name; // 0x08(0x10)
			struct TArray<struct AUpGunBasePlayerState*> Players; // 0x18(0x10)
		};

		enum class EUpGunChatMessageSenderType : uint8 {
			System = 0,
			Player = 1,
			EUpGunChatMessageSenderType_MAX = 2
		};

		struct FUpGunChatMessage {
			enum class EUpGunChatMessageSenderType SenderType; // 0x00(0x01)
			char pad_1[0x7]; // 0x01(0x07)
			struct AUpGunBasePlayerState* Sender; // 0x08(0x08)
			struct FUpGunChatRoomId Room; // 0x10(0x01)
			char pad_11[0x7]; // 0x11(0x07)
			struct FString Message; // 0x18(0x10)
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
			char pad_1[0x58];

			static UClass* StaticClass();
		};

		struct UFunction : UStruct {
			uint16 FunctionFlags;
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

		struct UKismetSystemLibrary_ExecuteConsoleCommand_Params
		{
			struct UObject* WorldContextObject;                                       // (Parm, ZeroConstructor, IsPlainOldData)
			struct FString                                     Command;                                                  // (Parm, ZeroConstructor)
			struct APlayerController* SpecificPlayer;                                           // (Parm, ZeroConstructor, IsPlainOldData)
		};

		struct UKismetStringLibrary_Conv_StringToName_Params
		{
			FString inString;
			FName ReturnValue;
		};		

		struct AUpGunGameModeBase_KickPlayer_Params
		{
			void* PlayerState;
			struct FText Reason;
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

		struct SpawnObject_Params
		{
			UObject* ObjectClass;
			UObject* Outer;
			UObject* ReturnValue;
		};

		template <typename Key, typename Value>
		struct TMapData {
			Key key;
			Value value;
			uint64_t N0000024C; //0x0010
		};

		template <typename Key, typename Value>
		struct TMap {
		
			TMap() {

			}

			TMap(int i) {

			}

			TArray<TMapData<Key, Value>> Pairs;
			char pad[0x40];

			Value Find(Key in)
			{
				for (int i = 0; i < Pairs.Count; i++)
				{
					auto pair = Pairs[i];

					if (in == pair.key) {
						return pair.value;
					}
				}

				return Value{};
			}
		};

		struct KismetRenderingLibrary {
			static struct UTexture2D* ImportFileAsTexture2D(const std::wstring Filename);

			static UClass* StaticClass();
		};

		struct KismetStringLibrary {
			static FName StringToName(const std::wstring String);

			static UClass* StaticClass();
		};

		struct KismetTextLibrary {
			static FText StringToText(FString inString);
			static FString TextToString(FText inText);

			static UClass* StaticClass();
		};

		struct KismetMaterialLibrary {
			static UMaterialInstanceDynamic* CreateDynamicMaterialInstance(UObject* WorldContextObject, struct UMaterialInterface* Parent, FName OptionalName, EMIDCreationFlags CreationFlags);

			static UClass* StaticClass();
		};

		struct KismetSystemLibrary {
			static void ExecuteConsoleCommand(FString Command);

			static UClass* StaticClass();
		};

		struct UGameplayStatics
		{
			static struct UObject* SpawnObject(struct UObject* ObjectClass, struct UObject* Outer);
		};

		enum class EUpGunCosmeticType : uint8 {
			Unknown = 0,
			Armor = 1,
			Hat = 2,
			Screen = 3,
			Rifle = 4,
			Knife = 5,
			Emote = 6,
			Equipment = 7,
			EUpGunCosmeticType_MAX = 8
		};

		struct FUpGunOSSItemId {
			struct FString ID; // 0x00(0x10)
		};

		struct UUpGunDeathmatchCheatManager
		{
			static UClass* StaticClass();
		};	
		
		struct AUpGunDeathmatchGameMode
		{
			static UClass* StaticClass();
		};

		struct AUpGunGameStateBase
		{
			static UClass* StaticClass();
		};	
			
		struct AGameStateBase
		{
			static UClass* StaticClass();
		};	
		
		struct AUpGunLobbyGameState
		{
			static UClass* StaticClass();
		};	
		
		struct AUpGunDeathmatchSettings
		{
			static UClass* StaticClass();
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

		struct FDataTableRowHandle {
			FDataTableRowHandle() {
				this->DataTable = nullptr;
				this->RowName.ComparisonIndex = 0;
				this->RowName.Number = 0;
			}

			FDataTableRowHandle(FName RowName) {
				this->RowName = RowName;
				this->DataTable = nullptr;
			}


			struct UDataTable* DataTable; // 0x00(0x08)
			FName RowName; // 0x08(0x08)

			bool operator==(FName other)
			{
				return this->RowName.ComparisonIndex == other.ComparisonIndex;
			}

			bool operator==(FDataTableRowHandle other)
			{
				return this->RowName.ComparisonIndex == other.RowName.ComparisonIndex;
			}
		};

		enum class FUpGunDeathmatchGameStateTags {
			Started,
			EnteringMap,
			WaitingToStart,
			Finished,
			WaitingPlayersToJoinTeam,
			PostWaitingPlayersToJoinTeam,
			TeamSelection,
			PickingSkill,
			InProgress,
			RoundFinished,
			UNKNOWN
		};

		FUpGunDeathmatchGameStateTags StringToGameStateTags(const std::wstring in);

		struct AUpGunBasePlayerState {

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
		static_assert(offsetof(UFunction, FunctionFlags) == 0xb0);
	}
}