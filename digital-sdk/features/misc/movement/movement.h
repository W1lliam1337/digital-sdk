#pragma once
#include "../../includes.h"
#include "../../sdk/sdk.h"

class c_movement
{
public:
	static void bunny_hop();
};

inline const auto g_movement = std::make_unique<c_movement>();