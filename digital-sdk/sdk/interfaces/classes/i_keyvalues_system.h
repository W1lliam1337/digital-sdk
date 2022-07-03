#pragma once

#define INVALID_KEY_SYMBOL (-1)
using hkey_symbol = int;


// you can make this shit lower case didn't feel like doing it :)
class i_keyvalues_system
{
public:
	virtual void RegisterSizeofKeyValues(int iSize) = 0;
private:
	virtual void function0() = 0;
public:
	virtual void* AllocKeyValuesMemory(int iSize) = 0;
	virtual void FreeKeyValuesMemory(void* pMemory) = 0;
	virtual hkey_symbol GetSymbolForString(const char* szName, bool bCreate = true) = 0;
	virtual const char* GetStringForSymbol(hkey_symbol hSymbol) = 0;
	virtual void AddKeyValuesToMemoryLeakList(void* pMemory, hkey_symbol hSymbolName) = 0;
	virtual void RemoveKeyValuesFromMemoryLeakList(void* pMemory) = 0;
	virtual void SetKeyValuesExpressionSymbol(const char* szName, bool bValue) = 0;
	virtual bool GetKeyValuesExpressionSymbol(const char* szName) = 0;
	virtual hkey_symbol GetSymbolForStringCaseSensitive(hkey_symbol& hCaseInsensitiveSymbol, const char* szName, bool bCreate = true) = 0;
};

using keyvalues_system_fn = i_keyvalues_system * (__cdecl*)();
