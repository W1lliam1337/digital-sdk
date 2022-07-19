#pragma once
#include <cstdint>
#include "../math/qangle.h"
#include "../math/vec3.h"

class c_user_cmd
{
public:
	void* m_vtable{}; // 0x00
	int m_command{}; // 0x04
	int m_tick_count{}; // 0x08
	qangle_t m_view_angles{}; // 0x0C Player instantaneous view angles.
	c_vec3 m_aim_direction{}; // 0x18
	float m_forward_move{}; // 0x24
	float m_side_move{}; // 0x28
	float m_up_move{}; // 0x2C
	int m_buttons{}; // 0x30 Attack button states
	char m_impulse{}; // 0x34
	int m_weapon_select{}; // 0x38 Current weapon id
	int m_weapon_sub_type{}; // 0x3C
	int m_random_seed{}; // 0x40 For shared random functions
	short m_mouse_direction_x{}; // 0x44 mouse accum in x from create move
	short m_mouse_direction_y{}; // 0x46 mouse accum in y from create move
	bool m_has_been_predicted{}; // 0x48 Client only, tracks whether we've predicted this command at least once
	char pad[0x18]{};
	[[nodiscard]] std::uint32_t get_checksum() const;
};

class c_verified_user_cmd
{
public:
	c_user_cmd m_cmd{};
	std::uint32_t m_crc{};
};

