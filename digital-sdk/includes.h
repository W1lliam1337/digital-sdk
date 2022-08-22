#pragma once

#include <Windows.h>
#include <cstdint>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <deque>
#include <array>

#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_internal.h"
#include "dependencies/imgui/imgui_impl_dx9.h"
#include "dependencies/imgui/imgui_impl_win32.h"

#include "utils/utils.h"
#include "utils/xor.h"
#include "utils/modules/modules.h"
#include "utils/render/render.h"
#include "utils/config-manager/config_manager.h"

#include "sdk/math/vec3.h"
#include "sdk/math/vec4.h"
#include "sdk/math/qangle.h"
#include "sdk/math/matrix.h"
#include "sdk/interfaces/interfaces.h"
#include "sdk/other/color/color.h"
#include "sdk/math/math.h"
#include "sdk/other/studio/studio.h"
#include "sdk/other/move_data.h"
#include "sdk/other/utl/utl.h"
#include "sdk/other/handle/base_handle.h"

#include <MinHook.h>
#pragma comment( lib, "MinHook.lib" )