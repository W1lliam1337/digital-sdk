#pragma once

template <typename T>
__forceinline T call_vfunc(void* instance, size_t index)
{
	return (*static_cast<T**>(instance))[index];
}