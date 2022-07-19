#pragma once
#include "../../math/vec3.h"
#include "../../other/cmd.h"

class c_verified_user_cmd;

class i_input
{
public:
	char pad0[0xC]{};
	bool m_trackir_available{};
	bool m_mouse_initialized{};
	bool m_mouse_active{};
	char pad1[0x9A]{};
	bool m_camera_in_third_person{};
	char pad2[0x2]{};
	c_vec3 m_camera_offset{};
	char pad3[0x38]{};
	c_user_cmd* m_commands{};
	c_verified_user_cmd* m_verified_commands{};

	[[nodiscard]] c_user_cmd* get_user_cmd(const int sequence_number) const
	{
		return &m_commands[sequence_number % 150];
	}

	[[nodiscard]] c_verified_user_cmd* get_verified_cmd(const int sequence_number) const
	{
		return &m_verified_commands[sequence_number % 150];
	}
};