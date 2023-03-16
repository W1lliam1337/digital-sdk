#include "misc.hh"

void c_misc::bhop( ) {
    if ( !( g_ctx->local( ).m_cmd->m_buttons & in_jump ) )
        return;

    c_player::get_local( )->flags( )& fl_on_ground ? g_ctx->local( ).m_cmd->m_buttons |= in_jump
                                                   : g_ctx->local( ).m_cmd->m_buttons &= ~in_jump;
}
