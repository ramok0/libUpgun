#include "../include/libupgun.hpp"

uint16 upgun::ReflectedObject::GetOffset(const std::wstring PropertyName)
{
	UClass Class = this->get_class_private().Cast<UClass>();
	uint16 result;
	if (result = Class.get_offset(PropertyName)) {
		return result;
	}
	UClass super;
	while (super = Class.get_super_struct())
	{
		if (super) {
			if (result = super.get_offset(PropertyName)) {
				return result;
			}
		}

		Class = super;
	}

	return 0;
}