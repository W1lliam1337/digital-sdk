#include "../hooks.h"

void __stdcall hooks::hk_create_move(int sequence, float frame_time, bool is_active, bool& send_packet)
{
	og::m_create_move(sequence, frame_time, is_active);

	const auto cmd = interfaces::m_input->get_user_cmd(sequence);
	const auto verified_cmd = interfaces::m_input->get_verified_cmd(sequence);

	if (!cmd || !verified_cmd)
		return;

	ctx::packet_data::m_cmd = cmd;
	ctx::packet_data::m_send_packet = true;

	engine_prediction::update();

	movement::bunny_hop();

	engine_prediction::setup();
	engine_prediction::begin();
	{
		legit_bot::init();
	}
	engine_prediction::end();

	send_packet = ctx::packet_data::m_send_packet;
	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->get_checksum();
}

__declspec(naked) void __stdcall hooks::hk_create_move_proxy(int sequence, float frame_time, bool is_active)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push ebx
		lea ecx, [esp]
		push ecx
		push dword ptr[ebp + 10h]
		push dword ptr[ebp + 0Ch]
		push dword ptr[ebp + 8]
		call hk_create_move
		pop ebx
		pop ebp
		retn 0Ch
	}
}

// @note: way of removing molotov
// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/cstrike15/Effects/clientinferno.cpp#L89
void __fastcall hooks::hk_inferno_client_think(void* ecx, void* edx)
{
	if (g_cfg.m_misc.m_remove_molotov)
		return;

	return og::m_inferno_client_think(ecx, edx);
}

// @note: way of removing blood on hit
// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/cstrike15/fx_cs_blood.cpp#L376
void __cdecl hooks::hk_blood_spray_callback()
{
	return;
}