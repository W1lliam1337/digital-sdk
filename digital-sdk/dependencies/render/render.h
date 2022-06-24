#pragma once
#include "../singleton/singleton.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <mutex>

#include "../color/color.h"
#include "../obfuscation/xor.h"
#include "../../sdk/math/math.h"

class vector_2d;
class vec3_t;

class c_render : public c_singleton<c_render>
{
	std::mutex m_render_mutex{};

	static ImU32 get_u32(c_color color)
	{
		return ((color[3] & 0xff) << 24) + ((color[2] & 0xff) << 16) + ((color[1] & 0xff) << 8) + (color[0] & 0xff);
	}

public:
	void rect(float x1, float y1, float x2, float y2, c_color color, float thickness = 1.f, float rounding = 0.f) const;
	void rect_filled(float x1, float y1, float x2, float y2, c_color color) const;
	void circle_2d(vec3_t position, int point_count, float radius, c_color color) const;
	void circle_2d_filled(vec3_t position, int point_count, float radius, c_color color) const;
	void circle_3d(vec3_t position, int point_count, float radius, c_color color) const;
	void circle_3d_filled(const vec3_t& origin, float radius, c_color color) const;
	void line(float x1, float y1, float x2, float y2, c_color color, float thickness) const;
	void triangle(ImVec2 first, ImVec2 second, ImVec2 third, c_color color) const;
	void text(const ImFont* font, ImVec2 text_position, const std::string& text, c_color color, bool centered,
	                 bool outline = true,
	                 bool menu = false);
	void init();
	void begin() const;
	void end();
	void clear_draw_list();
	ImDrawList* render_scene();

	ImDrawList* m_draw_list_act{};
	ImDrawList* m_draw_list_rendering{};
	ImDrawList* m_draw_list{};
	ImDrawData m_draw_data{};
};
