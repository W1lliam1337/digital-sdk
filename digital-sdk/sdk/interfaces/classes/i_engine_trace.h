#pragma once

#include <cstring>
#include "i_client_entity.h"
#include "../../math/vec4.h"

class c_vec3;
class i_handle_entity;
struct ray_t;
class i_collideable;
class i_trace_list_data;
class c_phys_collide;
struct virtualmeshlist_t;

#define CAM_HULL_OFFSET 14.0f
#define CAM_HULL_MIN c_vec3( -CAM_HULL_OFFSET,-CAM_HULL_OFFSET,-CAM_HULL_OFFSET )
#define CAM_HULL_MAX c_vec3( CAM_HULL_OFFSET, CAM_HULL_OFFSET, CAM_HULL_OFFSET  )

enum e_surfaceflags {
	dispsurf_flag_surface = (1 << 0),
	dispsurf_flag_walkable = (1 << 1),
	dispsurf_flag_buildable = (1 << 2),
	dispsurf_flag_surfprop1 = (1 << 3),
	dispsurf_flag_surfprop2 = (1 << 4),
};

enum e_contents
{
	contents_empty = 0,
	contents_solid = 0x1,
	contents_window = 0x2,
	contents_aux = 0x4,
	contents_grate = 0x8,
	contents_slime = 0x10,
	contents_water = 0x20,
	contents_blocklos = 0x40,
	contents_opaque = 0x80,
	contents_testfogvolume = 0x100,
	contents_unused = 0x200,
	contents_blocklight = 0x400,
	contents_team1 = 0x800,
	contents_team2 = 0x1000,
	contents_ignore_nodraw_opaque = 0x2000,
	contents_moveable = 0x4000,
	contents_areaportal = 0x8000,
	contents_playerclip = 0x10000,
	contents_monsterclip = 0x20000,
	contents_current_0 = 0x40000,
	contents_current_90 = 0x80000,
	contents_current_180 = 0x100000,
	contents_current_270 = 0x200000,
	contents_current_up = 0x400000,
	contents_current_down = 0x800000,
	contents_origin = 0x1000000,
	contents_monster = 0x2000000,
	contents_debris = 0x4000000,
	contents_detail = 0x8000000,
	contents_translucent = 0x10000000,
	contents_ladder = 0x20000000,
	contents_hitbox = 0x40000000,
};

enum e_surf
{
	surf_light = 0x0001,
	surf_sky2d = 0x0002,
	surf_sky = 0x0004,
	surf_warp = 0x0008,
	surf_trans = 0x0010,
	surf_noportal = 0x0020,
	surf_trigger = 0x0040,
	surf_nodraw = 0x0080,
	surf_hint = 0x0100,
	surf_skip = 0x0200,
	surf_nolight = 0x0400,
	surf_bumplight = 0x0800,
	surf_noshadows = 0x1000,
	surf_nodecals = 0x2000,
	surf_nopaint = surf_nodecals,
	surf_nochop = 0x4000,
	surf_hitbox = 0x8000
};

enum e_masks {
	mask_all = 0xffffffff,
	mask_solid = contents_solid | contents_moveable | contents_window | contents_monster | contents_grate,
	mask_playersolid = contents_solid | contents_moveable | contents_playerclip | contents_window | contents_monster | contents_grate,
	mask_npcsolid = contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster | contents_grate,
	mask_npcfluid = contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster | contents_grate,
	mask_water = contents_water | contents_moveable | contents_slime,
	mask_opaque = contents_solid | contents_moveable | contents_opaque,
	mask_opaque_and_npcs = mask_opaque | contents_monster,
	mask_blocklos = contents_solid | contents_moveable | contents_blocklos,
	mask_blocklos_and_npcs = mask_blocklos | contents_monster,
	mask_visible = mask_opaque | contents_ignore_nodraw_opaque,
	mask_visible_and_npcs = mask_opaque_and_npcs | contents_ignore_nodraw_opaque,
	mask_shot = contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate | contents_hitbox,
	mask_shot_brushonly = contents_solid | contents_moveable | contents_window | contents_debris,
	mask_shot_hull = contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate,
	mask_shot_portal = contents_solid | contents_moveable | contents_window | contents_monster,
	mask_solid_brushonly = contents_solid | contents_moveable | contents_window | contents_grate,
	mask_playersolid_brushonly = contents_solid | contents_moveable | contents_window | contents_playerclip | contents_grate,
	mask_npcsolid_brushonly = contents_solid | contents_moveable | contents_window | contents_monsterclip | contents_grate,
	mask_npcworldstatic = contents_solid | contents_window | contents_monsterclip | contents_grate,
	mask_npcworldstatic_fluid = contents_solid | contents_window | contents_monsterclip,
	mask_splitareportal = contents_water | contents_slime,
	mask_current = contents_current_0 | contents_current_90 | contents_current_180 | contents_current_270 | contents_current_up | contents_current_down,
	mask_deadsolid = contents_solid | contents_playerclip | contents_window | contents_grate,
};

enum e_trace_type : std::int32_t
{
	trace_everything = 0,
	trace_world_only,
	trace_entities_only,
	trace_everything_filter_props,
};

class i_trace_filter
{
public:
	virtual bool should_hit_entity(i_handle_entity* pEntity, int contentsMask) = 0;
	virtual e_trace_type get_trace_type() const = 0;
};

class c_trace_filter : public i_trace_filter
{
public:
	bool should_hit_entity(i_handle_entity* pEntityHandle, int contents_mask) override
	{
		const auto e_cc = static_cast<i_client_entity*>(pEntityHandle)->get_client_class();
		if (e_cc && m_ignore != nullptr && strcmp(m_ignore, "") != 0)
		{
			if (e_cc->m_network_name == m_ignore)
				return false;
		}

		return !(pEntityHandle == m_skip);
	}

	[[nodiscard]] e_trace_type get_trace_type() const override
	{
		return trace_everything;
	}

	void set_ignore_class(const char* Class)
	{
		m_ignore = Class;
	}

	void* m_skip;
	const char* m_ignore{ "" };
};

struct brush_side_info_t
{
	c_vec4 m_plane; // The plane of the brush side
	uint16_t m_bevel; // Bevel plane?
	uint16_t m_thin; // Thin?
};

class c_phys_collide;

struct vcollide_t
{
	uint16_t m_solid_count : 15;
	uint16_t m_is_packed : 1;
	uint16_t m_desc_size;
	c_phys_collide** m_solids;
	char* m_key_values;
	void* m_user_data;
};

struct cmodel_t
{
	c_vec3 m_mins, m_maxs;
	c_vec3 m_origin; // for sounds or lights
	int m_headnode;
	vcollide_t m_vcollision_data;
};

struct csurface_t
{
	const char* m_name;
	short m_surface_props;
	uint16_t m_flags;
};

class __declspec(align(16)) vector_aligned : public c_vec3
{
public:
	vector_aligned(void)
	{
	};

	vector_aligned(float X, float Y, float Z)
	{
		init(X, Y, Z);
	}

	explicit vector_aligned(const c_vec3& vOther)
	{
		init(vOther.x, vOther.y, vOther.z);
	}

	vector_aligned& operator=(const c_vec3& vOther)
	{
		init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	vector_aligned& operator=(const vector_aligned& vOther)
	{
		init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w{};
};

//-----------------------------------------------------------------------------
// A ray...
//-----------------------------------------------------------------------------
struct ray_t
{
	vector_aligned m_start; // starting point, centered within the extents
	vector_aligned m_delta; // direction + length of the ray
	vector_aligned m_start_offset; // Add this to m_Start to Get the actual ray start
	vector_aligned m_extents; // Describes an axis aligned box extruded along a ray
	const matrix_t* m_world_axis_transform;
	bool m_is_ray; // are the extents zero?
	bool m_is_swept; // is delta != 0?

	ray_t() : m_world_axis_transform(nullptr)
	{
	}

	void init(const c_vec3& start, const c_vec3& end)
	{
		m_delta = end - start;

		m_is_swept = (m_delta.length_sqr() != 0);

		m_extents.init();

		m_world_axis_transform = nullptr;
		m_is_ray = true;

		// Offset m_Start to be in the center of the box...
		m_start_offset.init();
		m_start = start;
	}

	void init(const c_vec3& start, const c_vec3& end, const c_vec3& mins, const c_vec3& maxs)
	{
		m_delta = end - start;

		m_world_axis_transform = nullptr;
		m_is_swept = (m_delta.length_sqr() != 0);

		m_extents = maxs - mins;
		m_extents *= 0.5f;
		m_is_ray = (m_extents.length_sqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		m_start_offset = maxs + mins;
		m_start_offset *= 0.5f;
		m_start = start + m_start_offset;
		m_start_offset *= -1.0f;
	}
};

struct cplane_t
{
	c_vec3 m_normal;
	float m_dist;
	uint8_t m_type; // for fast side tests
	uint8_t m_signbits; // signx + (signy<<1) + (signz<<1)
	uint8_t m_pad[2];
};

class c_game_trace
{
public:
	[[nodiscard]] bool did_hit() const
	{
		return m_fraction < 1.0f || m_all_solid || m_start_solid;
	}

	[[nodiscard]] bool is_visible() const
	{
		return m_fraction > 0.97f;
	}

	c_vec3			m_start;		// start position
	c_vec3			m_end;			// final position
	cplane_t		m_plane;			// surface normal at impact
	float			m_fraction;		// time completed, 1.0 = didn't hit anything
	int				m_contents;		// contents on other side of surface hit
	uint16_t		m_disp_flags;		// displacement flags for marking surfaces with data
	bool			m_all_solid;		// if true, plane is not valid
	bool			m_start_solid;	// if true, the initial point was in a solid area
	float				m_fraction_left_solid;	// time we left a solid, only valid if we started in solid
	csurface_t			m_surface;				// surface hit (impact surface)
	int					m_hit_group;				// 0 == generic, non-zero is specific body part
	std::uint16_t		m_world_surface_index;		// index of the msurface2_t, if applicable
	c_base_player* m_entity;				// entity hit by trace
	int					m_hitbox;				// box hit by trace in studio
};

class i_engine_trace
{
public:
	virtual int get_point_contents(const c_vec3& abs_position, int contents_mask = mask_all,
		i_handle_entity** ppEntity = nullptr) = 0;
	virtual int get_point_contents_world_only(const c_vec3& abs_position, int contents_mask = mask_all) = 0;
	virtual int get_point_contents_collideable(i_collideable* collide, const c_vec3& abs_position) = 0;
	virtual void clip_ray_to_entity(const ray_t& ray, unsigned int mask, i_handle_entity* ent, c_game_trace* trace) = 0;
	virtual void clip_ray_to_collideable(const ray_t& ray, unsigned int mask, i_collideable* collide, c_game_trace* trace)
		= 0;
	virtual void trace_ray(const ray_t& ray, unsigned int mask, c_trace_filter* filter, c_game_trace* engine_trace) = 0;

	void trace_line(c_vec3 src, c_vec3 dst, int mask, i_handle_entity* entity, int collision_group,
		c_game_trace* trace);
};
