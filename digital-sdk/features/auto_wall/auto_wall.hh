#ifndef AUTO_WALL_H
#define AUTO_WALL_H

#include "../../context.hh"

class c_auto_wall final {
public:
    void scale_damage( const c_game_trace& enter_trace, const c_weapon_info* weapon_info, float& dmg ) const;
    bool trace_to_exit( const c_game_trace& enter_trace, c_game_trace& exit_trace, c_vec3 start,
                        c_vec3 direction ) const;
    void clip_trace_to_players( const c_vec3& abs_start, const c_vec3& abs_end, c_trace_filter* filter,
                                c_game_trace* trace, float min_range ) const;
    bool handle_bullet_penetration( const c_weapon_info* weapon_data, const c_game_trace& enter_trace,
                                    const c_vec3& direction, c_vec3& eye_pos, int& hits, float& current_dmg,
                                    float pen_power ) const;
    bool fire_bullet( c_player* player, const c_vec3& direction, float& current_dmg, const c_vec3& point,
                      c_vec3& eye_pos, int& hits ) const;
};

inline const std::unique_ptr< c_auto_wall > g_auto_wall { new c_auto_wall( ) };

#endif