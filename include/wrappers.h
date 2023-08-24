#pragma once
#include <string>
#include <functional>
#include <vector>
#include "types.h"
#include "ue4.h"
#include "uemath.h"


namespace upgun {
	template <typename T>
	struct GameObject {
	public:
		GameObject(uintptr address) {
			this->m_address = address;
			if (address && !IsBadReadPtr((const void*)address, sizeof(T)))
			{
				this->data = *reinterpret_cast<T*>(this->get_address());
			}
		}

		const uintptr get_address() { return this->m_address; };

		const T get_data() { return this->data; };

		operator bool() {
			return this->get_address() != 0;
		}

		bool operator==(GameObject& other)
		{
			return this->get_address() == other.get_address();
		}

		T* operator->() {
			return &this->data;
		}

	private:
		uintptr m_address;
		T data;
	};



	struct UObject : public GameObject<ue4::UObject> {
	public:
		UObject() : GameObject(0) { };

		UObject(uintptr address) : GameObject(address) { };

		UObject get_class_private() {
			if (!*this || !this->get_data().ClassPrivate) return UObject(0);
			return UObject((uintptr)this->get_data().ClassPrivate);
		}

		UObject get_outer_private() {
			if (!*this || !this->get_data().OuterPrivate) return UObject(0);
			return UObject((uintptr)this->get_data().OuterPrivate);
		}

		bool IsAInternal(struct UClass* ClassPrivate);

		template <typename T>
		bool IsA()
		{
			return IsAInternal(T::StaticClass());
		}

		template <typename T>
		T Cast() {
			return T(this->get_address());
		}

		void ProcessEvent(UObject Function, void* params);

		const std::wstring get_name(void);
		const std::wstring get_full_name(void);
	};

	struct UClass : public UObject {
	public:
		UClass() : UObject(0) {};

		UClass(uintptr address) : UObject(address) { };

		upgun::ue4::UStruct* get_raw_pointer() {
			return reinterpret_cast<upgun::ue4::UStruct*>(this->get_address());
		}

		UClass get_super_struct() {
			return get_raw_pointer()->SuperStruct ? UClass((uintptr)get_raw_pointer()->SuperStruct) : UClass(0);
		}

		uint16 get_offset(const std::wstring PropertyName);

		static UClass* StaticClass();
	};

	struct FUObjectItem : public GameObject<ue4::FUObjectItem> {
	public:
		FUObjectItem(uintptr address) : GameObject(address) {};

		const UObject get_object() {
			return UObject((uintptr)this->get_data().Object);
		}
	};

	struct APlayerState : public UObject {
	public:
		APlayerState(uintptr address) : UObject(address) { };

		void KickPlayer(ue4::FString Message);
	};

	struct AActor : public UObject {
	public:
		AActor(uintptr address) : UObject(address) { };

		FTransform GetTransform();
	};

	struct APlayerController : public AActor {
	public:
		APlayerController(uintptr address) : AActor(address) {};

		void SetMouseSensitivity(float MouseSensitivity);
	};

	struct ObjectArray {
	public:
		ObjectArray() {
			this->m_ObjectArray = nullptr;
		}

		ObjectArray(const upgun::ue4::TUObjectArray* ObjectArray)
		{
			this->m_ObjectArray = const_cast<upgun::ue4::TUObjectArray*>(ObjectArray);
		}

		const int32 Num() {
			upgun::ue4::TUObjectArray* Array = GetArray();
			if (!Array) return 0;
			return Array->NumElements;
		}

		UObject GetElement(int32 Index);

		UObject find(const std::wstring objectName, bool bExact = false);
		UObject find(std::function<bool(UObject&)> pred);
		std::vector<UObject> find_all(std::function<bool(UObject&)> pred);

		upgun::ue4::TUObjectArray* GetArray() {
			return this->m_ObjectArray;
		}

	private:
		upgun::ue4::TUObjectArray* m_ObjectArray;
	};

	class ReflectedObject : public UObject {
	public:
		ReflectedObject(uintptr address) : UObject(address) { };

		uint16 GetOffset(const std::wstring PropertyName);

		template <typename T>
		T* GetPointer(const wchar_t* PropertyName)
		{
			if (*this) {
				uint16 Offset = this->GetOffset(PropertyName);
				uintptr Base = this->get_address();

				if (!Offset) {
					return nullptr;
				}

				return reinterpret_cast<T*>(Base + Offset);
			}

			return nullptr;
		}

		ReflectedObject Get(const wchar_t* PropertyName)
		{
			uintptr* Address;
			if (!(Address = this->GetPointer<uintptr>(PropertyName)))
			{
				return ReflectedObject(0);
			}

			return ReflectedObject(*Address);
		}

		template <typename T>
		bool write(const wchar_t* PropertyName, T buffer) {
			T* Address = this->GetPointer<T>(PropertyName);
			if (!Address) return false;

			*Address = buffer;

			return true;
		}

		template <typename T>
		T GetAs(const wchar_t* PropertyName)
		{
			T* Address = this->GetPointer<T>(PropertyName);

			if (!Address) return T{ 0 };

			return *Address;
		}
	};

	struct UWorld : public ReflectedObject {
		UWorld(uintptr address) : ReflectedObject(address) { };

		UObject SpawnActor(UClass Class, FTransform* Transform, const ue4::FActorSpawnParameters& SpawnParameters);
	};

	struct GameplayStatics {
		static UObject SpawnObject(UObject Object, UObject Outer);
	};

	struct UMaterialInstanceDynamic : UObject {
	public:
		UMaterialInstanceDynamic(uintptr address) : UObject(address) { };

		void SetTextureParameterValue(ue4::FName ParameterName, void* Value);
	};
}