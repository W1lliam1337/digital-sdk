#include "color.hh"

c_color c_color::black( 0, 0, 0, 255 );
c_color c_color::white( 255, 255, 255, 255 );
c_color c_color::red( 255, 0, 0, 255 );
c_color c_color::green( 0, 128, 0, 255 );
c_color c_color::blue( 0, 0, 255, 255 );

c_color::c_color( ) {
    *reinterpret_cast< int* >( this ) = 0;
}

c_color::c_color( const int _r, const int _g, const int _b ) {
    set_color( _r, _g, _b, 255 );
}

c_color::c_color( const int _r, const int _g, const int _b, const int _a ) {
    set_color( _r, _g, _b, _a );
}

void c_color::set_raw_color( const int color32 ) {
    *reinterpret_cast< int* >( this ) = color32;
}

int c_color::get_raw_color( ) const {
    return *( int* ) this;
}

void c_color::set_alpha( const int alpha ) {
    c_color_[ 3 ] = alpha;
}

void c_color::set_color( const int _r, const int _g, const int _b, const int _a ) {
    c_color_[ 0 ] = static_cast< unsigned char >( _r );
    c_color_[ 1 ] = static_cast< unsigned char >( _g );
    c_color_[ 2 ] = static_cast< unsigned char >( _b );
    c_color_[ 3 ] = static_cast< unsigned char >( _a );
}

void c_color::set_color( const float _r, const float _g, const float _b, const float _a ) {
    c_color_[ 0 ] = static_cast< unsigned char >( _r * 255.0f );
    c_color_[ 1 ] = static_cast< unsigned char >( _g * 255.0f );
    c_color_[ 2 ] = static_cast< unsigned char >( _b * 255.0f );
    c_color_[ 3 ] = static_cast< unsigned char >( _a * 255.0f );
}

void c_color::get_color( int& r, int& g, int& b, int& a ) const {
    r = c_color_[ 0 ];
    g = c_color_[ 1 ];
    b = c_color_[ 2 ];
    a = c_color_[ 3 ];
}

bool c_color::operator==( const c_color& rhs ) const {
    return *( int* ) this == *( int* ) &rhs;
}

bool c_color::operator!=( const c_color& rhs ) const {
    return !operator==( rhs );
}

c_color& c_color::operator=( const c_color& rhs ) {
    set_raw_color( rhs.get_raw_color( ) );
    return *this;
}

c_color c_color::from_hsb( const float hue, const float saturation, const float brightness ) {
    const float h = hue == 1.0f ? 0 : hue * 6.0f;
    const float f = h - static_cast< int >( h );
    const float p = brightness * ( 1.0f - saturation );
    const float q = brightness * ( 1.0f - saturation * f );
    const float t = brightness * ( 1.0f - ( saturation * ( 1.0f - f ) ) );

    if ( h < 1 ) {
        return { static_cast< unsigned char >( brightness * 255 ), static_cast< unsigned char >( t * 255 ),
                 static_cast< unsigned char >( p * 255 ) };
    }
    if ( h < 2 ) {
        return { static_cast< unsigned char >( q * 255 ), static_cast< unsigned char >( brightness * 255 ),
                 static_cast< unsigned char >( p * 255 ) };
    }
    if ( h < 3 ) {
        return { static_cast< unsigned char >( p * 255 ), static_cast< unsigned char >( brightness * 255 ),
                 static_cast< unsigned char >( t * 255 ) };
    }
    if ( h < 4 ) {
        return { static_cast< unsigned char >( p * 255 ), static_cast< unsigned char >( q * 255 ),
                 static_cast< unsigned char >( brightness * 255 ) };
    }
    if ( h < 5 ) {
        return { static_cast< unsigned char >( t * 255 ), static_cast< unsigned char >( p * 255 ),
                 static_cast< unsigned char >( brightness * 255 ) };
    }
    return { static_cast< unsigned char >( brightness * 255 ), static_cast< unsigned char >( p * 255 ),
             static_cast< unsigned char >( q * 255 ) };
}

unsigned int c_color::u32( ) {
    return ( ( c_color_[ 3 ] & 0xff ) << 24 ) + ( ( c_color_[ 2 ] & 0xff ) << 16 ) + ( ( c_color_[ 1 ] & 0xff ) << 8 )
        + ( c_color_[ 0 ] & 0xff );
}