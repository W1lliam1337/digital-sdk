#ifndef VALVE_PHYS_PROPS
#define VALVE_PHYS_PROPS

#include "../../../utils/memory/memory.hh"
#include <cstdint>

enum e_char_tex {
    char_tex_antlion = 'a',
    char_tex_bloodyflesh = 'b',
    char_tex_concrete = 'c',
    char_tex_dirt = 'd',
    char_tex_eggshell = 'e',
    char_tex_flesh = 'f',
    char_tex_grate = 'g',
    char_tex_alienflesh = 'h',
    char_tex_clip = 'i',
    char_tex_plastic = 'l',
    char_tex_metal = 'm',
    char_tex_sand = 'n',
    char_tex_foliage = 'o',
    char_tex_computer = 'p',
    char_tex_slosh = 's',
    char_tex_tile = 't',
    char_tex_cardboard = 'u',
    char_tex_vent = 'v',
    char_tex_wood = 'w',
    char_tex_glass = 'y',
    char_tex_warpshield = 'z',
};

struct surface_physics_params_t {
    float m_friction{ }, m_elasticity{ }, m_density{ }, m_thickness{ }, m_dampening{ };
};

struct surface_audio_params_t {
    float m_audio_reflectivity{ }, m_audio_hardness_factor{ }, m_audio_roughness_factor{ }, m_scrape_rough_threshold{ },
        m_impact_hard_threshold{ }, m_audio_hard_min_velocity{ }, m_high_pitch_occlusion{ }, m_mid_pitch_occlusion{ },
        m_low_pitch_occlusion{ };
};

struct surface_game_props_t {
    float m_max_speed_factor{ }, m_jump_factor{ }, m_pen_modifier{ }, m_dmg_modifier{ };

    std::uint16_t m_material{ };
    std::uint8_t m_climbable{ };
};

struct surface_sound_names_t {
    short m_walk_left{ }, m_walk_right{ }, m_run_left{ }, m_run_right{ }, m_impact_soft{ }, m_impact_hard{ },
        m_scrape_smooth{ }, m_scrape_rough{ }, m_bullet_impact{ }, rm_olling{ }, m_break_sound{ }, m_strain{ };
};

class c_surface_data {
public:
    surface_physics_params_t m_physics{ };
    surface_audio_params_t m_audio{ };
    surface_sound_names_t m_sounds{ };
    surface_game_props_t m_game{ };
private:
   [[maybe_unused]] std::byte pad[ 48 ]{ };
};

class i_phys_surface_props {
public:
    VFUNC( c_surface_data*( __thiscall* ) ( void*, int ), get_surface_data( const int surface_index ), 5,
           surface_index );
};

#endif