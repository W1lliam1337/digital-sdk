#pragma once
#include "../../../sdk/sdk.hpp"
#include "../../../dependencies/utils.h"

enum e_pitch_mode
{
	none,
	down,
	up
};

enum e_desync_side
{
	left,
	right
};

enum e_desync_mode
{
	custom,
	sway,
	opposite
};

class c_anti_aimbot : public c_singleton<c_anti_aimbot>
{
	int m_desync_side{};
	int m_manual_side{};
public:
	void init();
	static void pitch();
	void yaw();
	static bool cant_work();
};

