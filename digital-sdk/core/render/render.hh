#ifndef RENDER_H
#define RENDER_H

#include "../../context.hh"
#include "../../deps/deps.hh"
#include <memory>

enum e_draw_flags { drop_shadow, outline };

class c_render final {
    ImDrawList* m_draw_list{ };

	struct fonts_t {
        ImFont* m_esp{ };
    } m_fonts{ };
public:
    void init( );
    void line( float x1, float y1, float x2, float y2, c_color clr, float thickness = 1.0f ) const;
    void rect( float x, float y, float w, float h, c_color clr, float rounding = 0.0f ) const;
    void filled_rect( float x, float y, float w, float h, c_color color, float rounding, int flags ) const;
    static bool world_to_screen( const vec3_t& in, vec2_t& out );
    void filled_rect_gradient( float x, float y, float w, float h, c_color col_upr_left, c_color col_upr_right,
                               c_color col_bot_right, c_color col_bot_left ) const;
    void triangle( float x1, float y1, float x2, float y2, float x3, float y3, c_color clr, float thickness ) const;
    void triangle_filled( float x1, float y1, float x2, float y2, float x3, float y3, c_color clr ) const;
    void circle( float x1, float y1, float radius, c_color col, int segments ) const;
    void circle_filled( float x1, float y1, float radius, c_color col, int segments ) const;

    void text( const vec2_t& txt_pos, c_color color, int flags, const ImFont* font,
               const std::string_view& message ) const;

    template < typename... Args > void text( const float x, const float y, const c_color color, const int flags,
                                             const ImFont* font,
                                             const std::string_view& fmt, Args&&... args ) {
        text( vec2_t( x, y ), color, flags, font, std::vformat( fmt, std::make_format_args( args... ) ) );
    }

    auto& draw_list( ) { return m_draw_list; }
    auto& fonts( ) { return m_fonts; }
};

inline const std::unique_ptr< c_render > g_render{ new c_render( ) };

#endif