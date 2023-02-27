#include "context.hh"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved ) {
    if ( ul_reason_for_call != DLL_PROCESS_ATTACH )
        return FALSE;

    DisableThreadLibraryCalls( hModule );
    g_ctx->m_dllmain.create_thread( 
        g_ctx->m_dllmain.load, 
        hModule 
    );

    return TRUE;
}