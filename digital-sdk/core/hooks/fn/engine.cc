#include "../hooks.hh"
#include "../../../features/features.hh"

void c_hooks::engine_t::init( ) {
	g_modules->m_engine_dll.hook_function( _( "CL_Move" ), &cl_move );
}

void __vectorcall c_hooks::engine_t::cl_move( const float accumulated_extra_samples, const bool final_tick ) {
	static const auto og = g_modules->m_engine_dll.get< decltype( &cl_move ) >( _( "CL_Move" ) );

	// why not just make wrapper for createmove and change ebx in it?
	/*if ( g_ctx->m_in_move ) {
		// B3 01 mov bl, 1
		static const auto& send_packet_sig = g_modules->m_engine_dll.get_address( _( "CL_Move::SendPacket" ) ).add( 0x1 ).as<std::uintptr_t*>( );

		DWORD protect{};

		VirtualProtect( send_packet_sig, 4, PAGE_EXECUTE_READWRITE, &protect );
		*reinterpret_cast< bool* >( send_packet_sig ) = g_ctx->m_send_packet;
		VirtualProtect( send_packet_sig, 4, protect, &protect );
	}*/

	return og( accumulated_extra_samples, final_tick );
}