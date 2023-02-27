#include "pe.hh"
#include "../console/console.hh"

bool c_pe::init( ) {
    auto peb = reinterpret_cast< _PEB* >( __readfsdword( 0x30 ) );
    if ( !peb )
        return false;

    auto list = &peb->Ldr->InMemoryOrderModuleList;
    for ( auto it = list->Flink; it != list; it = it->Flink ) {
        const auto ldr_entry = CONTAINING_RECORD( it, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks );
        if ( !ldr_entry )
            continue;

        auto base_dll_name = reinterpret_cast< UNICODE_STRING* >( &ldr_entry->Reserved4[ 0 ] );
        auto wide_name = std::wstring( base_dll_name->Buffer, base_dll_name->Length >> 1 );
        auto name = std::string( wide_name.begin( ), wide_name.end( ) );

        m_loaded_modules[ fnv1a::hash( name ) ] = reinterpret_cast< std::size_t >( ldr_entry->DllBase );
        log_print( "found module {} -> {}", name.data( ), m_loaded_modules[ fnv1a::hash( name ) ] );
    }

    log_print( "pe passed." );
    return true;
}

std::size_t c_pe::export_fn( const std::size_t base, const std::size_t fn_hash, const bool in_memory ) const {
    const auto rva_2_offset = [ & ]( const std::uint32_t rva, PIMAGE_NT_HEADERS nt_headers,
                                     const bool in_mem = false ) {
        if ( !rva || !in_mem )
            return static_cast< std::int32_t >( rva );

        auto sec = IMAGE_FIRST_SECTION( nt_headers );
        for ( size_t i = 0; i < nt_headers->FileHeader.NumberOfSections; i++ ) {
            if ( rva >= sec->VirtualAddress && rva < sec->VirtualAddress + sec->Misc.VirtualSize )
                break;
            sec++;
        }

        return static_cast< std::int32_t >( rva - sec->VirtualAddress + sec->PointerToRawData );
    };

    const auto dos_headers = reinterpret_cast< PIMAGE_DOS_HEADER >( base );
    if ( dos_headers->e_magic != IMAGE_DOS_SIGNATURE )
        return 0;

    const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( base + dos_headers->e_lfanew );
    if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
        return 0;

    const auto exports = reinterpret_cast< IMAGE_EXPORT_DIRECTORY* >(
        rva_2_offset( base + nt_headers->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress,
                      nt_headers, in_memory ) );

    const auto names = reinterpret_cast< std::uint32_t* >( 
        base + rva_2_offset( exports->AddressOfNames, nt_headers, in_memory ) );

    auto ordinal_index = static_cast< std::uint32_t >( -1 );

    for ( std::uint32_t i = 0; i < exports->NumberOfFunctions; i++ ) {
        const auto function_name =
            reinterpret_cast< const char* >( base + rva_2_offset( names[ i ], nt_headers, in_memory ) );

        if ( fnv1a::hash( function_name ) == fn_hash ) {
            ordinal_index = i;
            break;
        }
    }

    if ( ordinal_index > exports->NumberOfFunctions )
        return 0;

    const auto ordinals = reinterpret_cast< std::uint16_t* >(
        base + rva_2_offset( exports->AddressOfNameOrdinals, nt_headers, in_memory ) );

    const auto addresses = reinterpret_cast< std::uint32_t* >( 
        base + rva_2_offset( exports->AddressOfFunctions, nt_headers, in_memory ) );

    return base + rva_2_offset( addresses[ ordinals[ ordinal_index ] ], nt_headers, in_memory );
}