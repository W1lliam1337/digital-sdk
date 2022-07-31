#pragma once
#include "../../../sdk/sdk.h"
#include "../../../includes.h"

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

namespace anti_aim
{
	inline int m_desync_side{};
	inline int m_manual_side{};
	void init();
	static void pitch();
	void yaw();
	static bool cant_work();
}