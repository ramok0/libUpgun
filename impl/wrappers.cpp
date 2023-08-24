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

upgun::UObject upgun::GameplayStatics::SpawnObject(UObject Object, UObject Outer)
{
	ue4::UObject* result = ue4::UGameplayStatics::SpawnObject((ue4::UObject*)Object.get_address(), (ue4::UObject*)Outer.get_address());
	
	return upgun::UObject((uintptr)result);
}

void upgun::APlayerState::KickPlayer(ue4::FString Message)
{
	if (!this->IsA<ue4::AUpGunBasePlayerState>()) {
		std::cout << "This PlayerState is not AUpGunBasePlayerState" << std::endl;
		return;
	}

	upgun::ReflectedObject GameMode = Game::GetSingleton().GetWorld().Get(L"AuthorityGameMode");

	if (!GameMode) {
		std::cout << "Failed to get GameMode" << std::endl;
		return;
	}

	ue4::FText reasontext = upgun::ue4::KismetTextLibrary::StringToText(Message);

	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/UpGun.UpGunGameModeBase.KickPlayer");

	ue4::AUpGunGameModeBase_KickPlayer_Params params;
	ZeroMemory(&params, sizeof(params));

	params.PlayerState = (void*)this->get_address();
	params.Reason = reasontext;

	GameMode.ProcessEvent(Function, &params);
}