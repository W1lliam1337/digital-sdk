#ifndef ESP_H
#define ESP_H

#include "../../context.hh"
#include "../../core/render/render.hh"

enum e_esp_type {
    both_type,
    player_type,
    team_type,
    local_type,
};

class c_esp final {
    struct box_t {
        float x{ }, y{ }, w{ }, h{ };
    };

    struct player_t {
        static void init( c_player* player, box_t& box );
        static void draw_box( c_player* player, box_t& box );
        static void draw_name( c_player* player, box_t& box );
    } m_player{ };

    int m_type{ };

    static bool get_bounding_box( const c_player* player, box_t& box );
    static int get_type( c_player* player );
public:
    void init( );
};

inline const std::unique_ptr< c_esp > g_esp{ new c_esp( ) };

#endif