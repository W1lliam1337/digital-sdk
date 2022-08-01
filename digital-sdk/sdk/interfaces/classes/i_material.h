#pragma once
#include "../../../utils/utils.h"
#include "../../math/vec3.h"
#include "../../math/vec4.h"
#include "../../math/vec2.h"
#include "../../game/enums.h"
#include "../../other/key-values/key_values.h"

#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#define MAXSTUDIOSKINS		32

// these are given to findmaterial to reference the texture groups that Show up on the
#define TEXTURE_GROUP_LIGHTMAP						        "Lightmaps"
#define TEXTURE_GROUP_WORLD							          "World textures"
#define TEXTURE_GROUP_MODEL							          "Model textures"
#define TEXTURE_GROUP_VGUI							          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE						        "Particle textures"
#define TEXTURE_GROUP_DECAL							          "Decal textures"
#define TEXTURE_GROUP_SKYBOX						          "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				      "ClientEffect textures"
#define TEXTURE_GROUP_OTHER							          "Other textures"
#define TEXTURE_GROUP_PRECACHED						        "Precached"
#define TEXTURE_GROUP_CUBE_MAP						        "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					      "RenderTargets"
#define texture_group_unaccounted					        "Unaccounted textures"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			    "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			  "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			  "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					      "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					        "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					      "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				      "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			  "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					      "Morph Targets"

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------

class matrix_t;
class i_material;
class i_material_var;
typedef int override_type_t;

enum e_compiled_vt_flags
{
	textureflags_pointsample = 0x00000001,
	textureflags_trilinear = 0x00000002,
	textureflags_clamps = 0x00000004,
	textureflags_clampt = 0x00000008,
	textureflags_anisotropic = 0x00000010,
	textureflags_hint_dxt5 = 0x00000020,
	textureflags_pwl_corrected = 0x00000040,
	textureflags_normal = 0x00000080,
	textureflags_nomip = 0x00000100,
	textureflags_nolod = 0x00000200,
	textureflags_all_mips = 0x00000400,
	textureflags_procedural = 0x00000800,
	textureflags_onebitalpha = 0x00001000,
	textureflags_eightbitalpha = 0x00002000,
	textureflags_envmap = 0x00004000,
	textureflags_rendertarget = 0x00008000,
	textureflags_depthrendertarget = 0x00010000,
	textureflags_nodebugoverride = 0x00020000,
	textureflags_singlecopy = 0x00040000,
	textureflags_pre_srgb = 0x00080000,
	textureflags_unused_00100000 = 0x00100000,
	textureflags_unused_00200000 = 0x00200000,
	textureflags_unused_00400000 = 0x00400000,
	textureflags_nodepthbuffer = 0x00800000,
	textureflags_unused_01000000 = 0x01000000,
	textureflags_clampu = 0x02000000,
	textureflags_vertextexture = 0x04000000,
	textureflags_ssbump = 0x08000000,
	textureflags_unused_10000000 = 0x10000000,
	textureflags_border = 0x20000000,
	textureflags_unused_40000000 = 0x40000000,
	textureflags_unused_80000000 = 0x80000000
};

enum e_standardlightmap
{
	material_system_lightmap_page_white = -1,
	material_system_lightmap_page_white_bump = -2,
	material_system_lightmap_page_user_defined = -3
};

struct materialsystem_sortinfo_t
{
	i_material* material;
	int			lightmappageid;
};

enum e_materialthreadmode
{
	material_single_threaded,
	material_queued_single_threaded,
	material_queued_threaded
};

enum e_materialcontexttype
{
	material_hardware_context,
	material_queued_context,
	material_null_context
};

enum
{
	material_adapter_name_length = 512
};

struct materialtextureinfo_t
{
	int m_iexcludeinformation;
};

struct applicationperformancecountersinfo_t
{
	float m_msmain{};
	float m_msmst{};
	float m_msgpu{};
	float m_msflip{};
	float m_mstotal{};
};

struct applicationinstantcountersinfo_t
{
	uint32_t m_ncpuactivitymask{};
	uint32_t m_ndeferredwordsallocated{};
};

struct materialadapterinfo_t
{
	char m_pdrivername[material_adapter_name_length]{};
	unsigned int m_vendorid{};
	unsigned int m_deviceid{};
	unsigned int m_subsysid{};
	unsigned int m_revision{};
	int m_ndxsupportlevel{};			// this is the *preferred* dx support level
	int m_nmindxsupportlevel{};
	int m_nmaxdxsupportlevel{};
	unsigned int m_ndriverversionhigh{};
	unsigned int m_ndriverversionlow{};
};

struct materialvideomode_t
{
	int m_width{};			// if width and height are 0 and you select
	int m_height{};			// windowed mode, it'll use the window size
	e_image_format m_format{};	// use imageformats (ignored for windowed mode)
	int m_refreshrate{};		// 0 == default (ignored for windowed mode)
};
enum hdrtype_t
{
	hdr_type_none,
	hdr_type_integer,
	hdr_type_float,
};

enum restorechangeflags_t
{
	material_restore_vertex_format_changed = 0x1,
	material_restore_release_managed_resources = 0x2,
};

enum rendertargetsizemode_t
{
	rt_size_no_change = 0,
	rt_size_default = 1,
	rt_size_picmip = 2,
	rt_size_hdr = 3,
	rt_size_full_frame_buffer = 4,
	rt_size_offscreen = 5,
	rt_size_full_frame_buffer_rounded_up = 6
};

enum materialrendertargetdepth_t
{
	material_rt_depth_shared = 0x0,
	material_rt_depth_separate = 0x1,
	material_rt_depth_none = 0x2,
	material_rt_depth_only = 0x3,
};

typedef unsigned short materialhandle_t;

enum material_var_flags_t {
	material_var_debug = (1 << 0),
	material_var_no_debug_override = (1 << 1),
	material_var_no_draw = (1 << 2),
	material_var_use_in_fillrate_mode = (1 << 3),
	material_var_vertexcolor = (1 << 4),
	material_var_vertexalpha = (1 << 5),
	material_var_selfillum = (1 << 6),
	material_var_additive = (1 << 7),
	material_var_alphatest = (1 << 8),
	material_var_znearer = (1 << 10),
	material_var_model = (1 << 11),
	material_var_flat = (1 << 12),
	material_var_nocull = (1 << 13),
	material_var_nofog = (1 << 14),
	material_var_ignorez = (1 << 15),
	material_var_decal = (1 << 16),
	material_var_envmapsphere = (1 << 17), // obsolete
	material_var_unused = (1 << 18), // unused
	material_var_envmapcameraspace = (1 << 19), // obsolete
	material_var_basealphaenvmapmask = (1 << 20),
	material_var_translucent = (1 << 21),
	material_var_normalmapalphaenvmapmask = (1 << 22),
	material_var_needs_software_skinning = (1 << 23), // obsolete
	material_var_opaquetexture = (1 << 24),
	material_var_envmapmode = (1 << 25), // obsolete
	material_var_suppress_decals = (1 << 26),
	material_var_halflambert = (1 << 27),
	material_var_wireframe = (1 << 28),
	material_var_allowalphatocoverage = (1 << 29),
	material_var_alpha_modified_by_proxy = (1 << 30),
	material_var_vertexfog = (1 << 31),
};

enum preview_image_retval_t {
	material_preview_image_bad = 0,
	material_preview_image_ok,
	material_no_preview_image,
};

class i_material_var {

	void set_vector_internal(const float x, const float y)
	{
		using o_fn = void(__thiscall*)(void*, float, float);
		return utils::call_vfunc<o_fn>(this, 10)(this, x, y);
	}

public:
	void set_float(const float val)
	{
		using o_fn = void(__thiscall*)(void*, float);
		return utils::call_vfunc<o_fn>(this, 4)(this, val);
	}

	void set_int(const int val)
	{
		using o_fn = void(__thiscall*)(void*, int);
		return utils::call_vfunc<o_fn>(this, 5)(this, val);
	}

	void set_string(char const* val)
	{
		using o_fn = void(__thiscall*)(void*, char const*);
		return utils::call_vfunc<o_fn>(this, 6)(this, val);
	}

	void set_matrix(matrix_t& matrix)
	{
		using o_fn = void(__thiscall*)(void*, matrix_t&);
		return utils::call_vfunc<o_fn>(this, 6)(this, matrix);
	}

	void set_vector_component(const float val, const int comp)
	{
		using o_fn = void(__thiscall*)(void*, float, int);
		return utils::call_vfunc<o_fn>(this, 26)(this, val, comp);
	}

	void set_vector(const c_vec2 vector)
	{
		set_vector_internal(vector.x, vector.y);
	}

	void set_vector_internal(const float x, const float y, const float z)
	{
		using o_fn = void(__thiscall*)(void*, float, float, float);
		return utils::call_vfunc<o_fn>(this, 11)(this, x, y, z);
	}

	void set_vector(const c_vec3 vector)
	{
		set_vector_internal(vector.x, vector.y, vector.z);
	}
};

typedef int vertex_format_t;
typedef int material_property_types_t;

class i_material {
public:
	virtual const char* get_name() const = 0;
	virtual const char* get_texture_group_name() const = 0;
	virtual preview_image_retval_t get_preview_image_properties(int* width, int* height, e_image_format* imageFormat, bool* isTranslucent) const = 0;
	virtual preview_image_retval_t get_preview_image(unsigned char* data, int width, int height, e_image_format imageFormat) const = 0;
	virtual int get_mapping_width() = 0;
	virtual int get_mapping_height() = 0;
	virtual int get_num_animation_frames() = 0;
	virtual bool in_material_page(void) = 0;
	virtual void get_material_offset(float* pOffset) = 0;
	virtual void get_material_scale(float* pScale) = 0;
	virtual i_material* get_material_page(void) = 0;
	virtual i_material_var* find_var(const char* varName, bool* found, bool complain = true) = 0;
	virtual void increment_reference_count(void) = 0;
	virtual void decrement_reference_count(void) = 0;
	virtual int get_enumeration_id(void) const = 0;
	virtual void get_low_res_color_sample(float s, float t, float* color) const = 0;
	virtual void recompute_state_snapshots() = 0;
	virtual bool is_translucent() = 0;
	virtual bool is_alpha_tested() = 0;
	virtual bool is_vertex_lit() = 0;
	virtual vertex_format_t get_vertex_format() const = 0;
	virtual bool has_proxy(void) const = 0;
	virtual bool uses_env_cubemap(void) = 0;
	virtual bool needs_tangent_space(void) = 0;
	virtual bool needs_power_of_two_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool needs_full_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool needs_software_skinning(void) = 0;
	virtual void alpha_modulate(float alpha) = 0;
	virtual void color_modulate(float r, float g, float b) = 0;
	virtual void set_material_var_flag(material_var_flags_t flag, bool on) = 0;
	virtual bool get_material_var_flag(material_var_flags_t flag) const = 0;
	virtual void get_reflectivity(c_vec3& reflect) = 0;
	virtual bool get_property_flag(material_property_types_t  type) = 0;
	virtual bool is_two_sided() = 0;
	virtual void set_shader(const char* pShaderName) = 0;
	virtual int get_num_passes(void) = 0;
	virtual int get_texture_memory_bytes(void) = 0;
	virtual void refresh() = 0;
	virtual bool needs_lightmap_blend_alpha(void) = 0;
	virtual bool needs_software_lighting(void) = 0;
	virtual int shader_param_count() const = 0;
	virtual i_material_var** get_shader_params(void) = 0;
	virtual bool is_error_material() const = 0;
	virtual void unused() = 0;
	virtual float get_alpha_modulation() = 0;
	virtual void get_color_modulation(float* r, float* g, float* b) = 0;
	virtual bool is_translucent_under_modulation(float fAlphaModulation = 1.0f) const = 0;
	virtual i_material_var* find_var_fast(char const* pVarName, unsigned int* pToken) = 0;
	virtual void set_shader_and_params(c_key_values* pKeyValues) = 0;
	virtual const char* get_shader_name() const = 0;
	virtual void delete_if_unreferenced() = 0;
	virtual bool is_sprite_card() = 0;
	virtual void call_bind_proxy(void* proxyData) = 0;
	virtual void refresh_preserving_material_vars() = 0;
	virtual bool was_reloaded_from_whitelist() = 0;
	virtual bool set_temp_excluded(bool bSet, int nExcludedDimensionLimit) = 0;
	virtual int get_reference_count() const = 0;
};

enum light_type_t {
	material_light_disable = 0,
	material_light_point,
	material_light_directional,
	material_light_spot,
};

class light_desc_t
{
public:
	light_type_t m_type;                 //0x0000
	c_vec3 m_color{};                     //0x0004
	c_vec3 m_position{};                  //0x0010
	c_vec3 m_direction{};                 //0x001C
	float m_range{};                      //0x0028
	float m_falloff{};                    //0x002C
	float m_attenuation0{};               //0x0030
	float m_attenuation1{};               //0x0034
	float m_attenuation2{};               //0x0038
	float m_theta{};                      //0x003C
	float m_phi{};                        //0x0040
	float m_theta_dot{};                   //0x0044
	float m_phi_dot{};                     //0x0048
	float m_one_over_theta_dot_minus_phi_dot{}; //0x004C
	__int32 m_flags{};                    //0x0050
	float m_range_squared{};               //0x0054
};

class i_studio_render
{
public:
	void set_ambient_light_colors(vec_t* pAmbientOnlyColors)
	{
		utils::call_vfunc<void(__thiscall*)(void*, vec_t*)>(this, 20)(this, pAmbientOnlyColors);
	}

	void set_local_lights(int nLights, light_desc_t* pLights)
	{
		utils::call_vfunc<void(__thiscall*)(void*, int, light_desc_t*)>(this, 22)(this, nLights, pLights);
	}

	void set_color_modulation(float r, float g, float b)
	{
		utils::call_vfunc<void(__thiscall*)(void*, float, float, float)>(this, 27)(this, r, g, b);
	}

	void set_alpha_modulation(float flAlpha)
	{
		utils::call_vfunc<void(__thiscall*)(void*, float)>(this, 28)(this, flAlpha);
	}

	void forced_material_override(i_material* pMaterial, override_type_t nOverrideType = 0, int nOverrides = 0)
	{
		utils::call_vfunc<void(__thiscall*)(void*, i_material*, override_type_t, int)>(this, 33)(this, pMaterial, nOverrideType, nOverrides);
	}
};

class i_material_system
{
public:
	i_material* create_material(const char* name, void* key_values)
	{
		return utils::call_vfunc<i_material*(__thiscall*)(void*, const char*, void*)>(this, 83)(this, name, key_values);
	}

	i_material* find_material(const char* name, const char* group = nullptr) 
	{
		return utils::call_vfunc<i_material*(__thiscall*)(void*, const char*, const char*)>(this, 84)(this, name, group);
	}

	materialhandle_t first_material() 
	{
		return utils::call_vfunc<materialhandle_t(__thiscall*)(i_material_system*)>(this, 86)(this);
	}

	materialhandle_t next_material(materialhandle_t handle) 
	{
		return utils::call_vfunc<materialhandle_t(__thiscall*)(i_material_system*, materialhandle_t)>(this, 87)(this, handle);
	}

	materialhandle_t invalid_material_handle() 
	{
		return utils::call_vfunc<materialhandle_t(__thiscall*)(i_material_system*)>(this, 88)(this);
	}

	i_material* get_material(materialhandle_t handle) 
	{
		return utils::call_vfunc<i_material*(__thiscall*)(i_material_system*, materialhandle_t)>(this, 89)(this, handle);
	}
};
