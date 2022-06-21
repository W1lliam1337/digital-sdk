#pragma once
#include "../../sdk.hpp"
#include "c_user_cmd.h"

class c_verified_user_cmd;

class i_input
{
public:
	char pad0[0xC]{}; // 0x0
	bool m_trackir_available{}; // 0xC
	bool m_mouse_initialized{}; // 0xD
	bool m_mouse_active{}; // 0xE
	char pad1[0x9A]{}; // 0xF
	bool m_camera_in_third_person{}; // 0xAD
	char pad2[0x2]{}; // 0xAE
	vec3_t m_camera_offset; // 0xB0
	char pad3[0x38]{}; // 0xBC
	c_user_cmd* m_commands{}; // 0xF4
	c_verified_user_cmd* m_verified_commands{}; // 0xF8   

	[[nodiscard]] c_user_cmd* get_user_cmd(const int sequence_number) const
	{
		return &this->m_commands[sequence_number % 150];
	}

	c_user_cmd* get_user_cmd(const int slot, const int sequence_number)
	{
		return call_vfunc<c_user_cmd* (__thiscall*)(void*, int, int)>(this, 8)(this, slot, sequence_number);
	}

	c_verified_user_cmd* get_verified_cmd(const int sequence_number)
	{
		const auto verified_commands = *reinterpret_cast<c_verified_user_cmd**>(reinterpret_cast<uint32_t>(this) + 0xF4);
		return &verified_commands[sequence_number % 150];
	}
};

