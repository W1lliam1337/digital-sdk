#pragma once
#include "player.h"

class c_inferno : public c_base_player
{
public:
	// [DT_FireCrackerBlast]
	GET_NETVAR(int, get_fire_x_delta, _("DT_Inferno"), _("m_fireXDelta")); // 0x9E4
	GET_NETVAR(int, get_fire_y_delta, _("DT_Inferno"), _("m_fireYDelta")); // 0xB74
	GET_NETVAR(int, get_fire_z_delta, _("DT_Inferno"), _("m_fireZDelta")); // 0xD04
	GET_NETVAR(bool, fire_is_burning, _("DT_Inferno"), _("m_bFireIsBurning")); // 0xE94
	GET_NETVAR(int, get_fire_effect_tick_begin, _("DT_Inferno"), _("m_nFireEffectTickBegin")); // 0x13B4
	GET_NETVAR(int, get_fire_count, _("DT_Inferno"), _("m_fireCount")); // 0x13A8
};
