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
	return g_sdk.m_interfaces.m_keyvalues->AllocKeyValuesMemory(nAllocSize);
}

void c_keyvalues::operator delete(void* pMemory)
{
	// do nothing, because game destructor will automatically free memory
	// I::KeyValuesSystem->FreeKeyValuesMemory(pMemory);
	(void)pMemory;
}


const char* c_keyvalues::GetName()
{
	return g_sdk.m_interfaces.m_keyvalues->GetStringForSymbol(this->uKeyNameCaseSensitive1 | (this->uKeyNameCaseSensitive2 << 8));
}

c_keyvalues* c_keyvalues::FromString(const char* szName, const char* szValue)
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

void c_keyvalues::LoadFromBuffer(char const* szResourceName, const char* szBuffer, void* pFileSystem, const char* szPathID, GetSymbolProcFn pfnEvaluateSymbolProc)
{
	using LoadFromBufferFn = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
	static auto oLoadFromBuffer = reinterpret_cast<LoadFromBufferFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"))); // @xref: "KeyValues::LoadFromBuffer(%s%s%s): Begin"
	assert(oLoadFromBuffer != nullptr);

	oLoadFromBuffer(this, szResourceName, szBuffer, pFileSystem, szPathID, pfnEvaluateSymbolProc, nullptr);
}

bool c_keyvalues::LoadFromFile(void* pFileSystem, const char* szResourceName, const char* szPathID, GetSymbolProcFn pfnEvaluateSymbolProc)
{
	using LoadFromFileFn = bool(__thiscall*)(void*, void*, const char*, const char*, void*);
	static auto oLoadFromFile = reinterpret_cast<LoadFromFileFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F8 83 EC 14 53 56 8B 75 08 57 FF"))); // @xref: "rb"
	assert(oLoadFromFile != nullptr);

	return oLoadFromFile(this, pFileSystem, szResourceName, szPathID, pfnEvaluateSymbolProc);
}

c_keyvalues* c_keyvalues::FindKey(const char* szKeyName, const bool bCreate)
{
	using FindKeyFn = c_keyvalues * (__thiscall*)(void*, const char*, bool);
	static auto oFindKey = reinterpret_cast<FindKeyFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 EC 1C 53 8B D9 85 DB")));
	assert(oFindKey != nullptr);

	return oFindKey(this, szKeyName, bCreate);
}

int c_keyvalues::GetInt(const char* szKeyName, const int iDefaultValue)
{
	c_keyvalues* pSubKey = this->FindKey(szKeyName, false);

	if (pSubKey == nullptr)
		return iDefaultValue;

	switch (pSubKey->iDataType)
	{
	case TYPE_STRING:
		return std::atoi(pSubKey->szValue);
	case TYPE_WSTRING:
		return _wtoi(pSubKey->wszValue);
	case TYPE_FLOAT:
		return static_cast<int>(pSubKey->flValue);
	case TYPE_UINT64:
		// can't convert, since it would lose data
		assert(false);
		return 0;
	default:
		break;
	}

	return pSubKey->iValue;
}

float c_keyvalues::GetFloat(const char* szKeyName, const float flDefaultValue)
{
	c_keyvalues* pSubKey = this->FindKey(szKeyName, false);

	if (pSubKey == nullptr)
		return flDefaultValue;

	switch (pSubKey->iDataType)
	{
	case TYPE_STRING:
		return static_cast<float>(std::atof(pSubKey->szValue));
	case TYPE_WSTRING:
		return std::wcstof(pSubKey->wszValue, nullptr);
	case TYPE_FLOAT:
		return pSubKey->flValue;
	case TYPE_INT:
		return static_cast<float>(pSubKey->iValue);
	case TYPE_UINT64:
		return static_cast<float>(*reinterpret_cast<std::uint64_t*>(pSubKey->szValue));
	case TYPE_PTR:
	default:
		return 0.0f;
	}
}

const char* c_keyvalues::GetString(const char* szKeyName, const char* szDefaultValue)
{
	using GetStringFn = const char* (__thiscall*)(void*, const char*, const char*);
	static auto oGetString = reinterpret_cast<GetStringFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08")));
	assert(oGetString != nullptr);

	return oGetString(this, szKeyName, szDefaultValue);
}

void c_keyvalues::SetString(const char* szKeyName, const char* szStringValue)
{
	c_keyvalues* pSubKey = FindKey(szKeyName, true);

	if (pSubKey == nullptr)
		return;

	using SetStringFn = void(__thiscall*)(void*, const char*);
	static auto oSetString = reinterpret_cast<SetStringFn>(c_utils::get()->find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01")));
	assert(oSetString != nullptr);

	oSetString(pSubKey, szStringValue);
}

void c_keyvalues::SetInt(const char* szKeyName, const int iValue)
{
	c_keyvalues* pSubKey = FindKey(szKeyName, true);

	if (pSubKey == nullptr)
		return;

	pSubKey->iValue = iValue;
	pSubKey->iDataType = TYPE_INT;
}

void c_keyvalues::SetUint64(const char* szKeyName, const int nLowValue, const int nHighValue)
{
	c_keyvalues* pSubKey = FindKey(szKeyName, true);

	if (pSubKey == nullptr)
		return;

	// delete the old value
	delete[] pSubKey->szValue;

	// make sure we're not storing the WSTRING - as we're converting over to STRING
	delete[] pSubKey->wszValue;
	pSubKey->wszValue = nullptr;

	pSubKey->szValue = new char[sizeof(std::uint64_t)];
	*reinterpret_cast<std::uint64_t*>(pSubKey->szValue) = static_cast<std::uint64_t>(nHighValue) << 32ULL | nLowValue;
	pSubKey->iDataType = TYPE_UINT64;
}