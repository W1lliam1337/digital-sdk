#pragma once
#include <cstddef>

using MemAllocFailHandler_t = std::size_t(__cdecl*)(std::size_t);

class i_memalloc
{
public:
	virtual void* alloc(std::size_t nSize) = 0;
	virtual void* realloc(void* pMemory, std::size_t nSize) = 0;
	virtual void  free(void* pMemory) = 0;
	virtual void* expand(void* pMemory, std::size_t nSize) = 0;

	virtual void* alloc(std::size_t nSize, const char* szFileName, int nLine) = 0;
	virtual void* realloc(void* pMemory, std::size_t nSize, const char* szFileName, int nLine) = 0;
	virtual void  free(void* pMemory, const char* szFileName, int nLine) = 0;
	virtual void* expand(void* pMemory, std::size_t nSize, const char* szFileName, int nLine) = 0;
	virtual std::size_t get_size(void* pMemory) = 0;
};
