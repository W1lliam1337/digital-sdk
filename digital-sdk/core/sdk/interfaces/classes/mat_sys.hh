#ifndef VALVE_MAT_SYS
#define VALVE_MAT_SYS

#include "../../../utils/utils.hh"
#include <cstdint>

enum e_material_var_flags {
    material_var_debug = 1 << 0,
    material_var_no_debug_override = 1 << 1,
    material_var_no_draw = 1 << 2,
    material_var_use_in_fillrate_mode = 1 << 3,
    material_var_vertexcolor = 1 << 4,
    material_var_vertexalpha = 1 << 5,
    material_var_selfillum = 1 << 6,
    material_var_additive = 1 << 7,
    material_var_alphatest = 1 << 8,
    material_var_znearer = 1 << 10,
    material_var_model = 1 << 11,
    material_var_flat = 1 << 12,
    material_var_nocull = 1 << 13,
    material_var_nofog = 1 << 14,
    material_var_ignorez = 1 << 15,
    material_var_decal = 1 << 16,
    material_var_envmapsphere = 1 << 17,      // obsolete
    material_var_unused = 1 << 18,            // unused
    material_var_envmapcameraspace = 1 << 19, // obsolete
    material_var_basealphaenvmapmask = 1 << 20,
    material_var_translucent = 1 << 21,
    material_var_normalmapalphaenvmapmask = 1 << 22,
    material_var_needs_software_skinning = 1 << 23, // obsolete
    material_var_opaquetexture = 1 << 24,
    material_var_envmapmode = 1 << 25, // obsolete
    material_var_suppress_decals = 1 << 26,
    material_var_halflambert = 1 << 27,
    material_var_wireframe = 1 << 28,
    material_var_allowalphatocoverage = 1 << 29,
    material_var_alpha_modified_by_proxy = 1 << 30,
    material_var_vertexfog = 1 << 31,
};

enum e_preview_image_retval {
    material_preview_image_bad = 0,
    material_preview_image_ok,
    material_no_preview_image,
};

// fix-me: bitmap\imageformat_decl
enum e_image_format {
    image_format_unknown = -1,
    image_format_rgba8888 = 0,
    image_format_abgr8888,
    image_format_rgb888,
    image_format_bgr888,
    image_format_rgb565,
    image_format_i8,
    image_format_ia88,
    image_format_p8,
    image_format_a8,
    image_format_rgb888_bluescreen,
    image_format_bgr888_bluescreen,
    image_format_argb8888,
    image_format_bgra8888,
    image_format_dxt1,
    image_format_dxt3,
    image_format_dxt5,
    image_format_bgrx8888,
    image_format_bgr565,
    image_format_bgrx5551,
    image_format_bgra4444,
    image_format_dxt1_onebitalpha,
    image_format_bgra5551,
    image_format_uv88,
    image_format_uvwq8888,
    image_format_rgba16161616f,
    image_format_rgba16161616,
    image_format_uvlx8888,
    image_format_r32f,
    image_format_rgb323232f,
    image_format_rgba32323232f,
    image_format_nv_dst16,
    image_format_nv_dst24,
    image_format_nv_intz,
    image_format_nv_rawz,
    image_format_ati_dst16,
    image_format_ati_dst24,
    image_format_nv_null,
    image_format_ati2n,
    image_format_ati1n,
    image_format_dxt1_runtime,
    image_format_dxt5_runtime,
    num_image_formats
};

class i_material_var {
    VFUNC( void( __thiscall* )( void*, float, float ), set_vector_internal( const float x, const float y ), 10, x, y );
public:
    VFUNC( void( __thiscall* )( void*, float ), set_float( const float val ), 4, val );
    VFUNC( void( __thiscall* )( void*, int ), set_int( const int val ), 5, val );
    VFUNC( void( __thiscall* )( void*, char const* ), set_string( char const* val ), 6, val );
    VFUNC( void( __thiscall* )( void*, float, float, float ),
           set_vector_internal( const float x, const float y, const float z ), 11, x, y, z );
    VFUNC( void( __thiscall* )( void*, float, int ), set_vector_component( const float val, const int comp ), 26, val,
           comp );

    void set_vector( const vec2_t& vector ) { return set_vector_internal( vector.x, vector.y ); }
    void set_vector( const vec3_t& vector ) { return set_vector_internal( vector.x, vector.y, vector.z ); }
};

class i_material {
public:
    virtual const char* get_name( ) const = 0;
    virtual const char* get_texture_group_name( ) const = 0;
    virtual e_preview_image_retval get_preview_image_properties( int* width, int* height, e_image_format* image_format,
                                                                 bool* is_translucent ) const = 0;
    virtual e_preview_image_retval get_preview_image( unsigned char* data, int width, int height,
                                                      e_image_format image_format ) const = 0;
    virtual int get_mapping_width( ) = 0;
    virtual int get_mapping_height( ) = 0;
    virtual int get_num_animation_frames( ) = 0;
    virtual bool in_material_page( void ) = 0;
    virtual void get_material_offset( float* offset ) = 0;
    virtual void get_material_scale( float* scale ) = 0;
    virtual i_material* get_material_page( void ) = 0;
    virtual i_material_var* find_var( const char* name, bool* found, bool complain = true ) = 0;
    virtual void increment_reference_count( void ) = 0;
    virtual void decrement_reference_count( void ) = 0;
    virtual int get_enumeration_id( void ) const = 0;
    virtual void get_low_res_color_sample( float s, float t, float* color ) const = 0;
    virtual void recompute_state_snapshots( ) = 0;
    virtual bool is_translucent( ) = 0;
    virtual bool is_alpha_tested( ) = 0;
    virtual bool is_vertex_lit( ) = 0;
    virtual std::uint64_t get_vertex_format( ) const = 0;
    virtual bool has_proxy( void ) const = 0;
    virtual bool uses_env_cubemap( void ) = 0;
    virtual bool needs_tangent_space( void ) = 0;
    virtual bool needs_power_of_two_frame_buffer_texture( bool check_specific_to_this_frame = true ) = 0;
    virtual bool needs_full_frame_buffer_texture( bool check_specific_to_this_frame = true ) = 0;
    virtual bool needs_software_skinning( void ) = 0;
    virtual void alpha_modulate( float alpha ) = 0;
    virtual void color_modulate( float r, float g, float b ) = 0;
    virtual void set_material_var_flag( e_material_var_flags flag, bool on ) = 0;
    virtual bool get_material_var_flag( e_material_var_flags flag ) const = 0;
    virtual void get_reflectivity( vec3_t& reflect ) = 0;
    virtual bool get_property_flag( int type ) = 0;
    virtual bool is_two_sided( ) = 0;
    virtual void set_shader( const char* p_shader_name ) = 0;
    virtual int get_num_passes( void ) = 0;
    virtual int get_texture_memory_bytes( void ) = 0;
    virtual void refresh( ) = 0;
    virtual bool needs_lightmap_blend_alpha( void ) = 0;
    virtual bool needs_software_lighting( void ) = 0;
    virtual int shader_param_count( ) const = 0;
    virtual i_material_var** get_shader_params( void ) = 0;
    virtual bool is_error_material( ) const = 0;
    virtual void unused( ) = 0;
    virtual float get_alpha_modulation( ) = 0;
    virtual void get_color_modulation( float* r, float* g, float* b ) = 0;
    virtual bool is_translucent_under_modulation( float alpha_modulation = 1.0f ) const = 0;
    virtual i_material_var* find_var_fast( char const* var_name, unsigned int* token ) = 0;
    virtual void set_shader_and_params( void* key_values ) = 0;
    virtual const char* get_shader_name( ) const = 0;
    virtual void delete_if_unreferenced( ) = 0;
    virtual bool is_sprite_card( ) = 0;
    virtual void call_bind_proxy( void* proxy_data ) = 0;
    virtual void refresh_preserving_material_vars( ) = 0;
    virtual bool was_reloaded_from_whitelist( ) = 0;
    virtual bool set_temp_excluded( bool set, int excluded_dimension_limit ) = 0;
    virtual int get_reference_count( ) const = 0;
};

class i_material_system {
public:
    VFUNC( i_material*( __thiscall* ) ( void*, const char*, void* ),
           create_material( const char* name, void* key_values ), 83, name, key_values );
    VFUNC( i_material*( __thiscall* ) ( void*, const char*, const char*, bool, const char* ),
           find_material( const char* name, const char* group = nullptr, const bool complain = true,
                          const char* complain_prefix = nullptr ),
           84, name, group, complain, complain_prefix );
    VFUNC( unsigned short( __thiscall* )( i_material_system* ), first_material( ), 86 );
    VFUNC( unsigned short( __thiscall* )( i_material_system*, unsigned short ),
           next_material( const unsigned short handle ), 87, handle );
    VFUNC( i_material*( __thiscall* ) ( i_material_system*, unsigned short ),
           get_material( const unsigned short handle ), 89, handle );
};

#endif