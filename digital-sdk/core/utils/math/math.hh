#ifndef MATH_H
#define MATH_H

#include "matrix.hh"
#include "utl.hh"
#include "vector.hh"
#include <memory>

class c_math final {
public:

};

inline const std::unique_ptr< c_math > g_math{ new c_math( ) };

#endif