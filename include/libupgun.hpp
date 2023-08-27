#pragma once
#include <Windows.h>
#include <stdexcept>
#include <iostream>
#include "types.h"
#include "wrappers.h"

struct UpGunModDescriptor {
	char signature[256];
	int revision;
	char modname[256];
	char author[256];
	char version_firstpart;
	char version_secondpart;
	char version_thirdpart;
	char description[2048];
};

#define STATIC_OBJECT_GETTER(fnName, ptrName, objectName) \
	 UObject fnName() { \
		if(!ptrName)\
		{\
			ptrName = this->GetObjects().find(objectName); \
		}\
		return ptrName; \
	}	

#define CREATE_MOD(iModName, iAuthor, iVersionFirst, iVersionSecond, iVersionThird, iDescription) \
    __declspec(dllexport) UpGunModDescriptor* GetUpGunModDescriptor() { \
        static UpGunModDescriptor mod; \
        mod.revision = 1; \
		mod.version_firstpart = (char)iVersionFirst; \
		mod.version_secondpart = (char)iVersionSecond; \
		mod.version_thirdpart = (char)iVersionThird; \
		strcpy_s(mod.author, sizeof(mod.author), iAuthor); \
		strcpy_s(mod.modname, sizeof(mod.modname), iModName); \
		strcpy_s(mod.signature, sizeof(mod.signature), "upgunmod"); \
		if(iDescription) \
			strcpy_s(mod.description, sizeof(mod.description), iDescription); \
        return &mod; \
    }

namespace upgun {
	namespace helpers {
		bool IsAttachedToUpGun(void);

		bool IsConsoleBuilt(void);
		bool IsCheatManagerBuilt(void);

		void BuildUE4Console(void);
		void BuildUE4CheatMananager(void);
	}

	namespace patterns {
		constexpr const char* OBJECTS = "48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB"; //TUObjectArray ObjObjects;
		constexpr const char* FNAMETOSTRING = "48 89 5C 24 ? 55 56 57 48 8B EC 48 83 EC 30 8B 01 48 8B F1 44 8B 49 04"; //FName::ToString
		constexpr const char* FREE = "48 85 C9 74 2E 53 48 83 EC 20 48 8B D9 48 8B 0D ? ? ? ? 48 85 C9 75 0C E8 ? ? ? ? 48 8B 0D"; //FMemory::Free
		constexpr const char* ENGINE = "41 B8 01 00 00 00 ? ? ? 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 85 C0"; //we can do it without gengine but its faster and more reliable (GEngine)
		constexpr const char* PROCESSEVENT = "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A 99 41 23 D0";
		constexpr const char* SPAWNACTOR = "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 40 49 8B D8 4C 8B E9";
		constexpr const char* MALLOC = "48 89 5C 24 ? 57 48 83 EC 20 48 8B F9 8B DA 48 8B 0D ? ? ? ? 48 85 C9 75 0C E8 ? ? ? ? 48 8B 0D ? ? ? ?";
		constexpr const char* REALLOC = "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F1 41 8B D8 48 8B 0D ? ? ? ? 48 8B FA 48 85 C9 75 0C E8 ? ? ? ? 48 8B 0D ? ? ? ?";
		constexpr const char* STATICCONSTRUCTOBJECTINTERNAL = "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B 39 4C 8D 25 ? ? ? ? 4C 8B 79 08 48 8B D9";
	}

	class Game {
	protected:
		const struct upgun::ue4::TUObjectArray* GetObjectsPtr();

	public:
		Game() {

			this->m_baseAddress = (uintptr)GetModuleHandleA(0);
			this->find_patterns();

			this->m_ObjectArray = ObjectArray(this->GetObjectsPtr());
		}

		static Game GetSingleton()
		{
			static Game instance;
			return instance;
		}

		UWorld GetWorld(void);
		ReflectedObject GetEngine(void);
		APlayerController GetLocalPlayer();

		void FreeMemory(void* Address);

		void* Malloc(size_t size);
		void* Realloc(void* Block, size_t size);

		const void* GetProcessEvent() const {
			return native::ProcessEvent;
		}

		void ProcessEvent(UObject object, UObject function, void* params);

		ObjectArray GetObjects() {
			return this->m_ObjectArray;
		}

		STATIC_OBJECT_GETTER(get_gameplay_statics, this->m_GameplayStatics, L"GameplayStatics /Script/Engine.Default__GameplayStatics");
		/*STATIC_OBJECT_GETTER(get_kismet_system_library, this->m_KismetSystemLibrary, L"KismetSystemLibrary /Script/Engine.Default__KismetSystemLibrary");
		STATIC_OBJECT_GETTER(get_kismet_rendering_library, this->m_KismetRenderingLibrary, L"KismetRenderingLibrary /Script/Engine.Default__KismetRenderingLibrary");
		STATIC_OBJECT_GETTER(get_kismet_string_library, this->m_KismetStringLibrary, L"KismetStringLibrary /Script/Engine.Default__KismetStringLibrary");
		STATIC_OBJECT_GETTER(get_kismet_text_library, this->m_KismetTextLibrary, L"KismetTextLibrary /Script/Engine.Default__KismetTextLibrary");
		STATIC_OBJECT_GETTER(get_kismet_material_library, this->m_KismetMaterialLibrary, L"KismetMaterialLibrary /Script/Engine.Default__KismetMaterialLibrary");*/
		STATIC_OBJECT_GETTER(get_upgun_inventory_subsystem, this->m_UpGunInventorySubsystem, L"UpGunInventorySubsystem /Engine/Transient");
		STATIC_OBJECT_GETTER(get_upgun_cosmetic_subsystem, this->m_UpGunCosmeticSubsystem, L"UpGunCosmeticSubsystem /Engine/Transient");

	private:
		//find addresses for objects, fnametostr, free and engine, throw if it fails
		void find_patterns();

		UObject m_GameplayStatics;
		UObject m_KismetSystemLibrary;
		UObject m_KismetRenderingLibrary;
		UObject m_KismetStringLibrary;
		UObject m_KismetTextLibrary;
		UObject m_KismetMaterialLibrary;
		UObject m_UpGunInventorySubsystem;
		UObject m_UpGunCosmeticSubsystem;
		uintptr m_baseAddress;
		ObjectArray m_ObjectArray;
	};
}