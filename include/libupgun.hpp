#pragma once
#include <Windows.h>
#include <stdexcept>
#include <iostream>
#include "types.h"
#include "wrappers.h"

#define STATIC_OBJECT_GETTER(fnName, ptrName, objectName) \
	const UObject fnName() { \
		if(!ptrName)\
		{\
			ptrName = this->GetObjects().find(objectName); \
		}\
		return ptrName; \
	}	

namespace upgun {
	namespace patterns {
		constexpr const char* OBJECTS = "48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB"; //TUObjectArray ObjObjects;
		constexpr const char* FNAMETOSTRING = "48 89 5C 24 ? 55 56 57 48 8B EC 48 83 EC 30 8B 01 48 8B F1 44 8B 49 04"; //FName::ToString
		constexpr const char* FREE = "48 85 C9 74 2E 53 48 83 EC 20 48 8B D9 48 8B 0D ? ? ? ? 48 85 C9 75 0C E8 ? ? ? ? 48 8B 0D"; //FMemory::Free
		constexpr const char* ENGINE = "41 B8 01 00 00 00 ? ? ? 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 85 C0"; //we can do it without gengine but its faster and more reliable (GEngine)
		constexpr const char* PROCESSEVENT = "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A 99 41 23 D0";
	}

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

			this->m_ObjectArray = ObjectArray(this->GetObjectsPtr());
		}

		static Game GetSingleton()
		{
			static Game instance;
			return instance;
		}

		void FreeMemory(void* Address);

		void ProcessEvent(UObject object, UObject function, void* params);

		ObjectArray GetObjects() {
			return this->m_ObjectArray;
		}

		const void* get_fnametostring_ptr() { return this->m_FNameToString; };
		const void* get_engine_ptr() { return *(void**)this->m_GameEngine; };

		STATIC_OBJECT_GETTER(get_kismet_rendering_library, this->m_KismetRenderingLibrary, L"KismetRenderingLibrary /Script/Engine.Default__KismetRenderingLibrary");
		STATIC_OBJECT_GETTER(get_kismet_string_library, this->m_KismetStringLibrary, L"KismetStringLibrary /Script/Engine.Default__KismetStringLibrary");
		STATIC_OBJECT_GETTER(get_kismet_material_library, this->m_KismetMaterialLibrary, L"KismetMaterialLibrary /Script/Engine.Default__KismetMaterialLibrary");
		STATIC_OBJECT_GETTER(get_upgun_inventory_subsystem, this->m_UpGunInventorySubsystem, L"UpGunInventorySubsystem /Engine/Transient");

	private:
		//find addresses for objects, fnametostr, free and engine, throw if it fails
		void find_patterns();

		void* m_Objects;
		void* m_FNameToString;
		void* m_Free;
		void* m_GameEngine;
		void* m_ProcessEvent;
		UObject m_KismetRenderingLibrary;
		UObject m_KismetStringLibrary;
		UObject m_KismetMaterialLibrary;
		UObject m_UpGunInventorySubsystem;
		uintptr m_baseAddress;
		ObjectArray m_ObjectArray;
	};
}