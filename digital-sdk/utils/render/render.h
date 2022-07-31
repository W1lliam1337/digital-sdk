#pragma once
#include "../../sdk/interfaces/interfaces.h"
#include "fonts/roboto_medium.h"
#include <mutex>
#include "../../sdk/other/color/color.h"
#pragma comment(lib, "freetype.lib")

class c_vec2;
class c_vec3;

namespace render
{
	inline std::mutex m_render_mutex{};

	static ImU32 get_u32(c_color color)
	{
		return ((color[3] & 0xff) << 24) + ((color[2] & 0xff) << 16) + ((color[1] & 0xff) << 8) + (color[0] & 0xff);
	}

	namespace fonts
	{
		inline ImFont* m_esp{};
		inline ImFont* m_logs{};
	}

	void rect(float x1, float y1, float x2, float y2, c_color color, float thickness = 1.f, float rounding = 0.f);
	void rect_filled(float x1, float y1, float x2, float y2, c_color color);
	void circle_2d(c_vec3 position, int point_count, float radius, c_color color);
	void circle_2d_filled(c_vec3 position, int point_count, float radius, c_color color);
	void circle_3d(c_vec3 position, int point_count, float radius, c_color color);
	void circle_3d_filled(const c_vec3& origin, float radius, c_color color);
	void line(float x1, float y1, float x2, float y2, c_color color, float thickness);
	void triangle(ImVec2 first, ImVec2 second, ImVec2 third, c_color color);
	void text(const ImFont* font, ImVec2 text_position, const std::string& text, c_color color, bool centered,
		bool outline = true,
		bool menu = false);
	void init() noexcept;
	void begin();
	void end();
	void clear_draw_list();
	ImDrawList* render_scene();

	inline ImDrawList* m_draw_list_act{};
	inline ImDrawList* m_draw_list_rendering{};
	inline ImDrawList* m_draw_list{};
	inline ImDrawData m_draw_data{};
}