#pragma once
#include "../../../utils/utils.h"

class c_view_setup
{
public:
	int m_x{};
	int m_unscaled_x{};
	int m_y{};
	int m_unscaled_y{};
	int m_width{};
	int m_unscaled_width{};
	int m_height{};
	int m_unscaled_height{};
	bool m_ortho{};
	std::byte pad0[0x8F];
	float m_fov{};
	float m_view_model_fov{};
	c_vec3 m_origin{};
	qangle_t m_view{};
	float m_near_z{};
	float m_far_z{};
	float m_near_viewmodel_z{};
	float m_far_viewmodel_z{};
	float m_aspect_ratio{};
	float m_near_blur_depth{};
	float m_near_focus_depth{};
	float m_far_focus_depth{};
	float m_far_blur_depth{};
	float m_near_blur_radius{};
	float m_far_blur_radius{};
	float m_do_fquality{};
	int m_motion_blur_mode{};
	float m_shutter_time{};
	c_vec3 m_shutter_open_position{};
	qangle_t m_shutter_open_angles{};
	c_vec3 m_shutter_close_position{};
	qangle_t m_shutter_close_angles{};
	float m_off_center_top{};
	float m_off_center_bottom{};
	float m_off_center_left{};
	float m_off_center_right{};
	bool m_off_center{};
	bool m_render_to_subrect_of_larger_screen{};
	bool m_do_bloom_and_tone_mapping{};
	bool m_do_depth_of_field{};
	bool m_hdr_target{};
	bool m_draw_world_normal{};
	bool m_cull_font_faces{};
	bool m_cache_full_scene_state{};
	bool m_csm_view{};
};

class i_render_view
{
public:
	void set_blend(const float blend)
	{
		utils::call_vfunc<void(__thiscall*)(void*, float)>(this, 4)(this, blend);
	}

	void set_color_modulation(const float blend)
	{
		utils::call_vfunc<void(__thiscall*)(void*, float)>(this, 6)(this, blend);
	}
};
