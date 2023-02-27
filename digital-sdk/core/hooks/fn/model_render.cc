#include "../hooks.hh"
#include "../../../features/features.hh"

void c_hooks::model_render_t::init( ) {
    static const auto model_render_target = g_mem->get_virtual( g_interfaces->m_model_render, 21 );
    g_modules->m_engine_dll.hook_function( _( "CModelRender::DME" ), &dme, model_render_target );

}

void __fastcall c_hooks::model_render_t::dme( i_model_render* const ecx,
                                   void* edx,
                                   void* ctx,
                                   draw_model_state_t& state,
                                   c_model_render_info& info,
                                   matrix3x4_t* custom_bone_to_world ) {
    static const auto og = g_modules->m_engine_dll.get< decltype( &dme ) >( _( "CModelRender::DME" ) );
    if ( !c_player::get_local( ) || ecx->is_forced_material( ) )
        return og( ecx, edx, ctx, state, info, custom_bone_to_world );

    if ( g_chams->draw_model( ecx, edx, ctx, state, info, custom_bone_to_world ) )
        return og( ecx, edx, ctx, state, info, custom_bone_to_world );

    ecx->force_material( nullptr );
}