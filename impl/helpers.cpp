#include "../include/libupgun.hpp"

#include <filesystem>

bool upgun::helpers::IsAttachedToUpGun(void)
{
	HMODULE mainModule = GetModuleHandle(0);

	char name[MAX_PATH];

	if (!GetModuleFileNameA(mainModule, name, sizeof(name))) return false;

	std::filesystem::path module_filename{name};

	return module_filename.filename().string() == "UpGun-Win64-Shipping.exe";
}

bool upgun::helpers::IsConsoleBuilt(void)
{
	return Game::GetSingleton().GetEngine().Get(L"GameViewport").Get(L"ViewportConsole").get_address() != 0;
}

bool upgun::helpers::IsCheatManagerBuilt(void)
{
	return Game::GetSingleton().GetLocalPlayer().Cast<ReflectedObject>().Get(L"CheatManager").get_address() != 0;
}

void upgun::helpers::BuildUE4Console(void)
{
	upgun::ReflectedObject Engine = Game::GetSingleton().GetEngine();

	upgun::UObject Console = upgun::GameplayStatics::SpawnObject(Engine.Get(L"ConsoleClass"), Engine.Get(L"GameViewport"));

	Engine.Get(L"GameViewport").write(L"ViewportConsole", Console.get_address());
}

void upgun::helpers::BuildUE4CheatMananager(void)
{
	upgun::ReflectedObject PlayerController = Game::GetSingleton().GetLocalPlayer().Cast<ReflectedObject>();

	const auto cheatmanagerclass = ue4::UUpGunDeathmatchCheatManager::StaticClass();

	upgun::UObject CheatManager = upgun::GameplayStatics::SpawnObject(*cheatmanagerclass, PlayerController);

	*reinterpret_cast<void**>(PlayerController.get_address() + PlayerController.GetOffset(L"CheatManager")) = (PVOID)CheatManager.get_address();
}
