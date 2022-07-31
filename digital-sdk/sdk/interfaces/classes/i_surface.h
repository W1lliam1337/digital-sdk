#pragma once
#include "../../../utils/utils.h"
#include "../../other/color/color.h"

namespace vgui
{
	using h_font = unsigned long;
	using vpanel = unsigned int;
}

class i_surface
{
public:
	unsigned long font_create()
	{
		return utils::call_vfunc<unsigned int(__thiscall*)(void*)>(this, 71)(this);
	}

	void set_font_glyph(const unsigned long font, const char* windowsFontName, const int tall, const int weight,
	                    const int blur, const int scanlines, const int flags)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, unsigned long, const char*, int, int, int, int, int, int,
		                                            int)>(this, 72)(this, font, windowsFontName, tall, weight, blur,
		                                                            scanlines, flags, 0, 0);
	}

	void unlock_cursor()
	{
		return utils::call_vfunc<void(__thiscall*)(void*)>(this, 66)(this);
	}

	void draw_filled_rect_fade(const int x, const int y, const int w, const int h, const unsigned int alpha1,
	                           const unsigned int alpha2, const bool is_horizontal)
	{
		return utils::call_vfunc<void(__thiscall*
		)(void*, int, int, int, int, unsigned int, unsigned int, bool)>(this, 123)(
			this, x, y, w, h, alpha1, alpha2, is_horizontal);
	}

	void set_drawing_color(const int r, const int g, const int b, const int a = 255)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 15)(this, r, g, b, a);
	}

	void set_drawing_color(const c_color col)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 15)(
			this, col.r(), col.g(), col.b(), col.a());
	}

	void draw_outlined_rect(const int x, const int y, const int w, const int h)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 18)(this, x, y, w, h);
	}

	void draw_line(const int x1, const int y1, const int x2, const int y2)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 19)(this, x1, y1, x2, y2);
	}

	void draw_filled_rectangle(const int x, const int y, const int w, const int h)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 16)(this, x, y, w, h);
	}

	void get_text_size(const unsigned long font, const wchar_t* text, int& wide, int& tall)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, unsigned long, const wchar_t*, int&, int&)>(this, 79)(
			this, font, text, wide, tall);
	}

	void draw_text_font(const unsigned long font)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, unsigned long)>(this, 23)(this, font);
	}

	void set_text_color(const c_color col)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, int, int, int, int)>(this, 25)(
			this, col.r(), col.g(), col.b(), col.a());
	}

	void draw_text_pos(const int x, const int y)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, int, int)>(this, 26)(this, x, y);
	}

	void draw_render_text(const wchar_t* text, const int text_len)
	{
		return utils::call_vfunc<void(__thiscall*
		)(void*, const wchar_t*, int, int)>(this, 28)(this, text, text_len, 0);
	}
};
