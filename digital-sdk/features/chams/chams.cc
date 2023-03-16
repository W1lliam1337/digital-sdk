#include "chams.hh"

void c_chams::init_material( ) {
    m_regular = create_material( _( "digital_regular.vmt" ), _( "VertexLitGeneric" ), _( R"#("VertexLitGeneric" {
			"$basetexture" "vgui/white_additive"
			"$ignorez"      "0"
			"$model"		"1"
			"$flat"			"0"
			"$nocull"		"1"
			"$halflambert"	"1"
			"$nofog"		"1"
			"$wireframe"	"0"
		})#" ) );
}

i_material* c_chams::create_material( const std::string_view material_name, const std::string_view shader_type,
                                      const std::string_view material_data ) {
    const auto key_values = new c_key_values( shader_type.data( ) );
    key_values->load_from_buffer( material_name.data( ), material_data.data( ) );

    return g_interfaces->m_material_system->create_material( material_name.data( ), key_values );
}

void c_chams::override_material( const c_color& clr, const bool ignorez ) const {
    const auto material = m_regular;
    if ( !material )
        return;

    material->set_material_var_flag( material_var_ignorez, ignorez ? true : false );

    material->alpha_modulate( clr.a( ) / 255.f );
    material->color_modulate( clr.r( ) / 255.f, clr.g( ) / 255.f, clr.b( ) / 255.f );
    material->increment_reference_count( );

    if ( const auto envmaptint = material->find_var( _( "$envmaptint" ), nullptr, false ) ) {
        envmaptint->set_vector( vec3_t( clr.r( ) / 255.f, clr.g( ) / 255.f, clr.b( ) / 255.f ) );
    }

    g_interfaces->m_model_render->force_material( material );
}

bool c_chams::draw_model( i_model_render* const model_render, void* edx, void* context, draw_model_state_t& state,
                          c_model_render_info& info, matrix3x4_t* bones ) const {
    static const auto og = g_modules->m_engine_dll.get< void( __fastcall* )(
        i_model_render*, void*, void*, draw_model_state_t&, c_model_render_info&, matrix3x4_t* ) >(
        _( "CModelRender::DME" ) );
    const auto entity =
        reinterpret_cast< c_entity* const >( g_interfaces->m_entity_list->get_client_entity( info.m_entity_index ) );
    if ( !entity )
        return true;

    const auto player = reinterpret_cast< c_player* const >( entity );
    if ( !player )
        return true;

    // get the player model
    const bool is_player = std::strstr( info.m_model->m_name, _( "models/player" ) );

    // works only for the enemy, remake it for yourself
    if ( is_player && player->is_player( ) && player != c_player::get_local( )
         && player->team( ) != c_player::get_local( )->team( ) ) {
        override_material( c_color( 154, 205, 50 ), true );
        og( model_render, edx, context, state, info, bones );
        override_material( c_color( 255, 215, 0 ), false );

        return false;
    }

	// @note: call the original so as not to break the game (models)
	return true;  
}
