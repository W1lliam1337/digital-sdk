#pragma once
#include "i_client.h"
#include "i_model_render.h"
#include "../../other/bfread/bfread.h"
#include "../../other/handle/i_handle_entity.h"

class matrix_t;
struct qangle_t;
class c_vec3;

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

class i_client_unknown;
class c_game_trace;
class c_base_handle;
struct ray_t;

class i_collideable {
public:
	virtual i_handle_entity* get_entity_handle() = 0;
	virtual c_vec3& obb_mins() const = 0;
	virtual c_vec3& obb_maxs() const = 0;
	virtual void               world_space_trigger_bounds(c_vec3* mins, c_vec3* maxs) const = 0;
	virtual bool               test_collision(const ray_t& ray, unsigned int mask, c_game_trace& trace) = 0;
	virtual bool               test_hitboxes(const ray_t& ray, unsigned int mask, c_game_trace& trace) = 0;
	virtual int                get_collision_model_index() = 0;
	virtual const model_t* get_collision_model() = 0;
	virtual const c_vec3& get_collision_origin() const = 0;
	virtual const qangle_t& get_collision_angles() const = 0;
	virtual const matrix_t& collision_to_world_transform() const = 0;
	virtual e_solid_type			get_solid() const = 0;
	virtual int                get_solid_flags() const = 0;
	virtual i_client_unknown* get_client_unknown() = 0;
	virtual int                get_collision_group() const = 0;
	virtual void               world_space_surrounding_bounds(c_vec3* mins, c_vec3* maxs) = 0;
	virtual bool               should_touch_trigger(int flags) const = 0;
	virtual const matrix_t* get_root_parent_to_world_transform() const = 0;
};

class i_client_thinkable {
public:
	virtual i_client_unknown* get_client_unknown() = 0;
	virtual void					client_think() = 0;
	virtual void* get_think_handle() = 0;
	virtual void					set_think_handle(void* think) = 0;
	virtual void					release() = 0;
};

class i_client_networkable {
public:
	virtual i_client_unknown* get_client_unknown() = 0;
	virtual void				release() = 0;
	virtual client_class_t* get_client_class() = 0;
	virtual void				notify_should_transmit(int state) = 0;
	virtual void				on_pre_data_changed(int update_type) = 0;
	virtual void				on_data_changed(int update_type) = 0;
	virtual void				pre_data_update(int update_type) = 0;
	virtual void				post_data_update(int update_type) = 0;
	virtual void				unknown0() = 0;
	virtual bool				is_dormant() = 0;
	virtual int					get_index() const = 0;
	virtual void				receive_message(int class_id, c_bf_read& msg) = 0;
	virtual void* get_data_table_base_ptr() = 0;
	virtual void				set_destroyed_on_recreate_entities() = 0;
};

class i_client_renderable {
public:
	virtual i_client_unknown* get_client_unknown() = 0;
	virtual c_vec3 const& get_render_origin() = 0;
	virtual qangle_t const& get_render_angles() = 0;
	virtual bool					should_draw() = 0;
	virtual int						get_render_flags() = 0;
	virtual void					unknown0() const = 0;
	virtual uint16_t				get_shadow_handle() const = 0;
	virtual uint16_t& get_render_handle() = 0;
	virtual const model_t* get_model() const = 0;
	virtual int						draw_model(int flags, uint8_t alpha) = 0;
	virtual int						get_body() = 0;
	virtual void					get_color_modulation(float* color) = 0;
	virtual bool					lod_test() = 0;
	virtual bool					setup_bones(matrix_t* bones, int max, int mask, float time) = 0;
	virtual void					setup_weights(const matrix_t* bones, int count, float* weights, float* delayed_weights) = 0;
	virtual void					do_animation_events() = 0;
	virtual void* get_pvs_notify_interface() = 0;
	virtual void					get_render_bounds(c_vec3& mins, c_vec3& maxs) = 0;
	virtual void					get_render_bounds_worldspace(c_vec3& mins, c_vec3& maxs) = 0;
	virtual void					get_shadow_render_bounds(c_vec3& mins, c_vec3& maxs, int type) = 0;
	virtual bool					should_receive_projected_textures(int flags) = 0;
	virtual bool					get_shadow_cast_distance(float* dist, int type) const = 0;
	virtual bool					get_shadow_cast_direction(c_vec3* direction, int type) const = 0;
	virtual bool					is_shadow_dirty() = 0;
	virtual void					mark_shadow_dirty(bool dirty) = 0;
	virtual i_client_renderable* get_shadow_parent() = 0;
	virtual i_client_renderable* first_shadow_child() = 0;
	virtual i_client_renderable* next_shadow_peer() = 0;
	virtual int						shadow_cast_type() = 0;
	virtual void					create_model_instance() = 0;
	virtual uint16_t				get_model_instance() = 0;
	virtual const matrix_t& renderable_to_world_transform() = 0;
	virtual int						lookup_attachment(const char* name) = 0;
	virtual bool					get_attachment(int index, c_vec3& origin, c_vec3& angles) = 0;
	virtual bool					get_attachment(int index, matrix_t& matrix) = 0;
	virtual float* get_render_clip_plane() = 0;
	virtual int						get_skin() = 0;
	virtual void					on_threaded_draw_setup() = 0;
	virtual bool					uses_flex_delayed_weights() = 0;
	virtual void					record_tool_message() = 0;
	virtual bool					should_draw_for_split_screen_user(int slot) = 0;
	virtual uint8_t					override_alpha_modulation(uint8_t alpha) = 0;
	virtual uint8_t					override_shadow_alpha_modulation(uint8_t alpha) = 0;
};

class c_base_entity;
class c_base_player;
class i_client_entity;
class i_client_alpha_property;

class i_client_unknown : public i_handle_entity
{
public:
	virtual i_collideable* get_collideable() = 0;
	virtual i_client_networkable* get_client_networkable() = 0;
	virtual i_client_renderable* get_client_renderable() = 0;
	virtual i_client_entity* get_client_entity() = 0;
	virtual c_base_entity* get_base_entity() = 0;
	virtual i_client_thinkable* get_client_thinkable() = 0;
	virtual i_client_alpha_property* get_client_alpha_property() = 0;
};

class i_client_entity : public i_client_unknown, public i_client_renderable, public i_client_networkable, public i_client_thinkable {
public:
	virtual void release() = 0;
	virtual const c_vec3& get_abs_origin() const = 0;
	virtual const qangle_t& get_abs_angles() const = 0;
};