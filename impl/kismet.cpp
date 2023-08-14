#include "../include/libupgun.hpp"

upgun::ue4::UTexture2D* upgun::ue4::KismetRenderingLibrary::ImportFileAsTexture2D(const wchar_t* Filename)
{
	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.KismetRenderingLibrary.ImportFileAsTexture2D", true);

	upgun::UObject library = Game::GetSingleton().get_kismet_rendering_library();

	if (library)
	{
		UKismetRenderingLibrary_ImportFileAsTexture2D_Params params;

		const void* Engine = Game::GetSingleton().get_engine_ptr();
		upgun::ReflectedObject World = upgun::ReflectedObject((uintptr)Engine).Get(L"GameViewport").Get(L"World");
		
		params.WorldContextObject = reinterpret_cast<upgun::ue4::UObject*>(World.get_address());
		params.Filename = Filename;
		library.ProcessEvent(Function, &params);

		return (upgun::ue4::UTexture2D*)params.ReturnValue;
	}
	
	return nullptr;
}

upgun::ue4::FName upgun::ue4::KismetStringLibrary::StringToName(const wchar_t* String)
{
	upgun::UObject library = Game::GetSingleton().get_kismet_string_library();

	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.KismetStringLibrary.Conv_StringToName");

	if (library)
	{
		UKismetStringLibrary_Conv_StringToName_Params params;

		params.inString = String;

		library.ProcessEvent(Function, &params);

		return params.ReturnValue;
	}

	return FName::Empty();
}