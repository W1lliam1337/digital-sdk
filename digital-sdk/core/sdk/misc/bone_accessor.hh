#ifndef VALVE_BONE_ACCESSOR
#define VALVE_BONE_ACCESSOR

#include "../../utils/math/matrix.hh"

class c_bone_accessor {
public:
    alignas( 16 ) matrix3x4_t* m_bones{ };
    int m_readable_bones{ }, m_writable_bones{ };
};

#endif