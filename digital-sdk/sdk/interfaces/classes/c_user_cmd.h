#pragma once
#include <cstdint>

class c_user_cmd
{
public:
	[[nodiscard]] crc32_t get_checksum(void) const
	{
		crc32_t crc;
		crc32_init(&crc);

		crc32_process_buffer(&crc, &m_command, 4);
		crc32_process_buffer(&crc, &m_tick_count, 4);
		crc32_process_buffer(&crc, &m_view_angles, sizeof(m_view_angles));
		crc32_process_buffer(&crc, &m_aim_direction, sizeof(m_aim_direction));
		crc32_process_buffer(&crc, &m_forward_move, sizeof(m_forward_move));
		crc32_process_buffer(&crc, &m_side_move, sizeof(m_side_move));
		crc32_process_buffer(&crc, &m_up_move, sizeof(m_up_move));
		crc32_process_buffer(&crc, &m_buttons, sizeof(m_buttons));
		crc32_process_buffer(&crc, &m_impulse, sizeof(m_impulse));
		crc32_process_buffer(&crc, &m_weapon_select, sizeof(m_weapon_select));
		crc32_process_buffer(&crc, &m_weapon_sub_type, sizeof(m_weapon_sub_type));
		crc32_process_buffer(&crc, &m_random_seed, sizeof(m_random_seed));
		crc32_process_buffer(&crc, &m_mouse_direction_x, sizeof(m_mouse_direction_x));
		crc32_process_buffer(&crc, &m_mouse_direction_y, sizeof(m_mouse_direction_y));

		crc32_final(&crc);
		return crc;
	}

	void* m_vtable{}; // 0x00
	int m_command{}; // 0x04
	int m_tick_count{}; // 0x08
	qangle_t m_view_angles{}; // 0x0C Player instantaneous view angles.
	vec3_t m_aim_direction{}; // 0x18
	float m_forward_move{}; // 0x24
	float m_side_move{}; // 0x28
	float m_up_move{}; // 0x2C
	bit_flag_t<int> m_buttons{}; // 0x30 Attack button states
	char m_impulse{}; // 0x34
	int m_weapon_select{}; // 0x38 Current weapon id
	int m_weapon_sub_type{}; // 0x3C
	int m_random_seed{}; // 0x40 For shared random functions
	short m_mouse_direction_x{}; // 0x44 mouse accum in x from create move
	short m_mouse_direction_y{}; // 0x46 mouse accum in y from create move
	bool m_has_been_predicted{}; // 0x48 Client only, tracks whether we've predicted this command at least once
	char pad[0x18]{};
};

class c_verified_user_cmd
{
public:
	c_user_cmd m_cmd{};
	crc32_t m_crc{};
};

