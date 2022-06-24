#pragma once
#include "../../math/vec3_t.h"
#include "../../math/vmatrix.h"
#include "../../../dependencies/color/color.h"

class overlay_text_t;

class i_debug_overlay
{
public:
	virtual void add_entity_text_overlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a,
		const char* format, ...) = 0;
	virtual void add_box_overlay(const vec3_t& origin, const vec3_t& mins, const vec3_t& max, const vec3_t& orientation,
		int r, int g, int b, int a, float duration) = 0;
	virtual void add_sphere_overlay(const vec3_t& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b,
		int a, float flDuration) = 0;
	virtual void add_triangle_overlay(const vec3_t& p1, const vec3_t& p2, const vec3_t& p3, int r, int g, int b, int a,
		bool noDepthTest, float duration) = 0;
	virtual void add_line_overlay(const vec3_t& origin, const vec3_t& dest, int r, int g, int b, bool noDepthTest,
		float duration) = 0;
	virtual void add_text_overlay(const vec3_t& origin, float duration, const char* format, ...) = 0;
	virtual void add_text_overlay(const vec3_t& origin, int line_offset, float duration, const char* format, ...) = 0;
	virtual void add_screen_text_overlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a,
		const char* text) = 0;
	virtual void add_swept_box_overlay(const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& max,
		const vec3_t& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void add_grid_overlay(const vec3_t& origin) = 0;
	virtual void add_coord_frame_overlay(const matrix_t& frame, float flScale, int vColorTable[3][3] = nullptr) = 0;
	virtual int screen_position(const vec3_t& point, vec3_t& screen) = 0;
	virtual int screen_position(float flXPos, float flYPos, vec3_t& screen) = 0;
	virtual overlay_text_t* get_first(void) = 0;
	virtual overlay_text_t* get_next(overlay_text_t* current) = 0;
	virtual void clear_dead_overlays(void) = 0;
	virtual void clear_all_overlays(void) = 0;
	virtual void add_text_overlay_rgb(const vec3_t& origin, int line_offset, float duration, float r, float g, float b,
		float alpha, const char* format, ...) = 0;
	virtual void add_text_overlay_rgb(const vec3_t& origin, int line_offset, float duration, int r, int g, int b, int a,
		const char* format, ...) = 0;
	virtual void add_line_overlay_alpha(const vec3_t& origin, const vec3_t& dest, int r, int g, int b, int a,
		bool noDepthTest, float duration) = 0;
	virtual void add_box_overlay2(const vec3_t& origin, const vec3_t& mins, const vec3_t& max, const vec3_t& orientation,
		c_color faceColor, c_color edgeColor, float duration) = 0;
	virtual void add_line_overlay(const vec3_t& origin, const vec3_t& dest, int r, int g, int b, int a, float, float) = 0;
	virtual void purge_text_overlays() = 0;
	virtual void add_capsule_overlay(const vec3_t& mins, const vec3_t& max, float& radius, int r, int g, int b, int a,
		float duration, char unknown, char ignorez) = 0;
};

