#ifndef VALVE_ENGINE
#define VALVE_ENGINE

#include "../../../utils/math/matrix.hh"
#include "../../misc/client_state.hh"
#include "../../modules/modules.hh"
#include "steam.hh"

class c_player_info {
public:
    std::int64_t m_something{ };
    union {
        std::int64_t m_steam_id64{ };
        struct {
            std::int32_t m_xuid_low;
            std::int32_t m_xuid_high;
        };
    };

    char m_name[ 128 ]{ };
    int m_user_id{ };
    char m_steam_id[ 20 ]{ };
private:
   [[maybe_unused]] std::byte pad1[ 0x10 ]{ };
public:
    unsigned long m_un_steam_id{ };
    char m_friends_name[ 128 ]{ };
    bool m_is_bot{ };
    bool m_is_hltv{ };
    unsigned int m_custom_files[ 4 ]{ };
    unsigned char m_files_downloaded{ };
};

enum client_frame_stage_t {
    frame_undefined = -1,
    frame_start,
    frame_net_update_start,
    frame_net_update_postdataupdate_start,
    frame_net_update_postdataupdate_end,
    frame_net_update_end,
    frame_render_start,
    frame_render_end,
    frame_net_full_frame_update_on_remove
};

class i_engine_client {
public:
    VFUNC( bool( __thiscall* )( void*, int, c_player_info* ), get_player_info( const int ent_num, c_player_info* info ),
           8, ent_num, info );
    VFUNC( bool( __thiscall* )( void* ), con_is_visible( ), 11 );
    VFUNC( int( __thiscall* )( void* ), get_local_player( ), 12 );
    VFUNC( void( __thiscall* )( void*, vec3_t& ), get_view_angles( vec3_t& angle ), 18, angle );
    VFUNC( void( __thiscall* )( void*, vec3_t& ), set_view_angles( vec3_t& angle ), 19, angle );
    VFUNC( bool( __thiscall* )( void* ), is_in_game( ), 26 );
    VFUNC( bool( __thiscall* )( void* ), is_connected( ), 27 );
    VFUNC( matrix4x4_t( __thiscall* )( void* ), world_to_screen_matrix( ), 37 );
    VFUNC( std::uintptr_t*( __thiscall* ) ( void* ), get_bsp_tree_query( ), 43 );
    VFUNC( void( __thiscall* )( void* ), fire_events( ), 59 );
    VFUNC( i_net_channel*( __thiscall* ) ( void* ), get_net_channel( ), 78 );
    VFUNC( c_steam_api_context*( __thiscall* ) ( void* ), get_steam_api_context( ), 185 );

    [[nodiscard]] matrix3x4_t& get_view_matrix( ) const {
        static auto view_matrix = *g_modules->m_client_dll.get_address( _( "CEngineClient::ScreenMatrix" ) ).offset( 0x3 ).as< std::uintptr_t* >( );
        static auto ptr = view_matrix + 176;
        return *reinterpret_cast< matrix3x4_t* >( ptr );
    }
};

#endif