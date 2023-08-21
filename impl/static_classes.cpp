#include "../include/libupgun.hpp"

#define GET_STATIC_CLASS(name) static upgun::UClass staticClass = Game::GetSingleton().GetObjects().find(name).Cast<upgun::UClass>()

upgun::UClass* upgun::ue4::UStruct::StaticClass()
{
	GET_STATIC_CLASS(L"Class /Script/CoreUObject.Struct");
	return &staticClass;
}

upgun::UClass* upgun::ue4::UpGunInventorySubsystem::StaticClass()
{
	GET_STATIC_CLASS(L"Class /Script/UpGun.UpGunInventorySubsystem");
	return &staticClass;
}

upgun::UClass* upgun::UClass::StaticClass()
{
	GET_STATIC_CLASS(L"Class /Script/CoreUObject.Class");
	return &staticClass;
}

upgun::UClass* upgun::ue4::KismetStringLibrary::StaticClass()
{
	GET_STATIC_CLASS(L"Class /Script/Engine.KismetStringLibrary");
	return &staticClass;
}

upgun::UClass* upgun::ue4::KismetRenderingLibrary::StaticClass()
{
	GET_STATIC_CLASS(L"Class /Script/Engine.KismetRenderingLibrary");
	return &staticClass;
}

upgun::UClass* upgun::ue4::KismetMaterialLibrary::StaticClass()
{
	GET_STATIC_CLASS(L"Class /Script/Engine.KismetMaterialLibrary");
	return &staticClass;
}

upgun::UClass* upgun::ue4::AUpGunDeathmatchGameMode::StaticClass()
{
	GET_STATIC_CLASS(L"Class /Script/UpGun.UpGunDeathmatchGameMode");
	return &staticClass;
}

upgun::UClass* upgun::ue4::AUpGunBasePlayerState::StaticClass()
{
	GET_STATIC_CLASS(L"Class /Script/UpGun.UpGunBasePlayerState");
	return &staticClass;
}

upgun::UClass* upgun::ue4::AUpGunGameStateBase::StaticClass()
{
	GET_STATIC_CLASS(L"Class /Script/UpGun.UpGunGameStateBase");
	return &staticClass;
}


upgun::UClass* upgun::ue4::AUpGunLobbyGameState::StaticClass()
{
	GET_STATIC_CLASS(L"Class /Script/UpGun.UpGunLobbyGameState");
	return &staticClass;
}

