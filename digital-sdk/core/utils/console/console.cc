#include "console.hh"
#include "../../../deps/xor/xor.hh"

void c_console::attach( ) {
    AllocConsole( );
    AttachConsole( ATTACH_PARENT_PROCESS );
    freopen_s( &m_stream, _( "CONOUT$" ), _( "w" ), stdout );
    SetConsoleTitleA( _( "digital-sdk" ) );
}

void c_console::detach( ) const {
    fclose( m_stream );
    FreeConsole( );
    PostMessageW( GetConsoleWindow( ), WM_CLOSE, 0, 0 );
}