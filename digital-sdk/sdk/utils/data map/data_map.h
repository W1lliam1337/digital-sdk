#pragma once
#include <cstdint>
#include "../../sdk.hpp"

struct inputdata_t;

using fieldtype_t = enum _fieldtypes
{
	field_void = 0,// No type or value
	field_float,// Any floating point value
	field_string,// A string ID (return from ALLOC_STRING)
	field_vector,// Any vector, QAngle, or AngularImpulse
	field_quaternion,// A quaternion
	field_integer,// Any integer or enum
	field_boolean,// boolean, implemented as an int, I may use this as a hint for compression
	field_short,// 2 byte integer
	field_character,// a byte
	field_color32,// 8-bit per channel r,g,b,a (32bit color)
	field_embedded,// an embedded object with a datadesc, recursively traverse and embedded class/structure based on an additional typedescription
	field_custom,// special type that contains function pointers to it's read/write/parse functions

	field_classptr,// CBaseEntity *
	field_ehandle,// Entity handle
	field_edict,// edict_t *

	field_position_vector,// A world coordinate (these are fixed up across level transitions automagically)
	field_time,// a floating point time (these are fixed up automatically too!)
	field_tick,// an integer tick count( fixed up similarly to time)
	field_modelname,// Engine string that is a model name (needs precache)
	field_soundname,// Engine string that is a sound name (needs precache)

	field_input,// a list of inputed data fields (all derived from CMultiInputVar)
	field_function,// A class function pointer (Think, Use, etc)

	field_vmatrix,// a vmatrix (output coords are NOT worldspace)

	// NOTE: Use float arrays for local transformations that don't need to be fixed up.
	field_vmatrix_worldspace,// A VMatrix that maps some local space to world space (translation is fixed up on level transitions)
	field_matrix3_x4_worldspace,// matrix3x4_t that maps some local space to world space (translation is fixed up on level transitions)

	field_interval,// a start and range floating point interval ( e.g., 3.2->3.6 == 3.2 and 0.4 )
	field_modelindex,// a model index
	field_materialindex,// a material index (using the material precache string table)

	field_vector_2d,// 2 floats

	field_typecount,// MUST BE LAST
};

class ISaveRestoreOps;
class c_base_entity;
class c_base_player;
//
// Function prototype for all input handlers.
//
typedef void (c_base_entity::*inputfunc_t)(inputdata_t& data);

struct c_data_map;
struct typedescription_t;

enum
{
	td_offset_normal = 0,
	td_offset_packed = 1,

	// Must be last
	td_offset_count,
};

struct typedescription_t
{
	int32_t field_type; //0x0000
	char* field_name; //0x0004
	int field_offset[td_offset_count]; //0x0008
	int16_t field_size_unknwn; //0x0010
	int16_t flags_unkwn; //0x0012
	char pad_0014[12]; //0x0014
	c_data_map* td; //0x0020
	char pad_0024[24]; //0x0024
}; //Size: 0x003C

//-----------------------------------------------------------------------------
// Purpose: stores the list of objects in the hierarchy
//            used to iterate through an object's data descriptions
//-----------------------------------------------------------------------------
struct c_data_map
{
	typedescription_t* m_data_description;
	int m_data_num_fields;
	const char* m_data_class_name;
	c_data_map* m_base_map;
	bool m_chains_validated;
	bool m_packed_offsets_computed;
	int m_packed_size;
};

inline uint32_t find_in_data_map(c_data_map* data_map, const uint32_t prop_name)
{
	while (data_map)
	{
		for (int i = 0; i < data_map->m_data_num_fields; i++)
		{
			if (!data_map->m_data_description[i].field_name)
				continue;

			if (data_map->m_data_description[i].field_name == reinterpret_cast<const char*>(prop_name))
				return data_map->m_data_description[i].field_offset[td_offset_normal];

			if (data_map->m_data_description[i].field_type != field_embedded)
				continue;

			if (!data_map->m_data_description[i].td)
				continue;

			const uint32_t iOffset = find_in_data_map(data_map->m_data_description[i].td, prop_name);
			if (!iOffset)
				continue;

			return iOffset;
		}

		data_map = data_map->m_base_map;
	}

	return 0;
}
