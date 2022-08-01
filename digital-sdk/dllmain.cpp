#include "includes.h"
#include "utils/netvar-manager/netvar_manager.h"
#include "cheat/hooks/hooks.h"
#include "cheat/features/misc/events/events.h"

DWORD WINAPI instance()
{
	while (!LoadLibraryA(_("serverbrowser.dll")))
		std::this_thread::sleep_for(std::chrono::seconds(2));

	AllocConsole();
	if (!freopen(_("CONOUT$"), _("w"), stdout))
	{
		FreeConsole();
		return EXIT_SUCCESS;
	}

	SetConsoleTitleA(_("csgo-sdk console\n"));

	printf(_(
		"Hello, World! Credits to sdk:\n @tg: https://t.me/kernel_mode2\n @ds: william_coder#8276\n @github: https://github.com/W1lliam1337\n"));

	modules::init();
	printf(_("module list initialization was successful\n"));

	interfaces::init();
	printf(_("interfaces initialization was successful\n"));

	render::init();
	printf(_("render initialization was successful\n"));

	net_vars::init();
	printf(_("netvars initialization was successful\n"));

	chams::init_materials();
	printf(_("chams materials initialization was successful\n"));

	hooks::init();
	printf(_("hooks initialization was successful\n"));

	g_events->init();
	printf(_("events initialization was successful\n"));

	FreeConsole();
	return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain(HMODULE hmodule,
                      DWORD reason_for_call,
                      LPVOID reserved
)
{
	if (reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	/* @xref: https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-disablethreadlibrarycalls */
	DisableThreadLibraryCalls(hmodule);

	std::jthread([]() -> void { instance(); }).detach();

	return TRUE;
}