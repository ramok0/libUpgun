#include "../include/libupgun.hpp"

uint16 upgun::ReflectedObject::GetOffset(const std::wstring ClassName, const std::wstring PropertyName)
{
	UClass Class = this->get_class_private().Cast<UClass>();
	uint16 result;
	std::wcout << Class.get_full_name() << std::endl;
	if (result = Class.get_offset(PropertyName)) {
		return result;
	}

	for (UObject outer = this->get_outer_private(); outer; outer = outer.get_outer_private())
	{
		std::wcout << outer.get_full_name() << std::endl;
		if (result = outer.Cast<UClass>().get_offset(PropertyName))
		{
			return result;
		}

		upgun::UClass outerClass = outer.get_class_private().Cast<UClass>();
		std::wcout << outerClass.get_full_name() << std::endl;
		if (result = outerClass.get_offset(PropertyName))
		{
			return result;
		}
	}

	return 0;
}