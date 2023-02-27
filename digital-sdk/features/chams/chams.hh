#ifndef CHAMS_H
#define CHAMS_H

#include "../../context.hh"

class c_chams final {
    i_material* m_regular{ };
    static i_material* create_material( std::string_view material_name, std::string_view shader_type,
                                        std::string_view material_data );
    void override_material( const c_color& clr, bool ignorez ) const;
public:
    void init_material( );
    bool draw_model( i_model_render* const model_render, void* edx, void* context, draw_model_state_t& state,
                     c_model_render_info& info, matrix3x4_t* bones ) const;
};

inline const std::unique_ptr< c_chams > g_chams{ new c_chams( ) };

#endif