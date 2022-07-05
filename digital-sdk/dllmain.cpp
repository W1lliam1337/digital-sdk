#include "includes.h"
#include "utils/netvar-manager/netvar_manager.h"
#include "hooks/hooks.h"
#include "features/misc/events/events.h"

DWORD WINAPI instance()
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
    SetConsoleTitleA(_("csgo-sdk console\n"));

    printf(_("hello world! credits to sdk:\n @tg: https://t.me/kernel_mode2\n @ds: william_coder#8276\n @github: https://github.com/W1lliam1337\n"));

    printf(_("init module list...\n"));
    g_modules.init();

    printf(_("init interfaces...\n"));
    g_interfaces.init();

    printf(_("init render...\n"));
    g_render.init();

	printf(_("init render...\n"));
    g_netvars.init();

    printf(_("init hooks...\n"));
    g_hooks.init();

    printf(_("init events...\n"));
    g_events.init();

    FreeConsole();
    return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call != DLL_PROCESS_ATTACH)
	    return TRUE;

    DisableThreadLibraryCalls(hModule);

    const HANDLE handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(instance), hModule, 0,
                                       nullptr);

    if (!handle)
	    return FALSE;

    CloseHandle(handle);

    return TRUE;
}