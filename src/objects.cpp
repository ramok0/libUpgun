#include "../include/libupgun.hpp"

const std::wstring upgun::Game::UObject::get_name(void)
{
	ue4::FName name = this->get_data().NamePrivate;
	return name.ToString();
}

const upgun::Game::UObject upgun::Game::ObjectArray::GetElement(int32 Index)
{
	const upgun::ue4::TUObjectArray* Array = Game::GetSingleton().GetObjectsPtr();

	upgun::ue4::FUObjectItem* Item = const_cast<upgun::ue4::TUObjectArray*>(Array)->GetObjectPtr(Index);
	if (Item && Item->Object) {
		return UObject((uintptr)Item->Object);
	}

	return UObject(0);
}