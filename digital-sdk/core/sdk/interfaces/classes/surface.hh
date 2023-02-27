#ifndef VALVE_SURFACE
#define VALVE_SURFACE

#include "../../../utils/memory/memory.hh"
#include "../../modules/modules.hh"

class i_surface {
public:
    VFUNC( void( __thiscall* )( void* ), unlock_cursor( ), 66 );

    void start_drawing( ) {
        static const auto fn = g_modules->m_vguimatsurface_dll.get_address( _( "CMatSystemSurface::StartDrawing" ) ).as< 
            void( __thiscall* )( void* ) >( );
        return fn( this );
    }

    void finish_drawing( ) {
        static const auto fn = g_modules->m_vguimatsurface_dll.get_address( _( "CMatSystemSurface::FinishDrawing" ) ).as< 
            void( __thiscall* )( void* ) >( );
        return fn( this );
    }
};

#endif