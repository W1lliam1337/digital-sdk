#pragma once
#include "i_convar.h"
#include "../../../utils/utils.h"

class i_cvar
{
public:
	i_convar* find_var(const char* convar_name)
	{
		return utils::call_vfunc<i_convar* (__thiscall*)(void*, const char*)>(this, 16)(this, convar_name);
	}

	template< typename... arguments >
	void console_color_print_f(const c_color& color, const char* format, arguments ... args)
	{
		return utils::call_vfunc< void(__cdecl*)(void*, const c_color&, const char* ...)>(this, 25)(this, color, format, args...);
	}

	template< typename... arguments >
	void console_printf(const char* format, arguments ... args)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, const char* ...)>(this, 26)(this, format);
	}
};