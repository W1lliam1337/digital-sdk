#pragma once

#include <cstdint>
#include "../../math/qangle.h"
#include "../../math/vec3.h"
#include "../../math/matrix.h"

using quaternion = float[4];
using radian_euler = float[3];

#define MAX_QPATH  260
#define MAX_NUM_LODS 8
#define MAXSTUDIOBONES		128		// total bones actually used

enum e_bone_flags
{
	bone_calculate_mask = 0x1f,
	bone_physically_simulated = 0x01,
	bone_physics_procedural = 0x02,
	bone_always_procedural = 0x04,
	bone_screen_align_sphere = 0x08,
	bone_screen_align_cylinder = 0x10,
	bone_used_mask = 0x0007ff00,
	bone_used_by_anything = 0x0007ff00,
	bone_used_by_hitbox = 0x00000100,
	bone_used_by_attachment = 0x00000200,
	bone_used_by_vertex_mask = 0x0003fc00,
	bone_used_by_vertex_lod0 = 0x00000400,
	bone_used_by_vertex_lod1 = 0x00000800,
	bone_used_by_vertex_lod2 = 0x00001000,
	bone_used_by_vertex_lod3 = 0x00002000,
	bone_used_by_vertex_lod4 = 0x00004000,
	bone_used_by_vertex_lod5 = 0x00008000,
	bone_used_by_vertex_lod6 = 0x00010000,
	bone_used_by_vertex_lod7 = 0x00020000,
	bone_used_by_bone_merge = 0x00040000,
	bone_type_mask = 0x00f00000,
	bone_fixed_alignment = 0x00100000,
	bone_has_saveframe_pos = 0x00200000,
	bone_has_saveframe_rot = 0x00400000
};

enum e_hitgroups
{
	hitgroup_generic = 0,
	hitgroup_head,
	hitgroup_chest,
	hitgroup_stomach,
	hitgroup_leftarm,
	hitgroup_rightarm,
	hitgroup_leftleg,
	hitgroup_rightleg,
	hitgroup_neck,
	hitgroup_gear = 10
};

enum e_modtypes
{
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

enum e_hitboxes
{
	hitbox_head,
	hitbox_neck,
	hitbox_pelvis,
	hitbox_stomach,
	hitbox_lower_chest,
	hitbox_chest,
	hitbox_upper_chest,
	hitbox_right_thigh,
	hitbox_left_thigh,
	hitbox_right_calf,
	hitbox_left_calf,
	hitbox_right_foot,
	hitbox_left_foot,
	hitbox_right_hand,
	hitbox_left_hand,
	hitbox_right_upper_arm,
	hitbox_right_forearm,
	hitbox_left_upper_arm,
	hitbox_left_forearm,
	hitbox_max
};

using mdl_handle_t = unsigned short;

struct mstudiobone_t
{
	int m_name_index{};
	char* psz_name(void) const { return (char*)this + m_name_index; }
	int m_parent{};
	int m_bone_controller[6]{}; // bone controller index, -1 == none
	c_vec3 m_pos{};
	quaternion m_quat{};
	radian_euler m_rot{};
	// compression scale
	c_vec3 m_pos_scale{};
	c_vec3 m_rot_scale{};

	matrix_t m_pose_to_bone{};
	quaternion m_alignment{};
	int m_flags{};
	int m_proc_type{};
	int m_proc_index{};
	mutable int m_physics_bone{};
	int m_surface_prop_idx{};

	int m_contents{};
	int m_surface_prop_lookup{};
	int m_pad01[7]{};

	char* psz_surface_prop(void) const { return (char*)(this + m_surface_prop_idx); }
	int get_surface_prop(void) const { return m_surface_prop_lookup; }

	mstudiobone_t()
	{
	}

private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t& other);
};

struct mstudiobbox_t
{
	int m_bone{};
	int m_group{};
	c_vec3 m_mins{};
	c_vec3 m_maxs{};
	int m_hitbox_name_index{};
	c_vec3 m_rotation{};
	float m_radius{};
	int pad2[4]{};

	const char* get_name()
	{
		if (!m_hitbox_name_index) return nullptr;
		return reinterpret_cast<const char*>(reinterpret_cast<uint8_t*>(this) + m_hitbox_name_index);
	}
};

struct mstudiohitboxset_t
{
	int m_name_index{};
	int m_num_hitboxes{};
	int m_hitbox_index{};

	const char* get_name()
	{
		if (!m_name_index) return nullptr;
		return reinterpret_cast<const char*>(reinterpret_cast<uint8_t*>(this) + m_name_index);
	}

	mstudiobbox_t* get_hitbox(const int i)
	{
		if (i > m_num_hitboxes) return nullptr;
		return reinterpret_cast<mstudiobbox_t*>(reinterpret_cast<uint8_t*>(this) + m_hitbox_index) + i;
	}
};

class studiohdr_t
{
public:
	__int32 m_id{}; //0x0000 
	__int32 m_version{}; //0x0004 
	long m_checksum{}; //0x0008 
	char m_sz_name[64]{}; //0x000C 
	__int32 m_length{}; //0x004C 
	c_vec3 m_eye_pos{}; //0x0050 
	c_vec3 m_illum_pos{}; //0x005C 
	c_vec3 m_hull_min{}; //0x0068 
	c_vec3 m_hull_max{}; //0x0074 
	c_vec3 m_bb_min{}; //0x0080 
	c_vec3 m_bb_max{}; //0x008C 
	__int32 m_flags{}; //0x0098 
	__int32 m_num_bones{}; //0x009C 
	__int32 m_bone_index{}; //0x00A0 
	__int32 m_num_bone_controllers{}; //0x00A4 
	__int32 m_bone_controller_index{}; //0x00A8 
	__int32 m_num_hitbox_sets{}; //0x00AC 
	__int32 m_hitbox_set_index{}; //0x00B0 
	__int32 m_num_local_anim{}; //0x00B4 
	__int32 m_local_anim_index{}; //0x00B8 
	__int32 m_num_local_seq{}; //0x00BC 
	__int32 m_local_seq_index{}; //0x00C0 
	__int32 m_activity_list_version{}; //0x00C4 
	__int32 m_events_indexed{}; //0x00C8 
	__int32 m_num_textures{}; //0x00CC 
	__int32 m_texture_index{}; //0x00D0

	mstudiohitboxset_t* get_hitbox_set(const int i)
	{
		if (i > m_num_hitbox_sets) return nullptr;
		return reinterpret_cast<mstudiohitboxset_t*>(reinterpret_cast<uint8_t*>(this) + m_hitbox_set_index) + i;
	}

	mstudiobone_t* get_bone(const int i)
	{
		if (i > m_num_bones) return nullptr;
		return reinterpret_cast<mstudiobone_t*>(reinterpret_cast<uint8_t*>(this) + m_bone_index) + i;
	}
};

struct model_render_info_t
{
	c_vec3 m_origin{};
	c_vec3 m_angles{};
	char pad[0x4]{};
	void* m_renderable{};
	const model_t* m_model{};
	const matrix_t* m_model_to_world{};
	const matrix_t* m_lighting_offset{};
	const c_vec3* m_lighting_origin{};
	int m_flags{};
	int m_entity_index{};
	int m_skin{};
	int m_body{};
	int m_hitboxset{};
	unsigned short m_instance{};

	model_render_info_t()
	{
		m_model_to_world = nullptr;
		m_lighting_offset = nullptr;
		m_lighting_origin = nullptr;
	}
};

using studio_decal_handle_t = void*;

struct draw_model_state_t
{
	studiohdr_t* m_studio_hdr{};
	studiohwdata_t* m_studio_hwdara{};
	i_client_renderable* m_entity{};
	const matrix_t* m_model_to_world{};
	studio_decal_handle_t m_decals{};
	int m_drawflags{};
	int m_lod{};
};