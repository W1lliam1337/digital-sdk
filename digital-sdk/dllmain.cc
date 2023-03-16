#include "context.hh"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved ) {
    if ( ul_reason_for_call != DLL_PROCESS_ATTACH )
        return FALSE;

    DisableThreadLibraryCalls( hModule );
    g_ctx->dllmain( ).create_thread( 
        g_ctx->dllmain( ).load, 
        hModule 
    );

    return TRUE;
}