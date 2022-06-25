#include "render.h"
#include "../../sdk/sdk.hpp"
#include "fonts/esp_font.h"
#include "../imgui/imgui_freetype.h"

void c_render::init()
{
	ImGui::CreateContext();
	m_draw_list = new ImDrawList(ImGui::GetDrawListSharedData());
	m_draw_list_act = new ImDrawList(ImGui::GetDrawListSharedData());
	m_draw_list_rendering = new ImDrawList(ImGui::GetDrawListSharedData());

	ImFontConfig cfg;
	cfg.PixelSnapH = false;
	cfg.OversampleH = 5;
	cfg.OversampleV = 5;
	cfg.RasterizerMultiply = 1.2f;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0xE000, 0xE226, // icons
		0,
	};

	cfg.GlyphRanges = ranges;

	g_sdk.m_fonts.m_esp = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		roboto_medium_compressed_data,
		roboto_medium_compressed_size,
		12.f,
		&cfg, ranges);

	g_sdk.m_fonts.m_logs = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		roboto_medium_compressed_data,
		roboto_medium_compressed_size,
		15.f,
		&cfg, ranges);

	ImGui::GetIO().Fonts->AddFontDefault();
	ImGuiFreeType::BuildFontAtlas(ImGui::GetIO().Fonts, 0x00);
}

void c_render::begin() const
{
	m_draw_list->Clear();
	m_draw_list->PushClipRectFullScreen();
}

void c_render::end()
{
	m_render_mutex.lock();
	*m_draw_list_act = *m_draw_list;
	m_render_mutex.unlock();
}

void c_render::clear_draw_list() {
	m_render_mutex.lock();
	m_draw_list_act->Clear();
	m_render_mutex.unlock();
}

ImDrawList* c_render::render_scene() {

	if (m_render_mutex.try_lock()) {
		*m_draw_list_rendering = *m_draw_list_act;
		m_render_mutex.unlock();
	}

	return m_draw_list_rendering;
}

void c_render::rect(const float x1, const float y1, const float x2, const float y2, const c_color color, const float thickness, const float rounding) const
{
	m_draw_list->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), get_u32(color), rounding, 15, thickness);
}

void c_render::rect_filled(const float x1, const float y1, const float x2, const float y2, const c_color color) const
{
	m_draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), get_u32(color), 0.0f);
}

void c_render::circle_2d(const vec3_t position, const int point_count, const float radius, const c_color color) const
{
	auto screen_position = vec3_t(0, 0, 0);
	if (g_sdk.m_interfaces.m_debug_overlay->screen_position(position, screen_position))
		return;

	m_draw_list->AddCircle(ImVec2(screen_position.x, screen_position.y), radius, get_u32(color), point_count);
}

void c_render::circle_2d_filled(const vec3_t position, const int point_count, const float radius, const c_color color) const
{
	auto screen_position = vec3_t(0, 0, 0);
	if (g_sdk.m_interfaces.m_debug_overlay->screen_position(position, screen_position))
		return;

	m_draw_list->AddCircleFilled(ImVec2(screen_position.x, screen_position.y), radius, get_u32(color), point_count);
}

void c_render::circle_3d(const vec3_t position, const int point_count, const float radius, const c_color color) const
{
	const float fl_step = 3.14159265358979323846f * 2.0f / static_cast<float>(point_count);
	for (float a = 0; a < 3.14159265358979323846f * 2.0f; a += fl_step)
	{
		auto start = vec3_t(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		auto end = vec3_t(radius * cosf(a + fl_step) + position.x, radius * sinf(a + fl_step) + position.y,
			position.z);

		vec3_t start_2d, end_2d;
		if (g_sdk.m_interfaces.m_debug_overlay->screen_position(start, start_2d) 
			|| g_sdk.m_interfaces.m_debug_overlay->screen_position(end, end_2d))
			return;

		line(start_2d.x, start_2d.y, end_2d.x, end_2d.y, color, 1.0f);
	}
}

void c_render::circle_3d_filled(const vec3_t& origin, const float radius, const c_color color) const
{
	static auto previous_screen_pos = vec3_t(0, 0, 0);
	static float step = 3.14159265358979323846f * 2.0f / 72.0f;

	auto screen_position = vec3_t(0, 0, 0);
	if (g_sdk.m_interfaces.m_debug_overlay->screen_position(origin, screen_position))
		return;

	for (float rotation = 0.0f; rotation <= 3.14159265358979323846f * 2.0f; rotation += step)
	{
		auto world_position = vec3_t(radius * cos(rotation) + origin.x,
			radius * sin(rotation) + origin.y, origin.z);

		if (g_sdk.m_interfaces.m_debug_overlay->screen_position(world_position, screen_position))
			continue;

		line(previous_screen_pos.x, previous_screen_pos.y, screen_position.x, screen_position.y, color,
			1.0f);
		triangle
		(
			ImVec2(screen_position.x, screen_position.y),
			ImVec2(screen_position.x, screen_position.y),
			ImVec2(previous_screen_pos.x, previous_screen_pos.y),
			c_color(color.r(), color.g(), color.b(), color.a() / 2)
		);

		previous_screen_pos = screen_position;
	}
}

void c_render::line(const float x1, const float y1, const float x2, const float y2, const c_color color, const float thickness) const
{
	m_draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), get_u32(color), thickness);
}

void c_render::triangle(const ImVec2 first, const ImVec2 second, const ImVec2 third, const c_color color) const
{
	m_draw_list->AddTriangleFilled(first, second, third, get_u32(color));
}

void c_render::text(const ImFont* font, ImVec2 text_position, const std::string& text, const c_color color, const bool centered,
	const bool outline, const bool menu)
{
	const ImVec2 im_text_size = font->CalcTextSizeA(font->FontSize, FLT_MAX, 0.0f, text.c_str());
	if (!font->ContainerAtlas)
		return;

	ImDrawList* old_draw_list = m_draw_list;
	if (menu)
		m_draw_list = ImGui::GetOverlayDrawList();

	m_draw_list->PushTextureID(font->ContainerAtlas->TexID);
	if (centered)
		text_position.x -= im_text_size.x / 2.0f;

	if (outline)
	{
		m_draw_list->AddText(font, font->FontSize, ImVec2(text_position.x + 1, text_position.y + 1),
			get_u32(c_color(30, 30, 36, color.a())), text.c_str());
		m_draw_list->AddText(font, font->FontSize, ImVec2(text_position.x - 1, text_position.y - 1),
			get_u32(c_color(30, 30, 36, color.a())), text.c_str());
		m_draw_list->AddText(font, font->FontSize, ImVec2(text_position.x + 1, text_position.y - 1),
			get_u32(c_color(30, 30, 36, color.a())), text.c_str());
		m_draw_list->AddText(font, font->FontSize, ImVec2(text_position.x - 1, text_position.y + 1),
			get_u32(c_color(30, 30, 36, color.a())), text.c_str());
	}

	m_draw_list->AddText(font, font->FontSize, text_position, get_u32(color), text.c_str());
	m_draw_list->PopTextureID();

	if (menu)
		m_draw_list = old_draw_list;
}