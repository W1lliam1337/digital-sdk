#include "interfaces.h"

void* interfaces::get_interface(const HMODULE module, const char* interface_name) 
{
	return reinterpret_cast<create_interface_fn>(
		GetProcAddress(module, _("CreateInterface")))(interface_name, nullptr);
}

void interfaces::init()
{
	m_client = static_cast<i_base_client_dll*>(get_interface(
		modules::m_client_dll, _("VClient018")));

	m_engine = static_cast<i_engine*>(get_interface(
		modules::m_engine_dll, _("VEngineClient014")));

	m_entity_list = static_cast<i_client_entity_list*>(get_interface(
		modules::m_client_dll, _("VClientEntityList003")));

	m_debug_overlay = static_cast<i_debug_overlay*>(get_interface(
		modules::m_engine_dll, _("VDebugOverlay004")));

	m_surface = static_cast<i_surface*>(get_interface(
		modules::m_vgui_dll, _("VGUI_Surface031")));

	m_trace = static_cast<i_engine_trace*>(get_interface(
		modules::m_engine_dll, _("EngineTraceClient004")));

	m_cvar = static_cast<i_cvar*>(get_interface(
		modules::m_vstd_dll, _("VEngineCvar007")));

	m_physics_surface_props = static_cast<i_physics_surface_props*>(get_interface(
		modules::m_physics_dll, _("VPhysicsSurfaceProps001")));

	m_prediction = static_cast<i_prediction*>(get_interface(
		modules::m_client_dll, _("VClientPrediction001")));

	m_game_movement = static_cast<i_game_movement*>(get_interface(
		modules::m_client_dll, _("GameMovement001")));

	m_mdl_cache = static_cast<i_mdl_cache*>(get_interface(
		modules::m_data_cache_dll, _("MDLCache004")));

	m_model_info = static_cast<i_model_info*>(get_interface(
		modules::m_engine_dll, _("VModelInfoClient004")));

	m_material_system = static_cast<i_material_system*>(get_interface(
		modules::m_mat_sys_dll, _("VMaterialSystem080")));
	
	m_studio_render = static_cast<i_studio_render*>(get_interface(
		modules::m_studio_render_dll, _("VStudioRender026")));

	m_model_render = static_cast<i_model_render*>(get_interface(
		modules::m_engine_dll, _("VEngineModel016")));

	m_render_view = static_cast<i_render_view*>(get_interface(
		modules::m_engine_dll, _("VEngineRenderView014")));

	m_event_manager = static_cast<i_game_event_manager*>(get_interface(
		modules::m_engine_dll, _("GAMEEVENTSMANAGER002")));

	m_panel = static_cast<i_panel*>(get_interface(
		modules::m_vgui2_dll, _("VGUI_Panel009")));

	m_mem_alloc = *reinterpret_cast<i_mem_alloc**>(GetProcAddress(modules::m_tier_dll, _("g_pMemAlloc")));

	m_client_state = **reinterpret_cast<i_client_state***>(utils::sig(
		modules::m_engine_dll, _("A1 ? ? ? ? 8B 80 ? ? ? ? C3")) 
		+ 0x1);

	m_input = *reinterpret_cast<i_input**>(utils::sig(
		modules::m_client_dll, _("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"))
		+ 0x1);

	m_move_helper = **reinterpret_cast<i_move_helper***>(utils::sig(modules::m_client_dll,
		_("8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01")) + 0x2);

	m_direct_device = **reinterpret_cast<IDirect3DDevice9***>(utils::sig(
		modules::m_shader_dll,
		_("A1 ? ? ? ? 50 8B 08 FF 51 0C")) + 0x1);

	m_globals = **reinterpret_cast<i_global_vars_base***>(utils::sig(
		modules::m_client_dll, _("A1 ? ? ? ? 5E 8B 40 10")) +
		0x1);
}