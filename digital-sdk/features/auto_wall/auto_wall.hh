#ifndef AUTO_WALL_H
#define AUTO_WALL_H

#include "../../context.hh"

class c_auto_wall final {
public:
    void scale_damage( const c_game_trace& enter_trace, const c_weapon_info* weapon_info, float& dmg ) const;
    bool trace_to_exit( const c_game_trace& enter_trace, c_game_trace& exit_trace, vec3_t start,
                        vec3_t direction ) const;
    void clip_trace_to_players( const vec3_t& abs_start, const vec3_t& abs_end, c_trace_filter* filter,
                                c_game_trace* trace, float min_range ) const;
    bool handle_bullet_penetration( const c_weapon_info* weapon_data, const c_game_trace& enter_trace,
                                    const vec3_t& direction, vec3_t& eye_pos, int& hits, float& current_dmg,
                                    float pen_power ) const;
    bool fire_bullet( c_player* player, const vec3_t& direction, float& current_dmg, const vec3_t& point,
                      vec3_t& eye_pos, int& hits ) const;
};

inline const std::unique_ptr< c_auto_wall > g_auto_wall { new c_auto_wall( ) };

#endif