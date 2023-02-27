#ifndef COLOR_H
#define COLOR_H

#include <iostream>

class c_color final {
public:
    c_color( );
    c_color( int _r, int _g, int _b );
    c_color( int _r, int _g, int _b, int _a );

    c_color( const float _r, const float _g, const float _b ) : c_color( _r, _g, _b, 1.0f ) {}

    c_color( const float _r, const float _g, const float _b, const float _a ) :
        c_color( static_cast< int >( _r * 255.0f ),
                 static_cast< int >( _g * 255.0f ),
                 static_cast< int >( _b * 255.0f ),
                 static_cast< int >( _a * 255.0f ) ) {}

    explicit c_color( float* rgb ) : c_color( rgb[ 0 ], rgb[ 1 ], rgb[ 2 ], rgb[ 3 ] ) {}

    explicit c_color( const unsigned long argb ) {
        c_color_[ 2 ] = static_cast< unsigned char >( ( argb & 0x000000FF ) >> ( 0 * 8 ) );
        c_color_[ 1 ] = static_cast< unsigned char >( ( argb & 0x0000FF00 ) >> ( 1 * 8 ) );
        c_color_[ 0 ] = static_cast< unsigned char >( ( argb & 0x00FF0000 ) >> ( 2 * 8 ) );
        c_color_[ 3 ] = static_cast< unsigned char >( ( argb & 0xFF000000 ) >> ( 3 * 8 ) );
    }

    void set_raw_color( int color32 );
    [[nodiscard]] int get_raw_color( ) const;
    void set_alpha( int alpha );
    void set_color( int _r, int _g, int _b, int _a = 0 );
    void set_color( float _r, float _g, float _b, float _a = 0 );
    void get_color( int& r, int& g, int& b, int& a ) const;

    [[nodiscard]] int r( ) const { return c_color_[ 0 ]; }
    [[nodiscard]] int g( ) const { return c_color_[ 1 ]; }
    [[nodiscard]] int b( ) const { return c_color_[ 2 ]; }
    [[nodiscard]] int a( ) const { return c_color_[ 3 ]; }

    unsigned char& operator[]( const int index ) { return c_color_[ index ]; }

    const unsigned char& operator[]( const int index ) const { return c_color_[ index ]; }

    bool operator==( const c_color& rhs ) const;
    bool operator!=( const c_color& rhs ) const;
    c_color& operator=( const c_color& rhs );

    static c_color from_hsb( float hue, float saturation, float brightness );
    unsigned int u32( );

    static c_color black;
    static c_color white;
    static c_color red;
    static c_color green;
    static c_color blue;
private:
    std::uint8_t c_color_[ 4 ]{ };
};

#endif