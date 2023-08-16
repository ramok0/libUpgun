#pragma once
#include "types.h"

namespace upgun {
	namespace native {
		inline void(__fastcall* FMemoryFree)(void*);
		inline void* (__fastcall* FMemoryMalloc)(size_t, int32);
		inline void* (__fastcall* FMemoryRealloc)(void*, size_t, int32);
		inline void* (__fastcall* FNameToString)(void*, void*);
		inline void (__fastcall* ProcessEvent)(void*, void*, void*);
		inline void* Objects;
		inline void** Engine;
	}
}