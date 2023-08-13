#include "../include/libupgun.hpp"

const std::wstring upgun::Game::FString::ToString(void)
{
	//this is a shitty implementation
	return std::wstring(Data);
}

const std::wstring upgun::ue4::FName::ToString(void)
{
	Game::FString out;

	const void* FNameToString = Game::GetSingleton().get_fnametostring_ptr();

	if (!FNameToString) return std::wstring();

	const Game::FString* temp = reinterpret_cast<Game::FString * (__fastcall*)(FName*, Game::FString*)>(FNameToString)(this, &out);
	if (!temp) return std::wstring();

	const std::wstring result = out.ToString();

	//Free memory since it will leak otherwise
	Game::GetSingleton().FreeMemory(out.Data);

	return result;
}
