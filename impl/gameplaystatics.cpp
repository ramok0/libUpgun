#include "../include/libupgun.hpp"
#include "../include/ue4.h"

upgun::ue4::UObject* upgun::ue4::UGameplayStatics::SpawnObject(UObject* ObjectClass, UObject* Outer)
{
	static upgun::UObject Function = Game::GetSingleton().GetObjects().find(L"Function /Script/Engine.GameplayStatics.SpawnObject");

	SpawnObject_Params params;
	params.ObjectClass = ObjectClass;
	params.Outer = Outer;

	upgun::Game::GetSingleton().get_gameplay_statics().ProcessEvent(Function, &params);

	return params.ReturnValue;
}