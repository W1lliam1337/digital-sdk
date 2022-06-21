#pragma once
#include "../singleton/singleton.h"
#include "../color/color.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>

#include "font.h"
#include "../../sdk/interfaces/classes/i_surface.h"
#include "../obfuscation/xor.h"
#include "../../sdk/math/math.h"

class vector_2d;
class vec3_t;

enum e_font_centering_flags
{
	hfont_centered_none = (1 << 0),
	hfont_centered_x = (1 << 1),
	hfont_centered_y = (1 << 2)
};

class c_render : public c_singleton<c_render>
{
public:
	vgui::h_font m_esp;

	void init() const;
	static void line(const vector_2d& v0, const vector_2d& v1, color color);
	static void line(int x0, int y0, int x1, int y1, color color);
	static void rect(int x, int y, int w, int h, color color);
	static void rect_filled(int x, int y, int w, int h, color color);
	static void rect_filled_fade(int x, int y, int w, int h, color color, int a1, int a2);
	static void rect_outlined(int x, int y, int w, int h, color color1, color color2);
	void circle(int x, int y, int radius, int segments, color color);
	static void gradient(int x, int y, int w, int h, color color1, color color2);
	static void text(vgui::h_font font, int x, int y, color color, DWORD flags, const char* msg, ...);
};
