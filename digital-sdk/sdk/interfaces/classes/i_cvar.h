#pragma once
#include "i_convar.h"
#include "../../../dependencies/vfunc/vfunc.h"

class i_cvar
{
public:
	i_convar* find_var(const char* convar_name)
	{
		return call_vfunc<i_convar* (__thiscall*)(void*, const char*)>(this, 16)(this, convar_name);
	}
};