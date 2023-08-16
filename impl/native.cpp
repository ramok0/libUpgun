#include "../include/libupgun.hpp"
#include "../include/native.h"
#include "../lib/memcury/memcury.h"

upgun::UWorld upgun::Game::GetWorld(void)
{
	return GetEngine().Get(L"GameViewport").Get(L"World").Cast<UWorld>();
}

upgun::ReflectedObject upgun::Game::GetEngine(void)
{
	return ReflectedObject((uintptr)*native::Engine);
}

void upgun::Game::FreeMemory(void* Address)
{
	native::FMemoryFree(Address);
	//	reinterpret_cast<void(__fastcall*)(void*)>(this->m_Free)(Address);
}

void* upgun::Game::Malloc(size_t size)
{
	return native::FMemoryMalloc(size, 32);
	//return reinterpret_cast<void* (__fastcall*)(size_t, uint32)>(this->m_Malloc)(size, 32);
}

void* upgun::Game::Realloc(void* Block, size_t size)
{
	return native::FMemoryRealloc(Block, size, 32);
//	return reinterpret_cast<void* (__fastcall*)(void*, size_t, uint32)>(this->m_Realloc)(Block, size, 32);
}

void upgun::Game::ProcessEvent(UObject object, UObject function, void* params)
{
	native::ProcessEvent((void*)object.get_address(), (void*)function.get_address(), params);
	//reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(this->m_ProcessEvent)((void*)object.get_address(), (void*)function.get_address(), params);
}

const std::wstring upgun::ue4::FName::ToString(void)
{
	ue4::FString out;

	//const ue4::FString* temp = reinterpret_cast<ue4::FString * (__fastcall*)(FName*, ue4::FString*)>(FNameToString)(this, &out);
	const ue4::FString* temp = (const ue4::FString*)native::FNameToString(this, &out);
	if (!temp) return std::wstring();

	const std::wstring result = out.ToString();

	//Free memory since it will leak otherwise
	Game::GetSingleton().FreeMemory(out.Data);

	return result;
}

const upgun::ue4::TUObjectArray* upgun::Game::GetObjectsPtr()
{
	ue4::TUObjectArray* Array = reinterpret_cast<upgun::ue4::TUObjectArray*>(native::Objects);

	return Array;
}

void upgun::Game::find_patterns()
{
	native::Engine = Memcury::Scanner::FindPattern(patterns::ENGINE).AbsoluteOffset(9).RelativeOffset(3).GetAs<decltype(native::Engine)>();
	native::Objects = Memcury::Scanner::FindPattern(patterns::OBJECTS).RelativeOffset(3).GetAs<decltype(native::Objects)>();
	native::FMemoryFree = Memcury::Scanner::FindPattern(patterns::FREE).GetAs<decltype(native::FMemoryFree)>();
	native::FNameToString = Memcury::Scanner::FindPattern(patterns::FNAMETOSTRING).GetAs<decltype(native::FNameToString)>();
	native::ProcessEvent = Memcury::Scanner::FindPattern(patterns::PROCESSEVENT).GetAs<decltype(native::ProcessEvent)>();
	native::FMemoryMalloc = Memcury::Scanner::FindPattern(patterns::MALLOC).GetAs<decltype(native::FMemoryMalloc)>();
	native::FMemoryRealloc = Memcury::Scanner::FindPattern(patterns::REALLOC).GetAs<decltype(native::FMemoryRealloc)>();
}