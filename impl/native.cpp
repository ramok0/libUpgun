#include "../include/libupgun.hpp"
#include "../lib/memcury/memcury.h"

void upgun::Game::FreeMemory(void* Address)
{
	if (this->m_Free)
		reinterpret_cast<void(__fastcall*)(void*)>(this->m_Free)(Address);
}

const upgun::ue4::TUObjectArray* upgun::Game::GetObjectsPtr()
{
	ue4::TUObjectArray* Array = reinterpret_cast<upgun::ue4::TUObjectArray*>(this->m_Objects);

	return Array;
}

void upgun::Game::find_patterns()
{
	this->m_GameEngine = Memcury::Scanner::FindPattern(patterns::ENGINE).AbsoluteOffset(9).RelativeOffset(3).GetAs<void*>();
	this->m_Objects = Memcury::Scanner::FindPattern(patterns::OBJECTS).RelativeOffset(3).GetAs<void*>();
	this->m_Free = Memcury::Scanner::FindPattern(patterns::FREE).GetAs<void*>();
	this->m_FNameToString = Memcury::Scanner::FindPattern(patterns::FNAMETOSTRING).GetAs<void*>();
}