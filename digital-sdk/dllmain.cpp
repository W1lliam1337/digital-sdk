#include "sdk/sdk.hpp"
#include "sdk/interfaces/interfaces.h"
#include "dependencies/modules/modules.h"
#include "sdk/utils/netvar manager/netvar_manager.h"
#include "cheat/hooks/hooks.h"
#include "dependencies/config manager/config_manager.h"
#include "dependencies/render/render.h"

int instance()
{
	/* waiting for serverbrowser.dll module */
	while (!GetModuleHandleA(_("serverbrowser.dll")))
		std::this_thread::sleep_for(std::chrono::seconds(5));

	/* open console */
	AllocConsole();
	if (!freopen(_("CONOUT$"), _("w"), stdout))
	{
		FreeConsole();
		return EXIT_SUCCESS;
	}

	/* set title */
	SetConsoleTitleA(_("csgo-sdk console"));

	printf(_("hello world! credits to sdk:\n @tg: https://t.me/kernel_mode2\n @ds: william_coder#8276\n @github: https://github.com/W1lliam1337\n"));

	printf(_("init module list...\n"));
	c_modules::get()->init();

	printf(_("init interface list...\n"));
	c_interfaces::get()->init();

	printf(_("init netvars...\n"));
	c_netvars::get()->init();

	printf(_("init render...\n"));
	c_render::get()->init();

	//printf(_("init config system...\n"));
	//c_config_manager::get()->test();

	printf(_("init hooks...\n"));
	c_hooks::get()->init();

	FreeConsole();
	return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	DisableThreadLibraryCalls(hModule);
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return FALSE;

	//MessageBoxA(nullptr, "Attached", nullptr, 0);

	if (const HANDLE thread = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(instance), hModule, NULL, nullptr))
		CloseHandle(thread);

	return TRUE;
}

