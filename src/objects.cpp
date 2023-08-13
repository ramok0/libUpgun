#include "../include/libupgun.hpp"

uint16 upgun::UClass::get_offset(const std::wstring PropertyName)
{
	auto structData = this->get_raw_pointer();

	for (ue4::FField* field = structData->ChildProperties; field; field = field->Next)
	{
		if (field->NamePrivate.ToString() == PropertyName) {
			return ((ue4::FProperty*)field)->Offset;
		}
	}

	return 0;
}

upgun::UClass* upgun::UClass::StaticClass()
{
	static UClass staticClass = Game::GetSingleton().GetObjects().find([](upgun::UObject& object)
		{
			return object.get_full_name() == L"Class /Script/CoreUObject.Class";
		}).Cast<UClass>();

		return &staticClass;
}


bool upgun::UObject::IsAInternal(UClass* ClassPrivate)
{
	return (UClass*)this->get_class_private().get_address() == ClassPrivate;
}

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
	upgun::ue4::TUObjectArray* Array = GetArray();
	if (!Array) return UObject(0);
	upgun::ue4::FUObjectItem* Item = Array->GetObjectPtr(Index);
	if (Item && Item->Object) {
		return UObject((uintptr)Item->Object);
	}

	return UObject(0);
}

upgun::UObject upgun::ObjectArray::find(std::function<bool(UObject&)> pred)
{
	upgun::ue4::TUObjectArray* Array = GetArray();
	if (!Array) return UObject(0);
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
	upgun::ue4::TUObjectArray* Array = GetArray();
	std::vector<upgun::UObject> result;
	if (!Array) return result;
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
