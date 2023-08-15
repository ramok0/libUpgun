#include "../include/libupgun.hpp"

#include "../lib/memcury/memcury.h"

upgun::UWorld upgun::Game::GetWorld(void)
{
	return upgun::ReflectedObject((uintptr)this->get_engine_ptr()).Get(L"GameViewport").Get(L"World").Cast<UWorld>();
}

void upgun::Game::FreeMemory(void* Address)
{
	if (this->m_Free)
		reinterpret_cast<void(__fastcall*)(void*)>(this->m_Free)(Address);
}

void* upgun::Game::Malloc(size_t size)
{
	return reinterpret_cast<void* (__fastcall*)(size_t, uint32)>(this->m_Malloc)(size, 32);
}

void* upgun::Game::Realloc(void* Block, size_t size)
{
	return reinterpret_cast<void* (__fastcall*)(void*, size_t, uint32)>(this->m_Realloc)(Block, size, 32);
}

void upgun::Game::ProcessEvent(UObject object, UObject function, void* params)
{
	reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(this->m_ProcessEvent)((void*)object.get_address(), (void*)function.get_address(), params);
}

void* upgun::Game::SpawnActor(UWorld World, UClass Class, FTransform const* Transform, const upgun::ue4::FActorSpawnParameters& SpawnParameters)
{
	return reinterpret_cast<void* (__fastcall*)(void*, void*, FTransform const*, const upgun::ue4::FActorSpawnParameters&)>(this->m_SpawnActor)((void*)World.get_address(), (void*)Class.get_address(), Transform, SpawnParameters);
}

void* upgun::Game::StaticConstructObjectInternal(UClass Class, UObject InOuter, void* Name, uint32 SetFlags, uint32 InternalSetFlags, UObject Template, bool bCopyTransientsFromClassDefaults, void* InstanceGraph, bool bAssumeTemplateIsArchetype)
{
	return reinterpret_cast<void* (__fastcall*)(UClass * Class,
		UObject * InOuter,
		void* Name,
		uint32 SetFlags,
		uint32 InternalSetFlags,
		UObject * Template,
		bool  bCopyTransientsFromClassDefaults,
		void* InstanceGraph,
		bool  bAssumeTemplateIsArchetype)>(this->m_SCOI)((upgun::UClass*)Class.get_address(), (upgun::UObject*)InOuter.get_address(), Name, SetFlags, InternalSetFlags, (upgun::UObject*)Template.get_address(), bCopyTransientsFromClassDefaults, InstanceGraph, bAssumeTemplateIsArchetype);
}

const std::wstring upgun::ue4::FName::ToString(void)
{
	ue4::FString out;

	const void* FNameToString = Game::GetSingleton().get_fnametostring_ptr();

	if (!FNameToString) return std::wstring();

	const ue4::FString* temp = reinterpret_cast<ue4::FString * (__fastcall*)(FName*, ue4::FString*)>(FNameToString)(this, &out);
	if (!temp) return std::wstring();

	const std::wstring result = out.ToString();

	//Free memory since it will leak otherwise
	Game::GetSingleton().FreeMemory(out.Data);

	return result;
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
	this->m_ProcessEvent = Memcury::Scanner::FindPattern(patterns::PROCESSEVENT).GetAs<void*>();
	this->m_SpawnActor = Memcury::Scanner::FindPattern(patterns::SPAWNACTOR).GetAs<void*>();
	this->m_SCOI = Memcury::Scanner::FindPattern(patterns::STATICCONSTRUCTOBJECTINTERNAL).GetAs<void*>();
	this->m_Malloc = Memcury::Scanner::FindPattern(patterns::MALLOC).GetAs<void*>();
	this->m_Realloc = Memcury::Scanner::FindPattern(patterns::REALLOC).GetAs<void*>();
}