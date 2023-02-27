#ifndef PATTERN_H
#define PATTERN_H

#include "address.hh"

#include <memory>
#include <Windows.h>
#include <string>

class c_module_info {
public:
    c_module_info( ) = default;

    c_module_info( const std::size_t loaded_module ) :
        m_loaded_module{ loaded_module } {
        m_bitmap = reinterpret_cast< std::uint8_t* >( m_loaded_module );
        m_dos_header = reinterpret_cast< PIMAGE_DOS_HEADER >( m_loaded_module );
        m_base = m_dos_header->e_lfanew;
        m_nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( m_loaded_module + m_base );
        m_size = m_nt_headers->OptionalHeader.SizeOfImage;
    }

    [[nodiscard]] std::size_t get_module( ) const {
        return m_loaded_module;
    }
protected:
    std::size_t m_loaded_module{ };
    std::uint8_t* m_bitmap{ };
    PIMAGE_DOS_HEADER m_dos_header{ };
    long m_base{ };
    PIMAGE_NT_HEADERS m_nt_headers{ };
    unsigned long m_size{ };
};

class c_pattern : public c_module_info {
    static std::size_t build_byte_array( const char* pattern, int* bytes );
    c_address search_byte_array( const int* bytes, std::size_t size ) const;
public:
    c_pattern( ) = default;

    c_pattern( const std::size_t loaded_module ) :
        c_module_info( loaded_module ) {
    };

    [[nodiscard]] c_address find( const std::string_view& pattern, const std::string_view& name ) const;
};

#endif