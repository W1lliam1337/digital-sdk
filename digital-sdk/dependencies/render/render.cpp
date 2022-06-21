#include "render.h"
#include "../../sdk/sdk.hpp"

void c_render::init() const
{
	static auto create_font = [](const char* name, const int size, const int weight, const DWORD flags) -> vgui::h_font
	{
		const auto font = g_sdk.m_interfaces.m_surface->font_create();
		g_sdk.m_interfaces.m_surface->set_font_glyph(font, name, size, weight, NULL, NULL, flags);

		return font;
	};

	g_sdk.m_fonts.m_esp = create_font(_("Verdana"), 12, FW_NORMAL, fontflag_antialias | fontflag_dropshadow);
}

void c_render::line(const vector_2d& v0, const vector_2d& v1, const color color) {
	g_sdk.m_interfaces.m_surface->set_drawing_color(color);
	g_sdk.m_interfaces.m_surface->draw_line(v0.x, v0.y, v1.x, v1.y);
}

void c_render::line(const int x0, const int y0, const int x1, const int y1, const color color) {
	g_sdk.m_interfaces.m_surface->set_drawing_color(color);
	g_sdk.m_interfaces.m_surface->draw_line(x0, y0, x1, y1);
}

void c_render::rect(const int x, const int y, const int w, const int h, const color color) {
	g_sdk.m_interfaces.m_surface->set_drawing_color(color);
	g_sdk.m_interfaces.m_surface->draw_outlined_rect(x, y, x + w, y + h);
}

void c_render::rect_filled(const int x, const int y, const int w, const int h, const color color) {
	g_sdk.m_interfaces.m_surface->set_drawing_color(color);
	g_sdk.m_interfaces.m_surface->draw_filled_rectangle(x, y, x + w, y + h);
}

void c_render::rect_filled_fade(const int x, const int y, const int w, const int h, const color color, const int a1, const int a2) {
	g_sdk.m_interfaces.m_surface->set_drawing_color(color);
	g_sdk.m_interfaces.m_surface->draw_filled_rect_fade(x, y, x + w, y + h, a1, a2, false);
}

void c_render::rect_outlined(const int x, const int y, const int w, const int h, const color color1, const color color2) {
	rect(x, y, w, h, color1);
	rect(x - 1, y - 1, w + 2, h + 2, color2);
	rect(x + 1, y + 1, w - 2, h - 2, color2);
}

void c_render::gradient(const int x, const int y, const int w, const int h, const color color1, const color color2) {
	g_sdk.m_interfaces.m_surface->set_drawing_color(color1);
	g_sdk.m_interfaces.m_surface->draw_filled_rect_fade(x, y, x + w, y + h, color1.a(), 0, false);

	g_sdk.m_interfaces.m_surface->set_drawing_color(color2);
	g_sdk.m_interfaces.m_surface->draw_filled_rect_fade(x, y, x + w, y + h, 0, color2.a(), false);
}

void c_render::text(const vgui::h_font font, int x, int y, const color color, DWORD flags, const char* msg, ...)
{
	va_list va_alist;
	char buffer[1024];
	va_start(va_alist, msg);
	_vsnprintf(buffer, sizeof(buffer), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];

	MultiByteToWideChar(CP_UTF8, 0, buffer, 256, wbuf, 256);

	int width, height;
	g_sdk.m_interfaces.m_surface->get_text_size(font, wbuf, width, height);

	if (!(flags & hfont_centered_none))
	{
		if (flags & hfont_centered_x)
			x -= width * 0.5f;

		if (flags & hfont_centered_y)
			y -= height * 0.5f;
	}

	g_sdk.m_interfaces.m_surface->draw_text_font(font);
	g_sdk.m_interfaces.m_surface->set_drawing_color(color);
	g_sdk.m_interfaces.m_surface->draw_text_pos(x, y);
	g_sdk.m_interfaces.m_surface->draw_render_text(wbuf, wcslen(wbuf));
}