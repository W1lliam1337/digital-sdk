#include "esp.hh"

bool c_esp::get_bounding_box( const c_player* player, box_t& box ) {
    const auto adjust = vec3_t( 0, 0, -18 ) * player->duck_amount( );
    const auto air = !( player->flags( ) & fl_on_ground ) && ( player->move_type( ) != move_type_ladder )
        ? vec3_t( 0, 0, 10 )
        : vec3_t( 0, 0, 0 );

    const auto down = player->origin( ) + air;
    const auto top = down + vec3_t( 0, 0, 72 ) + adjust;

    if ( vec2_t s[ 2 ]; g_render->world_to_screen( top, s[ 1 ] ) && g_render->world_to_screen( down, s[ 0 ] ) ) {
        const vec2_t delta = s[ 1 ] - s[ 0 ];

        box.h = fabsf( delta.y );
        box.w = box.h / 2.0f;

        box.x = s[ 1 ].x - box.w / 2;
        box.y = s[ 1 ].y;

        return true;
    }

    return false;
}

int c_esp::get_type( c_player* const player ) {
    if ( player->is_player( ) && player != c_player::get_local( ) && player->team( ) != c_player::get_local( )->
        team( ) )
        return player_type;

    if ( player == c_player::get_local( ) && g_interfaces->m_input->m_cam_in_third_person )
        return local_type;

    if ( player->team( ) == c_player::get_local( )->team( ) )
        return team_type;

    return both_type;
}

void c_esp::init( ) {
    auto loop = [ & ]( c_entity* const entity ) {
        const auto player = static_cast< c_player* const >( entity );
        if ( !player->is_alive( ) )
            return;

        m_type = get_type( player );
        if ( m_type != e_esp_type::player_type )
            return;

        box_t box{ };
        if ( !get_bounding_box( player, box ) )
            return;

        m_player.init( player, box );
    };

    g_listener->loop( loop );
}