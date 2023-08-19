#include "../include/libupgun.hpp"

upgun::APlayerController upgun::Game::GetLocalPlayer()
{
	uintptr LocalPlayerPtr = GetWorld().Get(L"OwningGameInstance").GetAs<ue4::TArray<uintptr>>(L"LocalPlayers")[0];
	return ReflectedObject(LocalPlayerPtr).Get(L"PlayerController").Cast<upgun::APlayerController>();
}

void upgun::UMaterialInstanceDynamic::SetTextureParameterValue(ue4::FName ParameterName, void* Value)
{
	upgun::ue4::UMaterialInstanceDynamic_SetTextureParameterValue_Params params;
	params.ParameterName = ParameterName;
	params.Value = (upgun::ue4::UObject*)Value;

	static UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.MaterialInstanceDynamic.SetTextureParameterValue");

	this->ProcessEvent(Function, &params);
}

FTransform upgun::AActor::GetTransform()
{
	static UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.Actor.GetTransform");

	FTransform out;

	this->ProcessEvent(Function, &out);

	return out;
}

void upgun::APlayerController::SetMouseSensitivity(float MouseSensitivity)
{
	static UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/UpGun.UpGunBasePlayerController.SetMouseSensitivity");

	this->ProcessEvent(Function, &MouseSensitivity);
}
