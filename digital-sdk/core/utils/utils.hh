#ifndef UTILS_INCLUDES
#define UTILS_INCLUDES

#include "color/color.hh"
#include "console/console.hh"
#include "math/math.hh"
#include "memory/memory.hh"
#include "pe/pe.hh"
#include <ShlObj.h>

struct ImFontConfig;
struct ImFont;

class c_utils final {
public:
    static std::wstring get_fonts_folder_path( );
    static ImFont* add_font_from_file( const std::wstring& filename, float font_size, ImFontConfig cfg );
};

inline const std::unique_ptr< c_utils > g_utils{ new c_utils( ) };

#endif