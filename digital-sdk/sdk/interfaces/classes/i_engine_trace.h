#pragma once

#include <cstring>
#include "i_client_entity.h"

#define   DISPSURF_FLAG_SURFACE           (1<<0)
#define   DISPSURF_FLAG_WALKABLE          (1<<1)
#define   DISPSURF_FLAG_BUILDABLE         (1<<2)
#define   DISPSURF_FLAG_SURFPROP1         (1<<3)
#define   DISPSURF_FLAG_SURFPROP2         (1<<4)

#define   CONTENTS_EMPTY                0

#define   CONTENTS_SOLID                0x1
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_AUX                  0x4
#define   CONTENTS_GRATE                0x8
#define   CONTENTS_SLIME                0x10
#define   CONTENTS_WATER                0x20
#define   CONTENTS_BLOCKLOS             0x40
#define   CONTENTS_OPAQUE               0x80
#define   LAST_VISIBLE_CONTENTS         CONTENTS_OPAQUE

#define   ALL_VISIBLE_CONTENTS            (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define   CONTENTS_TESTFOGVOLUME        0x100
#define   CONTENTS_UNUSED               0x200
#define   CONTENTS_BLOCKLIGHT           0x400
#define   CONTENTS_TEAM1                0x800
#define   CONTENTS_TEAM2                0x1000
#define   CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_AREAPORTAL           0x8000
#define   CONTENTS_PLAYERCLIP           0x10000
#define   CONTENTS_MONSTERCLIP          0x20000
#define   CONTENTS_CURRENT_0            0x40000
#define   CONTENTS_CURRENT_90           0x80000
#define   CONTENTS_CURRENT_180          0x100000
#define   CONTENTS_CURRENT_270          0x200000
#define   CONTENTS_CURRENT_UP           0x400000
#define   CONTENTS_CURRENT_DOWN         0x800000

#define   CONTENTS_ORIGIN               0x1000000

#define   CONTENTS_MONSTER              0x2000000
#define   CONTENTS_DEBRIS               0x4000000
#define   CONTENTS_DETAIL               0x8000000
#define   CONTENTS_TRANSLUCENT          0x10000000
#define   CONTENTS_LADDER               0x20000000
#define   CONTENTS_HITBOX               0x40000000

#define   SURF_LIGHT                    0x0001
#define   SURF_SKY2D                    0x0002
#define   SURF_SKY                      0x0004
#define   SURF_WARP                     0x0008
#define   SURF_TRANS                    0x0010
#define   SURF_NOPORTAL                 0x0020
#define   SURF_TRIGGER                  0x0040
#define   SURF_NODRAW                   0x0080

#define   SURF_HINT                     0x0100

#define   SURF_SKIP                     0x0200
#define   SURF_NOLIGHT                  0x0400
#define   SURF_BUMPLIGHT                0x0800
#define   SURF_NOSHADOWS                0x1000
#define   SURF_NODECALS                 0x2000
#define   SURF_NOPAINT                  SURF_NODECALS
#define   SURF_NOCHOP                   0x4000
#define   SURF_HITBOX                   0x8000

#define CAM_HULL_OFFSET ( float )14.0f
#define CAM_HULL_MIN Vector( -CAM_HULL_OFFSET,-CAM_HULL_OFFSET,-CAM_HULL_OFFSET )
#define CAM_HULL_MAX Vector( CAM_HULL_OFFSET, CAM_HULL_OFFSET, CAM_HULL_OFFSET  )

// -----------------------------------------------------
// spatial content masks - used for spatial queries (traceline,etc.)
// -----------------------------------------------------
#define   MASK_ALL                      (0xFFFFFFFF)
#define   MASK_SOLID                    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCSOLID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCFLUID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_WATER                    (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define   MASK_OPAQUE                   (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define   MASK_OPAQUE_AND_NPCS          (MASK_OPAQUE|CONTENTS_MONSTER)
#define   MASK_BLOCKLOS                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define   MASK_BLOCKLOS_AND_NPCS        (MASK_BLOCKLOS|CONTENTS_MONSTER)
#define   MASK_VISIBLE                  (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_VISIBLE_AND_NPCS         (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define   MASK_SHOT_HULL                (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define   MASK_SHOT_PORTAL              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_SOLID_BRUSHONLY          (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID_BRUSHONLY    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define   MASK_NPCSOLID_BRUSHONLY       (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC           (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC_FLUID     (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define   MASK_SPLITAREAPORTAL          (CONTENTS_WATER|CONTENTS_SLIME)
#define   MASK_CURRENT                  (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define   MASK_DEADSOLID                (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

class vec3_t;
class i_handle_entity;
struct ray_t;
class i_collideable;
class i_trace_list_data;
class c_phys_collide;
struct cplane_t;
struct virtualmeshlist_t;

enum class trace_type
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
	virtual trace_type get_trace_type() const = 0;
};

//-----------------------------------------------------------------------------
// Classes are expected to inherit these + implement the ShouldHitEntity method
//-----------------------------------------------------------------------------

// This is the one most normal traces will inherit from
class c_trace_filter : public i_trace_filter
{
public:
	bool should_hit_entity(i_handle_entity* pEntityHandle, int /*contentsMask*/) override
	{
		const auto eCC = reinterpret_cast<i_client_entity*>(pEntityHandle)->get_client_class();
		if (eCC && ccIgnore != nullptr && strcmp(ccIgnore, "") != 0) //-V526
		{
			if (eCC->m_network_name == ccIgnore)
			{
				return false;
			}
		}

		return !(pEntityHandle == pSkip);
	}

	[[nodiscard]] trace_type get_trace_type() const override
	{
		return trace_type::trace_everything;
	}

	void SetIgnoreClass(char* Class)
	{
		ccIgnore = Class;
	}

	void* pSkip;
	const char* ccIgnore{ "" };
};

class c_trace_filter_skip_entity : public i_trace_filter
{
public:
	c_trace_filter_skip_entity(i_handle_entity* pEntityHandle)
	{
		pSkip = pEntityHandle;
	}

	bool should_hit_entity(i_handle_entity* pEntityHandle, int /*contentsMask*/) override
	{
		return !(pEntityHandle == pSkip);
	}

	trace_type get_trace_type() const override
	{
		return trace_type::trace_everything;
	}

	void* pSkip;
};

class c_trace_filter_entities_only : public i_trace_filter
{
public:
	bool should_hit_entity(i_handle_entity* pEntityHandle, int /*contentsMask*/) override
	{
		return true;
	}

	trace_type get_trace_type() const override
	{
		return trace_type::trace_entities_only;
	}
};


//-----------------------------------------------------------------------------
// Classes need not inherit from these
//-----------------------------------------------------------------------------
class c_trace_filter_world_only : public i_trace_filter
{
public:
	bool should_hit_entity(i_handle_entity* /*pServerEntity*/, int /*contentsMask*/) override
	{
		return false;
	}

	[[nodiscard]] trace_type get_trace_type() const override
	{
		return trace_type::trace_world_only;
	}
};

class c_trace_filter_world_and_props_only : public i_trace_filter
{
public:
	bool should_hit_entity(i_handle_entity* /*pServerEntity*/, int /*contentsMask*/) override
	{
		return false;
	}

	trace_type get_trace_type() const override
	{
		return trace_type::trace_everything;
	}
};

class c_trace_filter_players_only_skip_one : public i_trace_filter
{
public:
	c_trace_filter_players_only_skip_one(i_client_entity* ent)
	{
		pEnt = ent;
	}
	
	bool should_hit_entity(i_handle_entity* pEntityHandle, int /*contentsMask*/) override
	{
		return static_cast<void*>(pEntityHandle) != static_cast<void*>(pEnt) && reinterpret_cast<i_client_entity*>(
			pEntityHandle)->get_client_class()->m_class_id == 40;
	}

	trace_type get_trace_type() const override
	{
		return trace_type::trace_entities_only;
	}

private:
	i_client_entity* pEnt;
};

class c_trace_filter_hit_all : public c_trace_filter
{
public:
	bool should_hit_entity(i_handle_entity* /*pServerEntity*/, int /*contentsMask*/) override
	{
		return true;
	}
};


//-----------------------------------------------------------------------------
// Enumeration interface for EnumerateLinkEntities
//-----------------------------------------------------------------------------
class i_entity_enumerator
{
public:
	// This gets called with each handle
	virtual bool enum_entity(i_handle_entity* pHandleEntity) = 0;
};


struct brush_side_info_t
{
	Vector4D plane; // The plane of the brush side
	unsigned short bevel; // Bevel plane?
	unsigned short thin; // Thin?
};

class c_phys_collide;

struct vcollide_t
{
	unsigned short solid_count : 15;
	unsigned short is_packed : 1;
	unsigned short desc_size;
	// VPhysicsSolids
	c_phys_collide** solids;
	char* p_key_values;
	void* p_user_data;
};

struct cmodel_t
{
	vec3_t mins, maxs;
	vec3_t origin; // for sounds or lights
	int headnode;
	vcollide_t vcollisionData;
};

struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
	// BUGBUG: These are declared per surface, not per material, but this database is per-material now
};

class __declspec(align(16)) vector_aligned : public vec3_t
{
public:
	vector_aligned(void)
	{
	};

	vector_aligned(float X, float Y, float Z)
	{
		init(X, Y, Z);
	}

public:
	explicit vector_aligned(const vec3_t& vOther)
	{
		init(vOther.x, vOther.y, vOther.z);
	}

	vector_aligned& operator=(const vec3_t& vOther)
	{
		init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	vector_aligned& operator=(const vector_aligned& vOther)
	{
		init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
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

	void init(const vec3_t& start, const vec3_t& end)
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

	void init(const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& maxs)
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

	[[nodiscard]] vec3_t inv_delta() const
	{
		vec3_t vecInvDelta;
		for (int iAxis = 0; iAxis < 3; ++iAxis)
		{
			if (m_delta[iAxis] != 0.0f)
			{
				vecInvDelta[iAxis] = 1.0f / m_delta[iAxis];
			}
			else
			{
				vecInvDelta[iAxis] = FLT_MAX;
			}
		}
		return vecInvDelta;
	}
};

class c_base_trace
{
public:
	vec3_t			m_start;		// start position
	vec3_t			m_end;			// final position
	cplane_t		m_plane;			// surface normal at impact
	float			m_fraction{};		// time completed, 1.0 = didn't hit anything
	int				m_contents{};		// contents on other side of surface hit
	uint16_t		m_disp_flags{};		// displacement flags for marking surfaces with data
	bool			m_all_solid{};		// if true, plane is not valid
	bool			m_start_solid{};	// if true, the initial point was in a solid area
};

class c_game_trace : public c_base_trace
{
public:
	c_game_trace() {}

	bool did_hit()
	{
		return m_fraction < 1.0f || m_all_solid || m_start_solid;
	}

	bool is_visible()
	{
		return m_fraction > 0.97f;
	}

	float				m_fraction_left_solid{};	// time we left a solid, only valid if we started in solid
	csurface_t			m_surface{};				// surface hit (impact surface)
	int					m_hit_group{};				// 0 == generic, non-zero is specific body part
	std::uint16_t		m_world_surface_index{};		// index of the msurface2_t, if applicable
	c_base_player*		m_entity{};				// entity hit by trace
	int					m_hitbox{};				// box hit by trace in studio
};

using trace_t = c_game_trace;

class c_engine_trace
{
public:
	virtual int get_point_contents(const vec3_t& vecAbsPosition, int contentsMask = MASK_ALL,
		i_handle_entity** ppEntity = nullptr) = 0;
	virtual int get_point_contents_world_only(const vec3_t& vecAbsPosition, int contentsMask = MASK_ALL) = 0;
	virtual int get_point_contents_collideable(i_collideable* pCollide, const vec3_t& vecAbsPosition) = 0;
	virtual void clip_ray_to_entity(const ray_t& ray, unsigned int fMask, i_handle_entity* pEnt, trace_t* pTrace) = 0;
	virtual void clip_ray_to_collideable(const ray_t& ray, unsigned int fMask, i_collideable* pCollide, trace_t* pTrace)
		= 0;
	virtual void trace_ray(const ray_t& ray, unsigned int fMask, i_trace_filter* pTraceFilter, trace_t* pTrace) = 0;

	void trace_line(vec3_t src, vec3_t dst, int mask, i_handle_entity* entity, int collision_group,
		trace_t* trace);
};
