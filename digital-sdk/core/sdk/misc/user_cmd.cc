#include "user_cmd.hh"
#include "../modules/modules.hh"

std::uint32_t c_user_cmd::get_checksum( ) const {
    static const auto sig = g_modules->m_client_dll.get_address( _( "CUserCmd::CheckSum" ) ).as< 
        std::uint32_t( __thiscall* )( const void* ) >( );
    return sig( this );
}