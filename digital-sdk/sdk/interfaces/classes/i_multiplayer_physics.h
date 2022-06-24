#pragma once

class i_multiplayer_physics
{
public:
	virtual int		get_multiplayer_physics_mode() = 0;
	virtual float	get_mass() = 0;
	virtual bool	is_asleep() = 0;
};