#include "hooks.h"

void hooks::init()
{
	if (MH_Initialize() != MH_OK)
		return;

	init_wnd_proc();

	static const auto create_move = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(interfaces::m_client))[22]);
	static const auto paint_traverse = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(interfaces::m_panel))[41]);
	static const auto reset = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(interfaces::m_direct_device))[16]);
	static const auto present = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(interfaces::m_direct_device))[17]);
	static const auto lock_cursor = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(interfaces::m_surface))[67]);
	static const auto is_paused = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(interfaces::m_engine))[90]);
	static const auto is_hltv = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(interfaces::m_engine))[93]);
	static const auto dme = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(interfaces::m_model_render))[21]);
	static const auto override_view = static_cast<void*>(utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F8 8B 4D 04 83 EC 58")));
	static const auto modify_eye_position = static_cast<void*>(utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 14 83 7F 60")));
	static const auto calculate_view = static_cast<void*>(utils::sig(modules::m_client_dll, _("55 8B EC 83 EC 14 53 56 57 FF 75 18")));
	static const auto blood_callback = static_cast<void*>(utils::sig(modules::m_client_dll, _("55 8B EC 8B 4D 08 F3 0F 10 51 ? 8D 51 18")));
	static const auto inferno_client_think = static_cast<void*>(utils::sig(modules::m_client_dll, _("55 8B EC 83 EC 20 53 56 57 8B F9 8B 0D ? ? ? ? 8B 81 ? ? ? ? 89 45 EC")));
	static const auto grenade_trail = static_cast<void*>(utils::sig(modules::m_client_dll, _("E9 ? ? ? ? 5E C3 CC 55 8B EC 83 E4 F8")));
	static const auto do_extra_bone_processing = static_cast<void*>(utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C")));
	static const auto setup_bones = static_cast<void*>(utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F0 B8 D8")));
	static const auto standard_blending_rules = static_cast<void*>(utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6")));
	static const auto build_transformations = static_cast<void*>(utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F0 81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 28")));
	static const auto should_skip_anim_frame = static_cast<void*>(utils::sig(modules::m_client_dll, _("57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02")));

	HOOK(create_move, hk_create_move_proxy, og::m_create_move);
	HOOK(reset, hk_reset, og::m_reset);
	HOOK(present, hk_present, og::m_present);
	HOOK(lock_cursor, hk_lock_cursor, og::m_lock_cursor);
	HOOK(paint_traverse, hk_paint_traverse, og::m_paint_traverse);
	HOOK(override_view, hk_override_view, og::m_override_view);
	HOOK(modify_eye_position, hk_modify_eye_position, hooks::og::m_modify_eye_position);
	HOOK(calculate_view, hk_calculate_view, og::m_calculate_view);
	HOOK(blood_callback, hk_blood_spray_callback, og::m_blood_spray_callback);
	HOOK(inferno_client_think, hk_inferno_client_think, og::m_inferno_client_think);
	HOOK(do_extra_bone_processing, hk_do_extra_bone_processing, og::m_do_extra_bone_processing);
	HOOK(setup_bones, hk_setup_bones, og::m_setup_bones);
	HOOK(build_transformations, hk_build_transformations, og::m_build_transformations);
	HOOK(should_skip_anim_frame, hk_should_skip_anim_frame, og::m_should_skip_anim_frame);
	HOOK(standard_blending_rules, hk_standard_blending_rules, og::m_standard_blending_rules);
	HOOK(is_paused, hk_is_paused, og::m_is_paused);
	HOOK(is_hltv, hk_is_hltv, og::m_is_hltv);
	HOOK(dme, hk_draw_model_execute, og::m_draw_model_execute);

	printf("targets size: %u\n", hooks::m_targets.size());
}

void hooks::unhook(const LPVOID target)
{
	printf(MH_DisableHook(target) == MH_OK
		? "disabled target: 0x%p\n"
		: "not disabled target: 0x%p\n", target);
}

void hooks::disable_all_hooks()
{
	for (const auto target : m_targets) { unhook(target); }
	MH_Uninitialize();
}