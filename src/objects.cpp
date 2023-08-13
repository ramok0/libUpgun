#include "../include/libupgun.hpp"
#include <sstream>

const std::wstring upgun::Game::UObject::get_name(void)
{
	ue4::FName name = this->get_data().NamePrivate;
	return name.ToString();
}

const std::wstring upgun::Game::UObject::get_full_name(void)
{
	std::wstring out;

	if (this->get_class_private())
		out += this->get_class_private().get_name() + L" ";

	std::wstring temp;
	for (auto p = this->get_outer_private(); p; p = p.get_outer_private())
	{
		temp = p.get_name() + L"." + temp;
	}

	out += temp;
	out += this->get_name();

	return out;
}

upgun::Game::UObject upgun::Game::ObjectArray::GetElement(int32 Index)
{
	const upgun::ue4::TUObjectArray* Array = Game::GetSingleton().GetObjectsPtr();

	upgun::ue4::FUObjectItem* Item = const_cast<upgun::ue4::TUObjectArray*>(Array)->GetObjectPtr(Index);
	if (Item && Item->Object) {
		return UObject((uintptr)Item->Object);
	}

	return UObject(0);
}

upgun::Game::UObject upgun::Game::ObjectArray::find(std::function<bool(UObject&)> pred)
{
	for (int i = 0; i < Game::ObjectArray::Num(); i++)
	{
		upgun::Game::UObject element = Game::ObjectArray::GetElement(i);
		if (!element) continue;

		if (pred(element))
		{
			return element;
		}
	}

	return UObject(0);
}
