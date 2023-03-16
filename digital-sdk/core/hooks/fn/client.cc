#include "../../../features/features.hh"
#include "../../menu/menu.hh"
#include "../hooks.hh"

void c_hooks::client_t::init( ) {
    static const auto create_move_target = g_mem->get_virtual( g_interfaces->m_client, 22 );
    g_modules->m_client_dll.hook_function( _( "CHLClient::CreateMove" ), &create_move_wrapper, create_move_target );
}

// module: client.dll; sig: 55 8B EC 83 EC 18 56 57 8B F9 8B 4D 04 E8 ?? ?? ?? ?? 8B 4D
void __stdcall c_hooks::client_t::create_move( const int sequence, const float frame_time, const bool is_active,
                                               bool& send_packet ) {
    static const auto og = g_modules->m_client_dll.get< void( __stdcall* )( int, float, bool ) >( _( "CHLClient::CreateMove" ) );
    og( sequence, frame_time, is_active );

    g_ctx->local( ).m_in_move = true;

    const auto& cmd = g_interfaces->m_input->get_user_cmd( sequence );
    const auto& ver_cmd = g_interfaces->m_input->get_verified_cmd( sequence );

    if ( !cmd || !ver_cmd )
        return;

    g_ctx->local( ).m_cmd = cmd;
    g_ctx->local( ).m_send_packet = send_packet = true;

    if ( g_menu->opened( ) ) {
        cmd->m_buttons &= ~( in_attack | in_attack2 );
    }

    const auto on_move = [ & ] {
        // input here ur legit, rage stf..
    };

    g_eng_pred->update( );
    g_misc->bhop( );
    g_eng_pred->predict( on_move );

    static const auto md5 =
        g_modules->m_client_dll.get_address( _( "MD5_PseudoRandom" ) ).as< int( __cdecl* )( int ) >( );
    cmd->m_random_seed = md5( sequence ) & 0x7fffffff;

    send_packet = g_ctx->local( ).m_send_packet;
    ver_cmd->m_cmd = *cmd;
    ver_cmd->m_crc = cmd->get_checksum( );
}

DECLSPEC_NAKED void __stdcall c_hooks::client_t::create_move_wrapper( int sequence, float frame_time, bool is_active ) {
    __asm
    {
		push ebp
		mov ebp, esp
		push ebx
		lea ecx, [ esp ]
		push ecx // send packet
		push dword ptr[ ebp + 10h ] // is active
		push dword ptr[ ebp + 0Ch ] // frame time
		push dword ptr[ ebp + 8 ] // sequence
		call create_move
		pop ebx
		pop ebp
		retn 0Ch
    }
}