#pragma once

template <typename T>
__forceinline T call_vfunc(const void* instance, const size_t index)
{
	int* pVTable = *(int**)instance;
	int dwAddress = pVTable[index];
	return (T)(dwAddress);
}