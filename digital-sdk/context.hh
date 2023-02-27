#ifndef CTX_H
#define CTX_H

#include <Windows.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <codecvt>
#include <tchar.h>
#include <Shlwapi.h>

#include "core/ent_listener/ent_listener.hh"
#include "core/sdk/net_vars/net_vars.hh"
#include "core/sdk/sdk.hh"
#include "core/utils/utils.hh"
#include "deps/deps.hh"

#include <d3d9.h>
#pragma comment( lib, "d3d9.lib" )

#include <MinHook.h>
#pragma comment( lib, "MinHook.lib" )

class c_ctx final {
public:
    struct dllmain_t {
        static bool create_thread( DWORD WINAPI function( void* ), void* parameter );
        static DWORD WINAPI load( void* param );
        bool m_unload{ };
    } m_dllmain;

    HWND m_hwnd{ };
    bool m_in_move{ }, m_send_packet{ };
    c_user_cmd* m_cmd{ };
};

inline const std::unique_ptr< c_ctx > g_ctx{ new c_ctx( ) };

#endif