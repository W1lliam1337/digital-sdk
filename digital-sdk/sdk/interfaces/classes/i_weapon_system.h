#pragma once
#include "../../entity/classes.h"

class i_weapon_system
{
	virtual void unused0() = 0;
	virtual void unused1() = 0;
public:
	virtual c_weapon_info* get_wpn_data(unsigned ItemDefinitionIndex) = 0;
};
