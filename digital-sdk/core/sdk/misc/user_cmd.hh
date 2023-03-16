#ifndef VALVE_USER_CMD
#define VALVE_USER_CMD

#include "../../utils/math/math.hh"

enum e_buttons {
    in_attack = 1 << 0,
    in_jump = 1 << 1,
    in_duck = 1 << 2,
    in_forward = 1 << 3,
    in_back = 1 << 4,
    in_use = 1 << 5,
    in_cancel = 1 << 6,
    in_left = 1 << 7,
    in_right = 1 << 8,
    in_moveleft = 1 << 9,
    in_moveright = 1 << 10,
    in_attack2 = 1 << 11,
    in_run = 1 << 12,
    in_reload = 1 << 13,
    in_alt1 = 1 << 14,
    in_alt2 = 1 << 15,
    in_score = 1 << 16,
    in_speed = 1 << 17,
    in_walk = 1 << 18,
    in_zoom = 1 << 19,
    in_weapon1 = 1 << 20,
    in_weapon2 = 1 << 21,
    in_bullrush = 1 << 22,
    in_grenade1 = 1 << 23,
    in_grenade2 = 1 << 24,
    in_lookspin = 1 << 25
};

class c_user_cmd {
    [[maybe_unused]] std::byte pad0[ 0x4 ]{ };
public:
    int m_command_number{ };
    int m_tick_count{ };
    vec3_t m_view_angles;
    vec3_t m_aim_direction;
    float m_forward_move{ };
    float m_side_move{ };
    float m_up_move{ };
    int m_buttons{ };
    int m_impulse{ };
    int m_weapon_select{ };
    int m_weapon_sub_type{ };
    int m_random_seed{ };
    short m_moused_x{ };
    short m_moused_y{ };
    bool m_has_been_predicted{ };
    vec3_t m_head_angles;
    vec3_t m_head_offset;
    [[nodiscard]] std::uint32_t get_checksum( ) const;
};

class c_verified_user_cmd {
public:
    c_user_cmd m_cmd{ };
    std::uint32_t m_crc{ };
};

#endif