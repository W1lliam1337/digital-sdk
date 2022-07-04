#include "keyvalues.h"
#include "../utils.h"

c_keyvalues::c_keyvalues(const char* szKeyName, void* pUnknown1, hkey_symbol hCaseInsensitiveKeyName)
{
	using CKeyValuesConstructorFn = void(__thiscall*)(void*, const char*, void*, hkey_symbol);
	static CKeyValuesConstructorFn oConstructor = reinterpret_cast<CKeyValuesConstructorFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 56 8B F1 33 C0 8B 4D 0C 81"))); // @xref: client.dll -> "OldParticleSystem_Destroy"
	oConstructor(this, szKeyName, pUnknown1, hCaseInsensitiveKeyName);
}


c_keyvalues::~c_keyvalues()
{
	using CKeyValuesDestructorFn = void(__thiscall*)(void*, int);
	static CKeyValuesDestructorFn oDestructor = reinterpret_cast<CKeyValuesDestructorFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("56 8B F1 E8 ? ? ? ? 8B 4E 14")));
	oDestructor(this, 1);
}

void* c_keyvalues::operator new(std::size_t nAllocSize)
{
	// manually allocate memory, because game constructor doesn't call it automatically
	return g_sdk.m_interfaces.m_keyvalues->alloc_keyvalues_memory(nAllocSize);
}

void c_keyvalues::operator delete(void* pMemory)
{
	// do nothing, because game destructor will automatically free memory
	// I::KeyValuesSystem->FreeKeyValuesMemory(pMemory);
	(void)pMemory;
}


const char* c_keyvalues::get_name()
{
	return g_sdk.m_interfaces.m_keyvalues->get_string_for_symbol(this->u_key_name_case_sensitive1 | (this->u_key_name_case_sensitive2 << 8));
}

c_keyvalues* c_keyvalues::from_string(const char* szName, const char* szValue)
{
	static auto oFromString = c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 81 EC ? ? ? ? 85 D2 53")); // @xref: "#empty#", "#int#"
	c_keyvalues* pKeyValues = nullptr;

	if (oFromString == 0U)
		return nullptr;

	__asm
	{
		push 0
		mov edx, szValue
		mov ecx, szName
		call oFromString
		add esp, 4
		mov pKeyValues, eax
	}

	return pKeyValues;
}

void c_keyvalues::load_from_buffer(char const* szResourceName, const char* szBuffer, void* pFileSystem, const char* szPathID, get_symbol_proc_fn pfnEvaluateSymbolProc)
{
	using LoadFromBufferFn = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
	static auto oLoadFromBuffer = reinterpret_cast<LoadFromBufferFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"))); // @xref: "KeyValues::LoadFromBuffer(%s%s%s): Begin"
	assert(oLoadFromBuffer != nullptr);

	oLoadFromBuffer(this, szResourceName, szBuffer, pFileSystem, szPathID, pfnEvaluateSymbolProc, nullptr);
}

bool c_keyvalues::load_from_file(void* pFileSystem, const char* szResourceName, const char* szPathID, get_symbol_proc_fn pfnEvaluateSymbolProc)
{
	using LoadFromFileFn = bool(__thiscall*)(void*, void*, const char*, const char*, void*);
	static auto oLoadFromFile = reinterpret_cast<LoadFromFileFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F8 83 EC 14 53 56 8B 75 08 57 FF"))); // @xref: "rb"
	assert(oLoadFromFile != nullptr);

	return oLoadFromFile(this, pFileSystem, szResourceName, szPathID, pfnEvaluateSymbolProc);
}

c_keyvalues* c_keyvalues::find_key(const char* szKeyName, const bool bCreate)
{
	using FindKeyFn = c_keyvalues * (__thiscall*)(void*, const char*, bool);
	static auto oFindKey = reinterpret_cast<FindKeyFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 EC 1C 53 8B D9 85 DB")));
	assert(oFindKey != nullptr);

	return oFindKey(this, szKeyName, bCreate);
}

int c_keyvalues::get_int(const char* szKeyName, const int iDefaultValue)
{
	c_keyvalues* pSubKey = this->find_key(szKeyName, false);

	if (pSubKey == nullptr)
		return iDefaultValue;

	switch (pSubKey->i_data_type)
	{
	case TYPE_STRING:
		return std::atoi(pSubKey->sz_value);
	case TYPE_WSTRING:
		return _wtoi(pSubKey->wsz_value);
	case TYPE_FLOAT:
		return static_cast<int>(pSubKey->fl_value);
	case TYPE_UINT64:
		// can't convert, since it would lose data
		assert(false);
		return 0;
	default:
		break;
	}

	return pSubKey->i_value;
}

float c_keyvalues::get_float(const char* szKeyName, const float flDefaultValue)
{
	c_keyvalues* pSubKey = this->find_key(szKeyName, false);

	if (pSubKey == nullptr)
		return flDefaultValue;

	switch (pSubKey->i_data_type)
	{
	case TYPE_STRING:
		return static_cast<float>(std::atof(pSubKey->sz_value));
	case TYPE_WSTRING:
		return std::wcstof(pSubKey->wsz_value, nullptr);
	case TYPE_FLOAT:
		return pSubKey->fl_value;
	case TYPE_INT:
		return static_cast<float>(pSubKey->i_value);
	case TYPE_UINT64:
		return static_cast<float>(*reinterpret_cast<std::uint64_t*>(pSubKey->sz_value));
	case TYPE_PTR:
	default:
		return 0.0f;
	}
}

const char* c_keyvalues::get_string(const char* szKeyName, const char* szDefaultValue)
{
	using GetStringFn = const char* (__thiscall*)(void*, const char*, const char*);
	static auto oGetString = reinterpret_cast<GetStringFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08")));
	assert(oGetString != nullptr);

	return oGetString(this, szKeyName, szDefaultValue);
}

void c_keyvalues::set_string(const char* szKeyName, const char* szStringValue)
{
	c_keyvalues* pSubKey = find_key(szKeyName, true);

	if (pSubKey == nullptr)
		return;

	using SetStringFn = void(__thiscall*)(void*, const char*);
	static auto oSetString = reinterpret_cast<SetStringFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01")));
	assert(oSetString != nullptr);

	oSetString(pSubKey, szStringValue);
}

void c_keyvalues::set_int(const char* szKeyName, const int iValue)
{
	c_keyvalues* pSubKey = find_key(szKeyName, true);

	if (pSubKey == nullptr)
		return;

	pSubKey->i_value = iValue;
	pSubKey->i_data_type = TYPE_INT;
}

void c_keyvalues::set_uint64(const char* szKeyName, const int nLowValue, const int nHighValue)
{
	c_keyvalues* pSubKey = find_key(szKeyName, true);

	if (pSubKey == nullptr)
		return;

	// delete the old value
	delete[] pSubKey->sz_value;

	// make sure we're not storing the WSTRING - as we're converting over to STRING
	delete[] pSubKey->wsz_value;
	pSubKey->wsz_value = nullptr;

	pSubKey->sz_value = new char[sizeof(std::uint64_t)];
	*reinterpret_cast<std::uint64_t*>(pSubKey->sz_value) = static_cast<std::uint64_t>(nHighValue) << 32ULL | nLowValue;
	pSubKey->i_data_type = TYPE_UINT64;
}