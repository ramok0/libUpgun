#include "../include/libupgun.hpp"

upgun::APlayerController upgun::Game::GetLocalPlayer()
{
	uintptr LocalPlayerPtr = GetWorld().Get(L"OwningGameInstance").GetAs<ue4::TArray<uintptr>>(L"LocalPlayers")[0];
	return ReflectedObject(LocalPlayerPtr).Get(L"PlayerController").Cast<upgun::APlayerController>();
}

void upgun::UMaterialInstanceDynamic::SetTextureParameterValue(ue4::FName ParameterName, void* Value)
{
	upgun::ue4::UMaterialInstanceDynamic_SetTextureParameterValue_Params params = { 0 };
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

void upgun::APlayerController::SendChatMessage(const wchar_t* Message, ue4::FUpGunChatRoomId roomId, bool system)
{
	static UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/UpGun.UpGunBasePlayerController.SendMessage");

	UObject PlayerState = this->Cast<ReflectedObject>().Get(L"PlayerState");
	if (!PlayerState) return;

	ue4::EUpGunChatMessageSenderType senderType = system ? ue4::EUpGunChatMessageSenderType::System : ue4::EUpGunChatMessageSenderType::Player;
	
	ue4::FUpGunChatMessage message;
	ZeroMemory(&message, sizeof(message));

	message.Message = Message;
	message.Room = roomId;
	message.Sender = (ue4::AUpGunBasePlayerState*)PlayerState.get_address();
	message.SenderType = senderType;

	this->ProcessEvent(Function, &message);
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