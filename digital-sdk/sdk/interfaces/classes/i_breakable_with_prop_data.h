#pragma once

enum e_property_data_interactions : int
{
	propinter_physgun_world_stick,		// "onworldimpact"		"stick"
	propinter_physgun_first_break,		// "onfirstimpact"		"break"
	propinter_physgun_first_paint,		// "onfirstimpact"		"paintsplat"
	propinter_physgun_first_impale,		// "onfirstimpact"		"impale"
	propinter_physgun_launch_spin_none,	// "onlaunch"			"spin_none"
	propinter_physgun_launch_spin_z,	// "onlaunch"			"spin_zaxis"
	propinter_physgun_break_explode,	// "onbreak"			"explode_fire"
	propinter_physgun_break_explode_ice,// "onbreak"			"explode_ice"
	propinter_physgun_damage_none,		// "damage"				"none"
	propinter_fire_flammable,			// "flammable"			"yes"
	propinter_fire_explosive_resist,	// "explosive_resist"	"yes"
	propinter_fire_ignite_halfhealth,	// "ignite"				"halfhealth"
	propinter_physgun_create_flare,		// "onpickup"			"create_flare"
	propinter_physgun_allow_overhead,	// "allow_overhead"		"yes"
	propinter_world_bloodsplat,			// "onworldimpact",		"bloodsplat"
	propinter_physgun_notify_children,	// "onfirstimpact" - cause attached flechettes to explode
	propinter_melee_immune,				// "melee_immune"		"yes"
	propinter_num_interactions			// if we get more than 32 of these, we'll need a different system
};

enum e_multiplayer_break : int
{
	multiplayer_break_default,
	multiplayer_break_serverside,
	multiplayer_break_clientside,
	multiplayer_break_both
};

class i_breakable_with_prop_data
{
public:
	virtual void		set_dmg_mod_bullet(float dmg_mod) = 0;
	virtual void		set_dmg_mod_club(float dmg_mod) = 0;
	virtual void		set_dmg_mod_explosive(float dmg_mod) = 0;
	virtual float		get_dmg_mod_bullet() = 0;
	virtual float		get_dmg_mod_club() = 0;
	virtual float		get_dmg_mod_explosive() = 0;
	virtual float		get_dmg_mod_fire() = 0;
	virtual void		set_explosive_radius(float radius) = 0;
	virtual void		set_explosive_damage(float flDamage) = 0;
	virtual float		get_explosive_radius() = 0;
	virtual float		get_explosive_damage() = 0;
	virtual void		set_physics_damage_table(void* iszTableName) = 0;
	virtual void* get_physics_damage_table() = 0;
	virtual void		set_breakable_model(void* iszModel) = 0;
	virtual void* get_breakable_model() = 0;
	virtual void		set_breakable_skin(int skin) = 0;
	virtual int			get_breakable_skin() = 0;
	virtual void		set_breakable_count(int count) = 0;
	virtual int			get_breakable_count() = 0;
	virtual void		set_max_breakable_size(int size) = 0;
	virtual int			get_max_breakable_size() = 0;
	virtual void		set_prop_data_blocks_los(bool blocks_los) = 0;
	virtual void		set_prop_data_is_ai_walkable(bool blocks_los) = 0;
	virtual void		set_interaction(e_property_data_interactions Interaction) = 0;
	virtual bool		has_interaction(e_property_data_interactions Interaction) = 0;
	virtual void		set_physics_mode(int iMode) = 0;
	virtual int			get_physics_mode() = 0;
	virtual void		set_multiplayer_break_mode(e_multiplayer_break mode) = 0;
	virtual e_multiplayer_break get_multiplayer_break_mode() const = 0;
	virtual void		set_base_prop_data(void* iszBase) = 0;
	virtual void* get_base_prop_data() = 0;
};