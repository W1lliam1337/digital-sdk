#include "local_anims.h"

void c_local_anims::init()
{
	
}

void c_local_anims::fix_balance_adjust_animation()
{
	if (!(g_sdk.m_local()->get_sequence_activity(g_sdk.m_local()->get_anim_layers()[3].m_sequence) == act_csgo_idle_turn_balanceadjust))
		return;


}