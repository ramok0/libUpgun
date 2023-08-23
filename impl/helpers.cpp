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
