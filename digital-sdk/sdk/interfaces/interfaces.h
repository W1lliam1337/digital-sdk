#pragma once
#include "../sdk.hpp"

using create_interface_fn = void* (*)(const char*, int*);

class c_interfaces : public c_singleton<c_interfaces>
{
public:
	void init() const;
	void* get_interface(HMODULE module, const char* interface_name) const;
};

// from qu0 lol also should prob move this
__forceinline void* GetExportAddress(const void* pModuleBase, const std::string_view szProcedureName)
{
	const std::uint8_t* pAddress = static_cast<const std::uint8_t*>(pModuleBase);
	const IMAGE_DOS_HEADER* pDosHeader = static_cast<const IMAGE_DOS_HEADER*>(pModuleBase);
	const IMAGE_NT_HEADERS* pNtHeaders = reinterpret_cast<const IMAGE_NT_HEADERS*>(pAddress + pDosHeader->e_lfanew);
	const IMAGE_OPTIONAL_HEADER* pOptionalHeader = &pNtHeaders->OptionalHeader;

	const std::uintptr_t uExportSize = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	const std::uintptr_t uExportAddress = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

	if (uExportSize <= 0U)
		return nullptr;

	const IMAGE_EXPORT_DIRECTORY* pExportDirectory = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(pAddress + uExportAddress);
	const std::uintptr_t* pNamesRVA = reinterpret_cast<const std::uintptr_t*>(pAddress + pExportDirectory->AddressOfNames);
	const std::uintptr_t* pFunctionsRVA = reinterpret_cast<const std::uintptr_t*>(pAddress + pExportDirectory->AddressOfFunctions);
	const std::uint16_t* pNameOrdinals = reinterpret_cast<const std::uint16_t*>(pAddress + pExportDirectory->AddressOfNameOrdinals);

	// perform binary search
	std::uintptr_t uRight = pExportDirectory->NumberOfNames;
	std::uintptr_t uLeft = 0;

	while (uRight != uLeft)
	{
		const std::uintptr_t uMiddle = uLeft + ((uRight - uLeft) >> 1U);
		const int iResult = szProcedureName.compare(reinterpret_cast<const char*>(pAddress + pNamesRVA[uMiddle]));

		if (iResult == 0)
			return const_cast<void*>(static_cast<const void*>(pAddress + pFunctionsRVA[pNameOrdinals[uMiddle]]));

		if (iResult > 0)
			uLeft = uMiddle;
		else
			uRight = uMiddle;
	}

	return nullptr;
}
