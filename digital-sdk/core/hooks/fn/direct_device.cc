#include "../hooks.hh"
#include "../../../features/features.hh"
#include "../../menu/menu.hh"
#include "../../render/render.hh"

void c_hooks::direct_device_t::init( ) {
    static const auto end_scene_target = g_mem->get_virtual( g_interfaces->m_device, 42 );
    g_modules->m_shaderapidx9_dll.hook_function( _( "DirectDevice::EndScene" ), &end_scene, end_scene_target );

    static const auto reset_target = g_mem->get_virtual( g_interfaces->m_device, 16 );
    g_modules->m_shaderapidx9_dll.hook_function( _( "DirectDevice::Reset" ), &reset, reset_target );
}

long c_hooks::direct_device_t::end_scene( IDirect3DDevice9* device ) {
    static const auto og = g_modules->m_shaderapidx9_dll.get<decltype( &end_scene )>( _( "DirectDevice::EndScene" ) );

    DWORD colorwrite, srgbwrite;
    IDirect3DVertexDeclaration9* vert_dec = nullptr;
    IDirect3DVertexShader9* vert_shader = nullptr;

    device->GetRenderState( D3DRS_COLORWRITEENABLE, &colorwrite );
    device->GetRenderState( D3DRS_SRGBWRITEENABLE, &srgbwrite );
    device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );
    device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, FALSE );
    device->SetRenderState( D3DRS_COLORWRITEENABLE, 0xffffffff );
    device->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
    device->GetVertexDeclaration( &vert_dec );
    device->GetVertexShader( &vert_shader );
    device->SetRenderState( D3DRS_COLORWRITEENABLE, 0xffffffff );
    device->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
    device->SetSamplerState( NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
    device->SetSamplerState( NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
    device->SetSamplerState( NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
    device->SetSamplerState( NULL, D3DSAMP_SRGBTEXTURE, NULL );

    ImGui_ImplDX9_NewFrame( );
    ImGui_ImplWin32_NewFrame( );
    ImGui::NewFrame( );

    g_render->draw_list( ) = ImGui::GetBackgroundDrawList( );
    g_esp->init( );
    g_menu->render( );

    ImGui::EndFrame( );
    ImGui::Render( );
    ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );

    device->SetRenderState( D3DRS_COLORWRITEENABLE, colorwrite );
    device->SetRenderState( D3DRS_SRGBWRITEENABLE, srgbwrite );
    device->SetVertexDeclaration( vert_dec );
    device->SetVertexShader( vert_shader );

    return og( device );
}

long __stdcall c_hooks::direct_device_t::reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* device_parameters ) {
    static const auto og = g_modules->m_shaderapidx9_dll.get<decltype( &reset )>( _( "DirectDevice::Reset" ) );

    ImGui_ImplDX9_InvalidateDeviceObjects( );
    const auto result = og( device, device_parameters );
    ImGui_ImplDX9_CreateDeviceObjects( );

    return result;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT __stdcall c_hooks::direct_device_t::wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
    if ( g_menu->opened( ) ) {
        ImGui_ImplWin32_WndProcHandler( wnd, msg, wparam, lparam );

        if ( ImGui::GetIO( ).WantTextInput )
            return true;
    }

    if ( wparam == VK_INSERT && msg == WM_KEYUP ) {
        g_menu->opened( ) = !g_menu->opened( );
    }

    return CallWindowProc( reinterpret_cast<WNDPROC>( g_hooks->wnd_proc( ) ), wnd, msg, wparam, lparam );
}