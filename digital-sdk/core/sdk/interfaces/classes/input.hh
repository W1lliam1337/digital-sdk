#ifndef VALVE_INPUT
#define VALVE_INPUT

#include "../../misc/user_cmd.hh"
#include "../../misc/base_handle.hh"

class c_weapon;

struct camera_third_data_t {
    float m_pitch{ }, m_yaw{ }, m_dist{ }, m_lag{ };
    vec3_t m_hull_min{ }, m_hull_max{ };
};

class i_input {
    void* dword0{ };
    [[maybe_unused]] std::byte gap4[ 164 ]{ };
public:
    bool m_cam_moving_with_mouse{ };
    bool m_cam_in_third_person{ };
    bool m_cam_intercepting_mouse{ };
private:
    char unknown[ 1 ]{ };
public:
    vec3_t m_cam_offset{ };
    bool m_cam_distance_move{ };
private:
    char unknown2[ 3 ]{ };
public:
    vec2_t m_old_cam{ };
    vec2_t m_cur_cam{ };
private:
    [[maybe_unused]] std::byte gapCC[ 36 ]{ };
public:
    c_user_cmd* m_cmds{ };
    c_verified_user_cmd* m_ver_cmds{ };
    c_handle< c_weapon* > m_selected_weapon{ };
    camera_third_data_t m_cam_third_data{ };
    int m_cam_cmd{ };

    [[nodiscard]] c_user_cmd* get_user_cmd( const int sequence_number ) const {
        return &m_cmds[ sequence_number % 150 ];
    }

    [[nodiscard]] c_verified_user_cmd* get_verified_cmd( const int sequence_number ) const {
        return &m_ver_cmds[ sequence_number % 150 ];
    }
};

#endif