#include "movement.h"

void c_movement::bunny_hop()
{
	if (g_sdk.m_local()->get_move_type() == move_type_ladder
		|| g_sdk.m_local()->get_move_type() == move_type_noclip)
	{
		return;
	}

	if (g_sdk.m_packet_data.m_cmd->m_buttons & in_jump
		&& !(g_sdk.m_local()->get_flags() & fl_on_ground))
	{
		g_sdk.m_packet_data.m_cmd->m_buttons &= ~in_jump;
	}
}