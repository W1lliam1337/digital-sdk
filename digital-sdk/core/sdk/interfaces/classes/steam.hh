#ifndef STEAM_API_H
#define STEAM_API_H

#include "../../../utils/memory/memory.hh"

using h_steam_pipe = int;
using h_steam_user = int;

#define ARGS h_steam_user steam_user, h_steam_pipe steam_pipe, const char *version

class i_steam_client {
public:
    VFUNC( h_steam_pipe( __thiscall* )( void* ), create_steam_pipe( ), 0 );
    VFUNC( void*( __thiscall* ) ( void*, ARGS ), get_steam_generic_interface( ARGS ), 12, steam_user, steam_pipe,
           version );
};

class i_steam_screenshots {
public:
    VFUNC( std::uint32_t( __thiscall* )( void*, void*, std::uint32_t, int, int ),
           write_screenshot( void* pub_rgb, const std::uint32_t cub_rgb, const int width, const int height ), 0,
           pub_rgb, cub_rgb, width, height );
};

class c_steam_api_context {
public:
    i_steam_client* m_steam_client{ };
    void* m_steam_user{ };
    void* m_steam_friends{ };
    void* m_steam_utils{ };
    void* m_steam_matchmaking{ };
    void* m_steam_game_search{ };
    void* m_steam_user_stats{ };
    void* m_steam_apps{ };
    void* m_steam_matchmaking_servers{ };
    void* m_steam_networking{ };
    void* m_steam_remote_storage{ };
    i_steam_screenshots* m_steam_steam_screenshots{ };
    void* m_steam_http{ };
    void* m_controller{ };
    void* m_steam_ugc{ };
    void* m_steam_app_list{ };
    void* m_steam_music{ };
    void* m_steam_music_remote{ };
    void* m_steam_html_surface{ };
    void* m_steam_inventory{ };
    void* m_steam_video{ };
    void* m_steam_parental_settings{ };
    void* m_steam_input{ };
};

#endif