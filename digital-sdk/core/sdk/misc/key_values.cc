#include "key_values.hh"
#include "../interfaces/interfaces.hh"

c_key_values::c_key_values( const char* key_name, void* unk, void* unk2 ) {
    static const auto fn = g_modules->m_client_dll.get_address( _( "CKeyValues::Init" ) )
                               .as< 
        void( __thiscall* )( void*, const char*, void*, void* ) >( );
    fn( this, key_name, unk, unk2 );
}

void c_key_values::load_from_buffer( const char* resource_name, const char* buf, void* file_sys, const char* path_id,
                                     void* eval_sym_proc, void* unk ) {
    static const auto fn = g_modules->m_client_dll.get_address( _( "CKeyValues::LoadFromBuffer" ) ).as< 
        void( __thiscall* )( void*, const char*, const char*, void*, const char*, void*, void* ) >( );
    return fn( this, resource_name, buf, file_sys, path_id, eval_sym_proc, unk );
}

const char* c_key_values::get_name( ) {
    const auto offset = *reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uintptr_t >( this ) + 0x3 );
    const auto offset2 = *reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uintptr_t >( this ) + 0x12 );
    return g_interfaces->m_key_values_system->get_string_for_symbol( offset | ( offset2 << 8 ) );
}