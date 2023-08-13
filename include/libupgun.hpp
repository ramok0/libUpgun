#pragma once
#include <stdexcept>

#define MISSING_IMPL throw std::runtime_error("This function is not implemented");

namespace upgun {
	namespace patterns {
		constexpr const char* OBJECTS = "48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB"; //TUObjectArray ObjObjects;
		constexpr const char* FNAMETOSTRING = "48 89 5C 24 ? 55 56 57 48 8B EC 48 83 EC 30 8B 01 48 8B F1 44 8B 49 04"; //FName::ToString
		constexpr const char* FREE = "48 85 C9 74 2E 53 48 83 EC 20 48 8B D9 48 8B 0D ? ? ? ? 48 85 C9 75 0C E8 ? ? ? ? 48 8B 0D"; //FMemory::Free
		constexpr const char* ENGINE = "41 B8 01 00 00 00 ? ? ? 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 85 C0"; //we cant do it without gengine but its faster and more reliable (GEngine)
	}

	class Game {
		Game() : 
			m_Objects(nullptr),
			m_FNameToString(nullptr),
			m_Free(nullptr),
			m_Engine(nullptr) {
			
			this->find_patterns();
		}

	private:
		//find addresses for objects, fnametostr, free and engine, throw if it fails
		void find_patterns();
		
		void* m_Objects;
		void* m_FNameToString;
		void* m_Free;
		void* m_Engine;
	};
}