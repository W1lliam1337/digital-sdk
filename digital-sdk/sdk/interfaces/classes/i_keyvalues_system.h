#pragma once

#define INVALID_KEY_SYMBOL (-1)
using hkey_symbol = int;


class i_keyvalues_system
{
public:
	virtual void register_sizeof_keyvalues(int iSize) = 0;
private:
	virtual void function0() = 0;
public:
	virtual void* alloc_keyvalues_memory(int iSize) = 0;
	virtual void free_keyvalues_memory(void* pMemory) = 0;
	virtual hkey_symbol get_symbol_for_string(const char* szName, bool bCreate = true) = 0;
	virtual const char* get_string_for_symbol(hkey_symbol hSymbol) = 0;
	virtual void add_keyvalues_to_memory_leak_list(void* pMemory, hkey_symbol hSymbolName) = 0;
	virtual void remove_keyvalues_from_memory_leak_list(void* pMemory) = 0;
	virtual void set_keyvalues_expression_symbol(const char* szName, bool bValue) = 0;
	virtual bool get_keyvalues_expression_symbol(const char* szName) = 0;
	virtual hkey_symbol get_symbol_for_string_case_sensitive(hkey_symbol& hCaseInsensitiveSymbol, const char* szName, bool bCreate = true) = 0;
};

using keyvalues_system_fn = i_keyvalues_system * (__cdecl*)();
