#pragma once
#include "../../../dependencies/vfunc/vfunc.h"

class i_panel
{
public:
	const char* get_name(const unsigned int vgui_panel)
	{
		return call_vfunc<const char* (__thiscall*)(void*, unsigned int)>(this, 36)(this, vgui_panel);
	}

	const char* get_class_name(const unsigned int vgui_panel)
	{
		return call_vfunc<const char* (__thiscall*)(void*, unsigned int)>(this, 37)(this, vgui_panel);
	}
};
