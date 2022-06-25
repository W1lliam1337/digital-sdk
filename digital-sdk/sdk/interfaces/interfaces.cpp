#include "interfaces.h"
#include "../../dependencies/utils.h"

void* c_interfaces::get_interface(const HMODULE module, const char* interface_name) const
{
	return reinterpret_cast<create_interface_fn>(
		GetProcAddress(module, _("CreateInterface")))(interface_name, nullptr);
}

void c_interfaces::init() const
{
	g_sdk.m_interfaces.m_client = static_cast<i_base_client_dll*>(this->get_interface(
		g_sdk.m_modules.m_client_dll, _("VClient018")));

	g_sdk.m_interfaces.m_engine = static_cast<i_engine*>(this->get_interface(
		g_sdk.m_modules.m_engine_dll, _("VEngineClient014")));

	g_sdk.m_interfaces.m_entity_list = static_cast<i_client_entity_list*>(this->get_interface(
		g_sdk.m_modules.m_client_dll, _("VClientEntityList003")));

	g_sdk.m_interfaces.m_prediction = static_cast<i_prediction*>(this->get_interface(
		g_sdk.m_modules.m_client_dll, _("VClientPrediction001")));

	g_sdk.m_interfaces.m_game_movement = static_cast<i_game_movement*>(get_interface(
		g_sdk.m_modules.m_client_dll, _("GameMovement001")));

	g_sdk.m_interfaces.m_model_cache = static_cast<i_mdl_cache*>(this->get_interface(
		g_sdk.m_modules.m_data_cache_dll, _("MDLCache004")));

	g_sdk.m_interfaces.m_surface = static_cast<i_surface*>(this->get_interface(
		g_sdk.m_modules.m_vgui_dll, _("VGUI_Surface031")));

	g_sdk.m_interfaces.m_debug_overlay = static_cast<i_debug_overlay*>(this->get_interface(
		g_sdk.m_modules.m_engine_dll, _("VDebugOverlay004")));

	g_sdk.m_interfaces.m_panel = static_cast<i_panel*>(this->get_interface(
		g_sdk.m_modules.m_vgui2_dll, _("VGUI_Panel009")));

	g_sdk.m_interfaces.m_model_info = static_cast<i_model_info*>(this->get_interface(
		g_sdk.m_modules.m_engine_dll, _("VModelInfoClient004")));

	g_sdk.m_interfaces.m_cvar = static_cast<i_cvar*>(this->get_interface(
		g_sdk.m_modules.m_vstd_dll, _("VEngineCvar007")));

	g_sdk.m_interfaces.m_physics_surface_props = static_cast<i_physics_surface_props*>(this->get_interface(
		g_sdk.m_modules.m_physics_dll, _("VPhysicsSurfaceProps001")));

	g_sdk.m_interfaces.m_trace = static_cast<c_engine_trace*>(this->get_interface(
		g_sdk.m_modules.m_engine_dll, _("EngineTraceClient004")));

	g_sdk.m_interfaces.m_event_manager = static_cast<i_game_event_manager*>(this->get_interface(
		g_sdk.m_modules.m_engine_dll, _("GAMEEVENTSMANAGER002")));

	g_sdk.m_interfaces.m_input = *reinterpret_cast<i_input**>(c_utils::find_sig(
		g_sdk.m_modules.m_client_dll, _("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"))
		+ 0x1);

	g_sdk.m_interfaces.m_globals = **reinterpret_cast<i_global_vars_base***>(c_utils::find_sig(
		g_sdk.m_modules.m_client_dll, _("A1 ? ? ? ? 5E 8B 40 10")) +
		0x1);

	g_sdk.m_interfaces.m_move_helper = **reinterpret_cast<i_move_helper***>(c_utils::find_sig(g_sdk.m_modules.m_client_dll,
		_("8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01")) + 0x2);

	g_sdk.m_interfaces.m_weapon_system = *reinterpret_cast<i_weapon_system**>(c_utils::find_sig(
		g_sdk.m_modules.m_client_dll, _("8B 35 ? ? ? ? FF 10 0F B7 C0")) + 0x2);

	g_sdk.m_interfaces.m_direct_device = **reinterpret_cast<IDirect3DDevice9***>(c_utils::find_sig(
		g_sdk.m_modules.m_shader_dll,
		_("A1 ? ? ? ? 50 8B 08 FF 51 0C")) + 0x1);
}
