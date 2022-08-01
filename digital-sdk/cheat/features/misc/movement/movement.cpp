#include "movement.h"

void movement::bunny_hop()
{
	if (!g_cfg.m_misc.m_bunny_hop)
		return;

	if (!ctx::local() || !ctx::packet_data::m_cmd)
		return;

	if (ctx::local()->get_move_type() == move_type_ladder
		|| ctx::local()->get_move_type() == move_type_noclip)
	{
		return;
	}

	if (ctx::packet_data::m_cmd->m_buttons & in_jump
		&& !(ctx::local()->get_flags() & fl_on_ground))
	{
		ctx::packet_data::m_cmd->m_buttons &= ~in_jump;
	}
}