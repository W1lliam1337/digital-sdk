#ifndef MISC_H
#define MISC_H

#include "../../context.hh"

class c_misc final {
public:
    static void bhop( );
};

inline const std::unique_ptr< c_misc > g_misc{ new c_misc( ) };

#endif