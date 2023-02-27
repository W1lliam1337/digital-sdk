#ifndef CONSOLE_H
#define CONSOLE_H

#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <windows.h>

#define log_print( ... ) g_console->print( __VA_ARGS__ )

class c_console final {
    FILE* m_stream{ };
public:
    void attach( );
    void detach( ) const;

    template < typename... args_t > void print( const std::string_view sz_text, const args_t&... args ) {
        std::cout << std::vformat( sz_text, std::make_format_args( args... ) ) << std::endl;
    }
};

inline const std::unique_ptr< c_console > g_console{ new c_console( ) };

#endif