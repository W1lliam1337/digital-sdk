#include "../esp.hh"

void c_esp::player_t::init( c_player* const player, box_t& box ) {
    draw_box( player, box );
    draw_name( player, box );
}

void c_esp::player_t::draw_box( c_player* const player, box_t& box ) {
    g_render->rect( box.x + 1, box.y + 1, box.w - 2, box.h - 2, c_color( 0, 0, 0, 255 ) );
    g_render->rect( box.x - 1, box.y - 1, box.w + 2, box.h + 2, c_color( 0, 0, 0, 255 ) );
    g_render->rect( box.x, box.y, box.w, box.h, c_color( 255, 255, 255 ) );
}

void c_esp::player_t::draw_name( c_player* const player, box_t& box ) {
    c_player_info info{ };
    if ( !g_interfaces->m_engine->get_player_info( player->get_index( ), &info ) )
        return;

    g_render->text( box.x + box.w / 2.0f, box.y - 17.0f, c_color::white, outline, g_render->fonts( ).m_esp,
                    info.m_name );
}