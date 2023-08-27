#include "../include/libupgun.hpp"
#include "../include/native.h"

upgun::ue4::UTexture2D* upgun::ue4::KismetRenderingLibrary::ImportFileAsTexture2D(const std::wstring Filename)
{
	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.KismetRenderingLibrary.ImportFileAsTexture2D", true);

	UKismetRenderingLibrary_ImportFileAsTexture2D_Params params;

	upgun::ReflectedObject World = Game::GetSingleton().GetWorld();

	params.WorldContextObject = reinterpret_cast<upgun::ue4::UObject*>(World.get_address());
	params.Filename = Filename.c_str();
	ue4::KismetRenderingLibrary::StaticClass()->ProcessEvent(Function, &params);

	return (upgun::ue4::UTexture2D*)params.ReturnValue;
}

upgun::ue4::FName upgun::ue4::KismetStringLibrary::StringToName(const std::wstring String)
{
	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.KismetStringLibrary.Conv_StringToName");


	UKismetStringLibrary_Conv_StringToName_Params params = { 0 };

		params.inString = String.c_str();

		KismetStringLibrary::StaticClass()->ProcessEvent(Function, &params);

		return params.ReturnValue;
	
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

	
	KismetMaterialLibrary::StaticClass()->ProcessEvent(Function, &params);

	return params.ReturnValue;
}

upgun::ue4::FText upgun::ue4::KismetTextLibrary::StringToText(FString inString)
{
	upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.KismetTextLibrary.Conv_StringToText");

	struct {
		FString inString;
		FText ReturnValue;
	} params = { 0 };

	params.inString = inString;

	KismetTextLibrary::StaticClass()->ProcessEvent(Function, &params);

	return params.ReturnValue;
}

upgun::ue4::FString upgun::ue4::KismetTextLibrary::TextToString(FText inText)
{
	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.KismetTextLibrary.Conv_TextToString");

	struct {
		FText text;
		FString ReturnValue;
	} params = { 0 };

	ZeroMemory(&params, sizeof(params));

	params.text = inText;

	ue4::KismetTextLibrary::StaticClass()->ProcessEvent(Function, &params);

	return params.ReturnValue;
}

void upgun::ue4::KismetSystemLibrary::ExecuteConsoleCommand(FString Command)
{
	if (!helpers::IsConsoleBuilt())
		helpers::BuildUE4Console();

	if (!helpers::IsCheatManagerBuilt())
		helpers::BuildUE4CheatMananager();

	upgun::UWorld world = Game::GetSingleton().GetWorld();

	static upgun::UObject FunctionObject = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.KismetSystemLibrary.ExecuteConsoleCommand");

	UKismetSystemLibrary_ExecuteConsoleCommand_Params params;
	ZeroMemory(&params, sizeof(params));
	params.WorldContextObject = reinterpret_cast<ue4::UObject*>(world.get_address());
	params.Command = Command;
	params.SpecificPlayer = nullptr;

	KismetSystemLibrary::StaticClass()->ProcessEvent(FunctionObject, &params);
}