#pragma once

class c_color;
class matrix_t;
class c_vec3;
class overlay_text_t;

class i_debug_overlay
{
public:
	virtual void add_entity_text_overlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a,
		const char* format, ...) = 0;
	virtual void add_box_overlay(const c_vec3& origin, const c_vec3& mins, const c_vec3& max, const c_vec3& orientation,
		int r, int g, int b, int a, float duration) = 0;
	virtual void add_sphere_overlay(const c_vec3& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b,
		int a, float flDuration) = 0;
	virtual void add_triangle_overlay(const c_vec3& p1, const c_vec3& p2, const c_vec3& p3, int r, int g, int b, int a,
		bool noDepthTest, float duration) = 0;
	virtual void add_line_overlay(const c_vec3& origin, const c_vec3& dest, int r, int g, int b, bool noDepthTest,
		float duration) = 0;
	virtual void add_text_overlay(const c_vec3& origin, float duration, const char* format, ...) = 0;
	virtual void add_text_overlay(const c_vec3& origin, int line_offset, float duration, const char* format, ...) = 0;
	virtual void add_screen_text_overlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a,
		const char* text) = 0;
	virtual void add_swept_box_overlay(const c_vec3& start, const c_vec3& end, const c_vec3& mins, const c_vec3& max,
		const c_vec3& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void add_grid_overlay(const c_vec3& origin) = 0;
	virtual void add_coord_frame_overlay(const matrix_t& frame, float flScale, int vColorTable[3][3] = nullptr) = 0;
	virtual int screen_position(const c_vec3& point, c_vec3& screen) = 0;
	virtual int screen_position(float flXPos, float flYPos, c_vec3& screen) = 0;
	virtual overlay_text_t* get_first(void) = 0;
	virtual overlay_text_t* get_next(overlay_text_t* current) = 0;
	virtual void clear_dead_overlays(void) = 0;
	virtual void clear_all_overlays(void) = 0;
	virtual void add_text_overlay_rgb(const c_vec3& origin, int line_offset, float duration, float r, float g, float b,
		float alpha, const char* format, ...) = 0;
	virtual void add_text_overlay_rgb(const c_vec3& origin, int line_offset, float duration, int r, int g, int b, int a,
		const char* format, ...) = 0;
	virtual void add_line_overlay_alpha(const c_vec3& origin, const c_vec3& dest, int r, int g, int b, int a,
		bool no_depth_test, float duration) = 0;
	virtual void add_box_overlay2(const c_vec3& origin, const c_vec3& mins, const c_vec3& max, const c_vec3& orientation,
		c_color face_color, c_color edge_color, float duration) = 0;
	virtual void add_line_overlay(const c_vec3& origin, const c_vec3& dest, int r, int g, int b, int a, float, float) = 0;
	virtual void purge_text_overlays() = 0;
	virtual void add_capsule_overlay(const c_vec3& mins, const c_vec3& max, float& radius, int r, int g, int b, int a,
		float duration, char unknown, char ignorez) = 0;
};