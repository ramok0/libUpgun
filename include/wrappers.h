#pragma once
#include <string>
#include <functional>
#include <vector>
#include "types.h"
#include "ue4.h"

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

	private:
		uintptr m_address;
		T data;
	};



	struct UObject : public GameObject<ue4::UObject> {
	public:
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

		void ProcessEvent(UObject Function, void* params) {
			return Game::GetSingleton().ProcessEvent(*this, Function, params);
		}

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

		ReflectedObject Get(const wchar_t* PropertyName)
		{
			if (*this) {
				uint16 Offset = this->GetOffset(PropertyName);
				uintptr Base = this->get_address();

				if (!Offset) {
					return ReflectedObject(0);
				}
				void* Address = *(void**)(Base + Offset);

				return ReflectedObject((uintptr)Address);
			}

			return ReflectedObject(0);
		}

		template <typename T>
		T GetAs(const wchar_t* PropertyName)
		{
			if (*this) {
				uint16 Offset = this->GetOffset(PropertyName);
				uintptr Base = this->get_address();

				if (!Offset) {
					return T(0);
				}
				T* Address = (T*)(Base + Offset);

				std::cout << "0x" << Base << " + " << "0x" << Offset << " = " << Address << std::endl;

				return *Address;
			}

			return T(0);
		}
	};
}