#include "../hooks.h"

bool __fastcall hooks::hk_is_paused(void* ecx, void* edx) 
{
	static const auto extrapolation = static_cast<void*>(utils::sig(modules::m_client_dll, _("FF D0 A1 ? ? ? ? B9 ? ? ? ?")) + 0x29);
	
	// @note: skip call to extrapolation
	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/c_baseentity.cpp#L3983
	if (_ReturnAddress() == extrapolation)
		return true;

	return og::m_is_paused(ecx, edx);
}

bool __fastcall hooks::hk_is_hltv(void* ecx, void* edx) 
{
	static auto setup_velocity = static_cast<void*>(utils::sig(modules::m_client_dll, _("84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80")));
	static auto accumulate_layers = static_cast<void*>(utils::sig(modules::m_client_dll, _("84 C0 75 0D F6 87")));

	// @note: skip call to setup_velocity and accumulate_layers
	if (_ReturnAddress() == setup_velocity || _ReturnAddress() == accumulate_layers)
		return true;

	return og::m_is_hltv(ecx, edx);
}