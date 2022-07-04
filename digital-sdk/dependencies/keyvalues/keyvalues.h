#pragma once
#include <cstddef>
#include <cstdint>
#include "../../sdk/interfaces/classes/i_keyvalues_system.h"

using get_symbol_proc_fn = bool(__cdecl*)(const char*);

class c_keyvalues
{
public:
	enum EKeyType : int
	{
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_COMPILED_INT_BYTE,
		TYPE_COMPILED_INT_0,
		TYPE_COMPILED_INT_1,
		TYPE_NUMTYPES
	};

	c_keyvalues(const char* szKeyName, void* pUnknown1 = nullptr, hkey_symbol hCaseInsensitiveKeyName = INVALID_KEY_SYMBOL);
	~c_keyvalues();

	void* operator new(std::size_t nAllocSize);
	void operator delete(void* pMemory);

	const char* get_name();

	static c_keyvalues* from_string(const char* szName, const char* szValue);
	void load_from_buffer(char const* szResourceName, const char* szBuffer, void* pFileSystem = nullptr, const char* szPathID = nullptr, get_symbol_proc_fn pfnEvaluateSymbolProc = nullptr);
	bool load_from_file(void* pFileSystem, const char* szResourceName, const char* szPathID = nullptr, get_symbol_proc_fn pfnEvaluateSymbolProc = nullptr);

	c_keyvalues* find_key(const char* szKeyName, const bool bCreate);

	int get_int(const char* szKeyName, const int iDefaultValue);
	float get_float(const char* szKeyName, const float flDefaultValue);
	const char* get_string(const char* szKeyName, const char* szDefaultValue);

	void set_string(const char* szKeyName, const char* szStringValue);
	void set_int(const char* szKeyName, const int iValue);
	void set_uint64(const char* szKeyName, const int nLowValue, const int nHighValue);

	inline void set_bool(const char* szKeyName, const bool bValue)
	{
		set_int(szKeyName, bValue ? 1 : 0);
	}

private:
	std::uint32_t u_key_name : 24; // 0x00
	std::uint32_t u_key_name_case_sensitive1 : 8; // 0x3 // byte, explicitly specify bits due to packing
	char* sz_value; // 0x04
	wchar_t* wsz_value; // 0x08

	union
	{
		int i_value;
		float fl_value;
		void* p_value;
		unsigned char arr_color[4];
	}; // 0x0C

	std::int8_t i_data_type; // 0x10
	bool b_has_escape_sequences; // 0x11
	std::uint16_t u_key_name_case_sensitive2; // 0x12
	void* p_unknown14; // 0x14 // seems like IKeyValuesSystem*, but why do they need it here? also calling smth on destructor and cleans up
	bool b_has_case_insenseitive_key_symbol; // 0x18
	c_keyvalues* p_peer; // 0x1C
	c_keyvalues* p_sub; // 0x20
	c_keyvalues* p_chain; // 0x24
	get_symbol_proc_fn	p_expression_get_symbol_proc; // 0x28
};
static_assert(sizeof(c_keyvalues) == 0x2C);
