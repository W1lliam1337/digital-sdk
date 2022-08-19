#pragma once
#include <cstdint>
#include "../math/vec3.h"

class c_weapon;
class c_base_player;
class matrix_t;

class c_weapon_info {
	char gap14[140];
public:
	char* m_weapon_name;
private:
	char gap90[48];
public:
	int m_weapon_price;
	int m_kill_award;
	int m_weapon_type;
private:
	char gap6[16];
public:
	float m_cycle_time;
	float m_cycle_time_alt;
	float m_time_to_idle;
	float m_idle_interval;
	bool m_is_full_auto;
private:
	char gapED[3];
public:
	int m_damage;
	float m_headshot_multiplier;
	float m_armor_ratio;
	int m_bullets;
	float m_penetration;
	float m_flinch_velocity_modifier_large;
	float m_flinch_velocity_modifier_small;
	float m_range;
	float m_range_modifier;
	float m_throw_velocity;
private:
	char pad11[12];
public:
	bool m_has_silencer;
	char* m_silencer_model;
private:
	char pad12[4];
public:
	int m_crosshair_delta_distance;
	float m_max_player_speed;
	float m_max_player_speed_alt;
	float m_attack_movespeed_factor;
	float m_speed;
	float m_speed_alt;
	float m_inaccuracy_crouch;
	float m_inaccuracy_crouch_alt;
	float m_inaccuracy_stand;
	float m_inaccuracy_stand_alt;
	float m_inaccuracy_jump_initial;
	float m_inaccuracy_jump_apex;
	float m_inaccuracy_jump;
	float m_inaccuracy_jump_alt;
	float m_inaccuracy_land;
	float m_inaccuracy_land_alt;
	float m_inaccuracy_ladder;
	float m_inaccuracy_ladder_alt;
	float m_inaccuracy_fire;
	float m_inaccuracy_fire_alt;
	float m_inaccuracy_move;
	float m_inaccuracy_move_alt;
	float m_inaccuracy_reload;
	float m_recoil_seed;
	float m_recoil_angle;
	float m_recoil_angle_alt;
	float m_recoil_angle_variance;
	float m_recoil_angle_variance_alt;
	float m_recoil_magnitude;
	float m_recoil_magnitude_alt;
	float m_recoil_magnitude_variance;
	float m_recoil_magnitude_variance_alt;
	float m_spread_seed;
	float m_recovery_time_crouch;
	float m_recovery_time_stand;
	float m_recovery_time_crouch_final;
	float m_recovery_time_stand_final;
	float m_recovery_transition_start_bullet;
	float m_recovery_transition_end_bullet;
	bool m_unzoom_after_shot;
	bool m_hide_view_model_zoomed;
private:
	char gap1CE[2];
public:
	int m_zoom_levels;
	int m_zoom_fov1;
	int m_zoom_fov2;
	float m_zoom_time0;
	float m_zoom_time1;
	float m_zoom_time2;
private:
	char gap1E8[8];
public:
	float m_addon_scale;
private:
	char gap1F4[8];
public:
	float m_tracer_frequency;
	float m_tracer_frequency_alt;
private:
	char gap204[20];
public:
	float m_heat_per_shot;
private:
	char gap21C[8];
public:
	float m_inaccuracy_pitch_shift;
	float m_inaccuracy_alt_sound_threshold;
	float m_bot_audible_range;
private:
	char gap230[12];
public:
	bool m_has_burst_mode;
	bool m_is_revolver;
	bool m_cannot_shoot_underwater;
};

class c_anim_layer {
	char pad_0x01[0x14]{};
public:
	int	m_order{};
	int	m_sequence{};
	float m_previous_cycle{};
	float m_weight{};
	float m_weight_delta_rate{};
	float m_playback_rate{};
	float m_cycle{};
	void* m_owner{};
private:
	char pad_0x02[0x4]{};
};

class c_anim_state_pose_param_cache {
	std::uint8_t pad_0x0[0x4]{}; //0x0
public:
	std::uint32_t m_idx{}; //0x4 
	char* m_name{}; //0x8
};

class procedural_foot_t {
public:
	c_vec3 m_pos_anim{};
	c_vec3 m_pos_anim_last{};
	c_vec3 m_pos_plant{};
	c_vec3 m_plant_vel{};
	float m_lock_amount{};
	float m_last_plant_time{};
};

class c_anim_state {
	std::uint8_t pad_0x0000[0x4]{}; //0x0000
public:
	bool m_force_update{}; //0x0005 
private:
	std::uint8_t pad_0x0006[0x5A]{}; //0x0006
public:
	c_base_player* m_player{}; //0x0060 
	c_weapon* m_weapon{}; //0x0064 
	c_weapon* m_last_weapon{}; //0x0068 
	float m_last_clientside_anim_update{}; //0x006C 
	uint32_t m_last_clientside_anim_framecount{}; //0x0070 
	float m_last_clientside_anim_update_time_delta{}; //0x0074 
	float m_eye_yaw{}; //0x0078 
	float m_pitch{}; //0x007C 
	float m_abs_yaw{}; //0x0080 
	float m_feet_yaw{}; //0x0084 
	float m_body_yaw{}; //0x0088 
	float m_body_yaw_clamped{}; //0x008C 
	float m_feet_vel_dir_delta{}; //0x0090 
private:
	uint8_t pad_0x0094[0x4]{}; //0x0094
public:
	float m_feet_cycle{}; //0x0098 
	float m_feet_yaw_rate{}; //0x009C 
private:
	uint8_t pad_0x00A0[0x4]{}; //0x00A0
public:
	float m_duck_amount{}; //0x00A4 
	float m_landing_duck_additive{}; //0x00A8 
private:
	uint8_t pad_0x00AC[0x4]{}; //0x00AC
public:
	c_vec3 m_origin{};
	c_vec3 m_old_origin{};
	c_vec3 m_velocity{};
	c_vec3 m_velocity_normalized{};
	c_vec3 m_last_accelerating_vel{};
	float m_speed2d{};
	float m_abs_velocity{};
	float m_speed_normalized{};
	float m_run_speed{}; //0x00F8 
	float m_duck_speed{}; //0x00FC 
	float m_time_since_move{}; //0x0100 
	float m_time_since_stop{}; //0x0104 
	bool m_on_ground{}; //0x0108 
	bool m_hit_ground{}; //0x0109 
private:
	uint8_t pad_0x010A[0x4]{}; //0x010A
public:
	float m_time_in_air{}; //0x0110 
private:
	uint8_t pad_0x0114[0x6]{}; //0x0114
public:
	float m_ground_fraction{}; //0x011C 
private:
	uint8_t pad_0x0120[0x2]{}; //0x0120
public:
	float m_unk_fraction{}; //0x0124 
private:
	uint8_t pad_0x0128[0xC]{}; //0x0128
public:
	bool m_moving{}; //0x0134
private:
	uint8_t pad_0x0135[0x7B]{}; //0x0135
public:
	c_anim_state_pose_param_cache m_lean_yaw_pose{}; //0x1B0
	c_anim_state_pose_param_cache m_speed_pose{}; //0x01BC
	c_anim_state_pose_param_cache m_ladder_speed_pose{}; //0x01C8
	c_anim_state_pose_param_cache m_ladder_yaw_pose{}; //0x01D4
	c_anim_state_pose_param_cache m_move_yaw_pose{}; //0x01E0
	c_anim_state_pose_param_cache m_run_pose{}; //0x01EC 
	c_anim_state_pose_param_cache m_body_yaw_pose{}; //0x01F8
	c_anim_state_pose_param_cache m_body_pitch_pose{}; //0x0204
	c_anim_state_pose_param_cache m_dead_yaw_pose{}; //0x0210
	c_anim_state_pose_param_cache m_stand_pose{}; //0x021C
	c_anim_state_pose_param_cache m_jump_fall_pose{}; //0x0228
	c_anim_state_pose_param_cache m_aim_blend_stand_idle_pose{}; //0x0234
	c_anim_state_pose_param_cache m_aim_blend_crouch_idle_pose{}; //0x0240
	c_anim_state_pose_param_cache m_strafe_yaw_pose{}; //0x024C
	c_anim_state_pose_param_cache m_aim_blend_stand_walk_pose{}; //0x0258
	c_anim_state_pose_param_cache m_aim_blend_stand_run_pose{}; //0x0264
	c_anim_state_pose_param_cache m_aim_blend_crouch_walk_pose{}; //0x0270
	c_anim_state_pose_param_cache m_move_blend_walk_pose{}; //0x027C
	c_anim_state_pose_param_cache m_move_blend_run_pose{}; //0x0288
	c_anim_state_pose_param_cache m_move_blend_crouch_pose{}; //0x0294
	float m_duration_move_weight_is_too_high{};
	float m_static_approach_speed{};
	int m_previous_move_state{};
	float m_stutter_step{};
	float m_action_weight_bias_remainder{};
	procedural_foot_t m_foot_left;
	procedural_foot_t m_foot_right;
	float m_camera_smooth_height{};
	bool m_smooth_height_valid{};
	float m_last_time_velocity_over_ten{};
	float m_min_yaw{}; //0x0330 
	float m_max_yaw{}; //0x0334 
	float m_max_pitch{}; //0x0338 
	float m_min_pitch{}; //0x033C
};

class c_bone_accressor {
public:
	[[nodiscard]] matrix_t* get_bone_array_for_write( ) const {
		return m_bone_array;
	}

	void set_bone_array_for_write( matrix_t* bone_array ) {
		m_bone_array = bone_array;
	}

	void set_readable_bones( const int flags ) {
		m_readable_bones = flags;
	}

	void set_writable_bones( const int flags ) {
		m_writable_bones = flags;
	}
private:
	alignas(16) matrix_t* m_bone_array;
	int m_readable_bones;
	int m_writable_bones;
};
