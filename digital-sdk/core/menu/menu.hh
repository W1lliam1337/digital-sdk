#ifndef MENU_H
#define MENU_H

#include "../../context.hh"

class c_menu final {
    int m_selected_tab{ };
    bool m_opened{ };
public:
    void render( );
    auto& opened( ) { return m_opened; }
};

inline const std::unique_ptr< c_menu > g_menu{ new c_menu( ) };

#endif