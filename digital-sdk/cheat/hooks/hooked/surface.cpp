#include "../hooks.h"

void __fastcall hooks::hk_lock_cursor(void* ecx, void* edx)
{
	return ctx::menu_data::m_is_menu_opened ? interfaces::m_surface->unlock_cursor() : hooks::og::m_lock_cursor(ecx, edx);
}