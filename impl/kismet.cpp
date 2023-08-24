#include "../include/libupgun.hpp"
#include "../include/native.h"

upgun::ue4::UTexture2D* upgun::ue4::KismetRenderingLibrary::ImportFileAsTexture2D(const wchar_t* Filename)
{
	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.KismetRenderingLibrary.ImportFileAsTexture2D", true);

	upgun::UObject library = Game::GetSingleton().get_kismet_rendering_library();

	if (library)
	{
		UKismetRenderingLibrary_ImportFileAsTexture2D_Params params;

		upgun::ReflectedObject World = Game::GetSingleton().GetWorld();
		
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

upgun::UMaterialInstanceDynamic* upgun::ue4::KismetMaterialLibrary::CreateDynamicMaterialInstance(UObject* WorldContextObject, UMaterialInterface* Parent, FName OptionalName, EMIDCreationFlags CreationFlags)
{
	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.PrimitiveComponent.CreateDynamicMaterialInstance");

	UKismetMaterialLibrary_CreateDynamicMaterialInstance_Params params;
	params.CreationFlags = CreationFlags;
	params.OptionalName = OptionalName;
	params.Parent = Parent;
	params.WorldContextObject = (upgun::ue4::UObject*)WorldContextObject;

	upgun::UObject materialLibrary = Game::GetSingleton().get_kismet_material_library();
	materialLibrary.ProcessEvent(Function, &params);

	return params.ReturnValue;
}

upgun::ue4::FText upgun::ue4::KismetTextLibrary::StringToText(const FString inString)
{
	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.KismetTextLibrary.Conv_StringToText");
	upgun::UObject library = Game::GetSingleton().get_kismet_text_library();

	UKismetTextLibrary_Conv_StringToText_Params params;

	params.inString = inString;

	library.ProcessEvent(Function, &params);

	return params.ReturnValue;
}

void upgun::ue4::KismetSystemLibrary::ExecuteConsoleCommand(FString Command)
{
	upgun::UWorld world = Game::GetSingleton().GetWorld();

	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.KismetSystemLibrary.ExecuteConsoleCommand");

	struct {
		upgun::ue4::UObject* World;
		FString* Command;
		upgun::ue4::UObject* PlayerController;
	} params;

	params.World = (upgun::ue4::UObject*)world.get_address();
	params.PlayerController = nullptr;
	params.Command = &Command;

	Game::GetSingleton().get_kismet_system_library().ProcessEvent(Function, &params);
}