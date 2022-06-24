#pragma once

template <typename T>
T call_vfunc(void* instance, size_t index)
{
	return (*static_cast<T**>(instance))[index];
}