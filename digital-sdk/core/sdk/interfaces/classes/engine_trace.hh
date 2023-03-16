#ifndef VALVE_ENGINE_TRACE
#define VALVE_ENGINE_TRACE

#include "../../misc/game_trace.hh"
#include "../../misc/client_entity.hh"

enum e_surface_flags {
    dispsurf_flag_surface = 1 << 0,
    dispsurf_flag_walkable = 1 << 1,
    dispsurf_flag_buildable = 1 << 2,
    dispsurf_flag_surfprop1 = 1 << 3,
    dispsurf_flag_surfprop2 = 1 << 4,
};

enum e_contents {
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

enum e_surf {
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

enum e_masks : std::uintptr_t {
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

enum trace_type_t {
    trace_everything = 0,
    trace_world_only,
    trace_entities_only,
    trace_everything_filter_props,
};

class i_trace_filter {
public:
    virtual bool should_hit_entity( void* entity, int contents_mask ) = 0;
    virtual trace_type_t get_trace_type( ) const = 0;
};

class c_trace_filter : public i_trace_filter {
public:
    [[nodiscard]] trace_type_t get_trace_type( ) const override { return trace_everything; }
};

class i_engine_trace {
public:
    virtual int get_point_contents( const vec3_t& abs_position, int contents_mask = mask_all,
                                    c_handle_entity** entity = nullptr ) = 0;
    virtual int get_point_contents_world_only( const vec3_t& abs_position, int contents_mask = mask_all ) = 0;
    virtual int get_point_contents_collideable( c_collideable* collide, const vec3_t& abs_position ) = 0;
    virtual void clip_ray_to_entity( const ray_t& ray, unsigned int mask, c_handle_entity* ent,
                                     c_game_trace* trace ) = 0;
    virtual void clip_ray_to_collideable( const ray_t& ray, unsigned int mask, c_collideable* collide,
                                          c_game_trace* trace ) = 0;
    virtual void trace_ray( const ray_t& ray, unsigned int mask, i_trace_filter* trace_filter,
                            c_game_trace* trace ) = 0;
};

#endif