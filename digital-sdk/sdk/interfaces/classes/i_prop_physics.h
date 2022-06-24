#pragma once
#include <cstddef>

struct surfacephysicsparams_t
{
	float m_friction;
	float m_elasticity;
	float m_density;
	float m_thickness;
	float m_dampening;
};

struct surfaceaudioparams_t
{
	float m_reflectivity; // like elasticity, but how much sound should be reflected by this surface
	float m_hardness_factor; // like elasticity, but only affects impact sound choices
	float m_roughness_factor; // like friction, but only affects scrape sound choices
	float m_rough_threshold; // surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
	float m_hard_threshold; // surface hardness > this causes "hard" impacts, < this causes "soft" impacts
	float m_hard_velocity_threshold; // collision velocity > this causes "hard" impacts, < this causes "soft" impacts
	float m_high_pitch_occlusion;
	//a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound)
	float m_mid_pitch_occlusion;
	float m_low_pitch_occlusion;
};

struct surfacesoundnames_t
{
	unsigned short m_walk_step_left;
	unsigned short m_walk_step_right;
	unsigned short m_run_step_left;
	unsigned short m_run_step_right;
	unsigned short m_impact_soft;
	unsigned short m_impact_hard;
	unsigned short m_scrape_smooth;
	unsigned short m_scrape_rough;
	unsigned short m_bullet_impact;
	unsigned short m_rolling;
	unsigned short m_break_sound;
	unsigned short m_strain_sound;
};

struct surfacegameprops_t
{
public:
	float m_max_speed_factor{};
	float m_jump_factor{};
	float m_penetration_modifier{};
	float m_damage_modifier{};
	unsigned short m_material{};
	std::byte m_climbable{};
};

struct surfacedata_t
{
	surfacephysicsparams_t m_physics;
	surfaceaudioparams_t m_audio;
	surfacesoundnames_t m_sounds;
	surfacegameprops_t m_game;
};

class i_physics_surface_props
{
public:
	virtual ~i_physics_surface_props(void)
	{
	}

	virtual int parse_surface_data(const char* p_filename, const char* p_textfile) = 0;
	virtual int surface_prop_count(void) const = 0;
	virtual int get_surface_index(const char* p_surface_prop_name) const = 0;
	virtual void get_physics_properties(int surface_data_index, float* density, float thickness, float friction,
		float elasticity) const = 0;
	virtual surfacedata_t* get_surface_data(int surface_data_index) = 0;
	virtual const char get_string(unsigned short string_table_index) const = 0;
	virtual const char get_prop_name(int surface_data_index) const = 0;
	virtual void set_world_material_index_table(int* p_map_array, int map_size) = 0;
	virtual void get_physics_parameters(int surface_data_index, surfacephysicsparams_t* p_params_out) const = 0;
};