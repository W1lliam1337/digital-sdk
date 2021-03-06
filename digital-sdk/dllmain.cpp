#include "includes.h"
#include "utils/netvar-manager/netvar_manager.h"
#include "hooks/hooks.h"
#include "features/misc/events/events.h"

DWORD WINAPI instance()
{
	/* waiting for serverbrowser.dll module */
	while (!LoadLibraryA(_("serverbrowser.dll")))
		std::this_thread::sleep_for(std::chrono::seconds(2));

	/* open console */
	AllocConsole();
	if (!freopen(_("CONOUT$"), _("w"), stdout))
	{
		FreeConsole();
		return EXIT_SUCCESS;
	}

	/* set title */
	SetConsoleTitleA(_("csgo-sdk console\n"));

	printf(_(
		"Hello, World! Credits to sdk:\n @tg: https://t.me/kernel_mode2\n @ds: william_coder#8276\n @github: https://github.com/W1lliam1337\n"));

	printf(_("init module list...\n"));
	g_modules->init();

	printf(_("init interfaces...\n"));
	g_interfaces->init();

	printf(_("init render...\n"));
	g_render->init();

	printf(_("init netvars...\n"));
	g_net_vars->init();

	printf(_("init hooks...\n"));
	g_hooks->init();

	printf(_("init events...\n"));
	g_events->init();

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

	/* @ref: https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-disablethreadlibrarycalls */
	DisableThreadLibraryCalls(hmodule);

	std::jthread([]() -> void { instance(); }).detach();

	return TRUE;
}