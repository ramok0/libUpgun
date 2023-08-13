#include "../include/libupgun.hpp"
#include <sstream>

const std::wstring upgun::UObject::get_name(void)
{
	ue4::FName name = this->get_data().NamePrivate;
	return name.ToString();
}

const std::wstring upgun::UObject::get_full_name(void)
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

upgun::UObject upgun::ObjectArray::GetElement(int32 Index)
{
	upgun::ue4::FUObjectItem* Item = GetArray()->GetObjectPtr(Index);
	if (Item && Item->Object) {
		return UObject((uintptr)Item->Object);
	}

	return UObject(0);
}

upgun::UObject upgun::ObjectArray::find(std::function<bool(UObject&)> pred)
{
	for (int i = 0; i < this->Num(); i++)
	{
		upgun::UObject element = this->GetElement(i);
		if (!element) continue;

		if (pred(element))
		{
			return element;
		}
	}

	return UObject(0);
}

std::vector<upgun::UObject> upgun::ObjectArray::find_all(std::function<bool(UObject&)> pred)
{
	std::vector<upgun::UObject> result;

	for (int i = 0; i < this->Num(); i++)
	{
		upgun::UObject element = this->GetElement(i);
		if (!element) continue;

		if (pred(element))
		{
			result.push_back(element);
		}
	}

	return result;
}
