#pragma once
#include <Windows.h>
#include <stdexcept>
#include <iostream>
#include "types.h"
#include "ue4.h"
#include <functional>

namespace upgun {
	namespace patterns {
		constexpr const char* OBJECTS = "48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB"; //TUObjectArray ObjObjects;
		constexpr const char* FNAMETOSTRING = "48 89 5C 24 ? 55 56 57 48 8B EC 48 83 EC 30 8B 01 48 8B F1 44 8B 49 04"; //FName::ToString
		constexpr const char* FREE = "48 85 C9 74 2E 53 48 83 EC 20 48 8B D9 48 8B 0D ? ? ? ? 48 85 C9 75 0C E8 ? ? ? ? 48 8B 0D"; //FMemory::Free
		constexpr const char* ENGINE = "41 B8 01 00 00 00 ? ? ? 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 85 C0"; //we cant do it without gengine but its faster and more reliable (GEngine)
	}

	template <typename T>
	struct GameObject {
	public:
		GameObject(uintptr address) {
			this->m_address = address;
			if (address)
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

		const std::wstring get_name(void);
		const std::wstring get_full_name(void);
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
		ObjectArray(const upgun::ue4::TUObjectArray* ObjectArray)
		{
			this->m_ObjectArray = const_cast<upgun::ue4::TUObjectArray*>(ObjectArray);
		}

		const int32 Num() {
			return GetArray()->NumElements;
		}

		UObject GetElement(int32 Index);

		UObject find(std::function<bool(UObject&)> pred);
		std::vector<UObject> find_all(std::function<bool(UObject&)> pred);

		upgun::ue4::TUObjectArray* GetArray() {
			return this->m_ObjectArray;
		}

	private:
		upgun::ue4::TUObjectArray* m_ObjectArray;
	};

	class Game {
	protected:
		const struct upgun::ue4::TUObjectArray* GetObjectsPtr();

	public:
		Game() : 
			m_Objects(nullptr),
			m_FNameToString(nullptr),
			m_Free(nullptr),
			m_GameEngine(nullptr) {
			
			this->m_baseAddress = (uintptr)GetModuleHandleA(0);

			this->find_patterns();
		}

		static Game GetSingleton()
		{
			static Game instance;
			return instance;
		}

		void FreeMemory(void* Address);

		ObjectArray GetObjects() {
			return ObjectArray(this->GetObjectsPtr());
		}

		const void* get_fnametostring_ptr() { return this->m_FNameToString; };
		const void* get_engine_ptr() { return this->m_GameEngine; };

	private:
		//find addresses for objects, fnametostr, free and engine, throw if it fails
		void find_patterns();
		
		void* m_Objects;
		void* m_FNameToString;
		void* m_Free;
		void* m_GameEngine;
		uintptr m_baseAddress;
	};
}