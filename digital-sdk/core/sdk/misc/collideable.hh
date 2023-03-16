#ifndef VALVE_COLLIDEABLE
#define VALVE_COLLIDEABLE

#include "../../utils/math/matrix.hh"
#include "../../utils/math/vector.hh"

class c_game_trace;
struct ray_t;
class c_client_unknown;
class c_handle_entity;

enum e_solid_type {
    solid_none,
    solid_bsp,
    solid_bbox,
    solid_obb,
    solid_obb_yaw,
    solid_custom,
    solid_vphysics,
    solid_last
};

struct model_t {
private:
    [[maybe_unused]] std::byte pad0[ 4 ]{ };
public:
    char m_name[ 260 ]{ };
    int m_load_flags{ };
    int m_server_count{ };
    int m_type{ };
    int m_flags{ };
    vec3_t m_mins{ };
    vec3_t m_maxs{ };
    float m_radius{ };
    void* m_key_values{ };
private:
    [[maybe_unused]] std::byte pad1[ 24 ]{ };
};

class c_collideable {
public:
    virtual c_handle_entity* get_entity_handle( ) = 0;
    virtual const vec3_t& obb_mins( ) const = 0;
    virtual const vec3_t& obb_maxs( ) const = 0;
    virtual void world_space_trigger_bounds( vec3_t* vec_world_mins, vec3_t* vec_world_maxs ) const = 0;
    virtual bool test_collision( const ray_t& ray, unsigned int contents_mask, c_game_trace& tr ) = 0;
    virtual bool test_hitboxes( const ray_t& ray, unsigned int contents_mask, c_game_trace& tr ) = 0;
    virtual int get_collision_model_index( ) = 0;
    virtual const model_t* get_collision_model( ) = 0;
    virtual const vec3_t& get_collision_origin( ) const = 0;
    virtual const vec3_t& get_collision_angles( ) const = 0;
    virtual const matrix3x4_t& collision_to_world_transform( ) const = 0;
    virtual e_solid_type get_solid( ) const = 0;
    virtual int get_solid_flags( ) const = 0;
    virtual c_client_unknown* get_iclient_unknown( ) = 0;
    virtual int get_collision_group( ) const = 0;
    virtual void world_space_surrounding_bounds( vec3_t* vec_mins, vec3_t* vec_maxs ) = 0;
    virtual int get_required_trigger_flags( ) const = 0;
    virtual const matrix3x4_t* get_root_parent_to_world_transform( ) const = 0;
    virtual void* get_vphysics_object( ) const = 0;
};

#endif