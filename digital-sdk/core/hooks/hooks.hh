#ifndef HOOKS_H
#define HOOKS_H

#include "../../context.hh"

#define REGS void* ecx, void* edx

#if defined( __clang__ )
#define DECLSPEC_NAKED __attribute__( ( naked ) )
#elif defined( _MSC_VER )
#define DECLSPEC_NAKED __declspec( naked )
#endif

class c_hooks final {
    struct client_t {
        static void init( );
        static void __stdcall create_move( int sequence, float frame_time, bool is_active, bool& send_packet );
        static void __stdcall create_move_wrapper( int sequence, float frame_time, bool is_active );
    } m_client{ };

    struct engine_t {
        static void init( );
        static void __vectorcall cl_move( float accumulated_extra_samples, bool final_tick );
    } m_engine{ };

    struct client_ent_list_t {
        static void init( );
        static void __fastcall on_add_entity( REGS, c_handle_entity* entity, c_base_handle handle );
        static void __fastcall on_remove_entity( REGS, c_handle_entity* entity, c_base_handle handle );
    } m_client_ent_list{ };

    struct direct_device_t {
        static void init( );
        static long WINAPI end_scene( IDirect3DDevice9* device );
        static long __stdcall reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* device_parameters );
        static LRESULT __stdcall wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
        LONG m_wnd_proc{ };
    } m_direct_device{ };

    struct surface_t {
        static void init( );
        static void __fastcall lock_cursor( i_surface* ecx, void* edx );
    } m_surface{ };

    struct model_render_t {
        static void init( );
        static void __fastcall dme( i_model_render* ecx,
                                    void* edx,
                                    void* ctx,
                                    draw_model_state_t& state,
                                    c_model_render_info& info,
                                    matrix3x4_t* custom_bone_to_world );
    } m_model_render{ };
public:
    void init( ) const;
    static void unhook( );

    void set_wnd_proc( ) {
        m_direct_device.m_wnd_proc = SetWindowLongA( g_ctx->hwnd( ), 
                            GWLP_WNDPROC, 
                            reinterpret_cast< LONG_PTR >( m_direct_device.wnd_proc ) );
    }

    auto& wnd_proc( ) { return m_direct_device.m_wnd_proc; }
};

inline const std::unique_ptr< c_hooks > g_hooks{ new c_hooks( ) };

#endif