#ifndef RAY_H
#define RAY_H

#include "../../utils/math/vector.hh"
#include "../../utils/math/matrix.hh"

struct ray_t {
    vector_aligned_t m_start{ };        // starting point, centered within the extents
    vector_aligned_t m_delta{ };        // direction + length of the ray
    vector_aligned_t m_start_offset{ }; // Add this to m_Start to Get the actual ray start
    vector_aligned_t m_extents{ };      // Describes an axis aligned box extruded along a ray
    const matrix3x4_t* m_world_axis_transform{ };
    bool m_is_ray{ };   // are the extents zero?
    bool m_is_swept{ }; // is delta != 0?

    ray_t( ) : m_world_axis_transform( nullptr ) {}

    ray_t( const vec3_t& start, const vec3_t& end ) { init( start, end ); }

    ray_t( const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& maxs ) :
        m_start{ start + ( mins + maxs ) * 0.5f },
        m_delta{ end - start },
        m_start_offset{ ( mins + maxs ) * -0.5f },
        m_extents{ ( maxs - mins ) * 0.5f },
        m_is_ray{ m_extents.length_sqr( ) < 1e-6 },
        m_is_swept{ m_delta.length_sqr( ) != 0.f } {}

    void init( const vec3_t& start, const vec3_t& end ) {
        m_delta = end - start;

        m_is_swept = ( m_delta.length_sqr( ) != 0 );

        m_extents.init( );

        m_world_axis_transform = nullptr;
        m_is_ray = true;

        // Offset m_Start to be in the center of the box...
        m_start_offset.init( );
        m_start = start;
    }

    void init( const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& maxs ) {
        m_delta = end - start;

        m_world_axis_transform = nullptr;
        m_is_swept = ( m_delta.length_sqr( ) != 0 );

        m_extents = maxs - mins;
        m_extents *= 0.5f;
        m_is_ray = ( m_extents.length_sqr( ) < 1e-6 );

        // Offset m_Start to be in the center of the box...
        m_start_offset = maxs + mins;
        m_start_offset *= 0.5f;
        m_start = start + m_start_offset;
        m_start_offset *= -1.0f;
    }
};

#endif