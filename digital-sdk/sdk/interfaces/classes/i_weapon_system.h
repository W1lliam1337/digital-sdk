#pragma once

class c_weapon_info;

class i_weapon_system
{
public:
	c_weapon_info* get_weapon_data(short item_definition_index)
	{
		return call_vfunc<c_weapon_info*(__thiscall*)(void*, short)>(this, 8)(this, item_definition_index);
	}
};
