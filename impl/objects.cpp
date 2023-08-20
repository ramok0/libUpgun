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

bool upgun::UObject::IsAInternal(UClass* ClassPrivate)
{
	for (UClass currentClass = this->get_class_private().Cast<UClass>(); currentClass; currentClass = UClass((uintptr)currentClass.get_raw_pointer()->SuperStruct))
	{
		if (currentClass.get_address() == ClassPrivate->get_address())
			return true;
	}

	return false;;
}

void upgun::UObject::ProcessEvent(UObject Function, void* params)
{
	return Game::GetSingleton().ProcessEvent(*this, Function, params);
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

upgun::UObject upgun::ObjectArray::find(const std::wstring objectName, bool bExact)
{
	return this->find([&objectName, bExact](upgun::UObject& obj) {
		std::wstring Name = obj.get_full_name();

		if (bExact) {
			return Name == objectName;
		}
		else {
			return Name.contains(objectName);
		}
		});
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
