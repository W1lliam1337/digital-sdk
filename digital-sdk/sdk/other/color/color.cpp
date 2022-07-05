#include "color.h"

c_color c_color::black(0, 0, 0, 255);
c_color c_color::white(255, 255, 255, 255);
c_color c_color::red(255, 0, 0, 255);
c_color c_color::green(0, 128, 0, 255);
c_color c_color::blue(0, 0, 255, 255);

c_color::c_color()
{
    *((int*)this) = 0;
}
c_color::c_color(int _r, int _g, int _b)
{
    set_color(_r, _g, _b, 255);
}
c_color::c_color(int _r, int _g, int _b, int _a)
{
    set_color(_r, _g, _b, _a);
}
void c_color::set_raw_color(int color32)
{
    *reinterpret_cast<int*>(this) = color32;
}
int c_color::get_raw_color() const
{
    return *((int*)this);
}

void c_color::set_alpha(int alpha) { _CColor[3] = alpha; }

void c_color::set_color(int _r, int _g, int _b, int _a)
{
    _CColor[0] = static_cast<unsigned char>(_r);
    _CColor[1] = static_cast<unsigned char>(_g);
    _CColor[2] = static_cast<unsigned char>(_b);
    _CColor[3] = static_cast<unsigned char>(_a);
}
void c_color::set_color(float _r, float _g, float _b, float _a)
{
    _CColor[0] = static_cast<unsigned char>(_r * 255.0f);
    _CColor[1] = static_cast<unsigned char>(_g * 255.0f);
    _CColor[2] = static_cast<unsigned char>(_b * 255.0f);
    _CColor[3] = static_cast<unsigned char>(_a * 255.0f);
}
void c_color::get_color(int& _r, int& _g, int& _b, int& _a) const
{
    _r = _CColor[0];
    _g = _CColor[1];
    _b = _CColor[2];
    _a = _CColor[3];
}
bool c_color::operator== (const c_color& rhs) const
{
    return (*((int*)this) == *((int*)&rhs));
}
bool c_color::operator!= (const c_color& rhs) const
{
    return !(operator==(rhs));
}
c_color& c_color::operator=(const c_color& rhs)
{
    set_raw_color(rhs.get_raw_color());
    return *this;
}
