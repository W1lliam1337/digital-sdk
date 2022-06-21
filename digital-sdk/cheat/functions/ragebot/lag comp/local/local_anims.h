#pragma once
#include "../../../sdk/sdk.hpp"

class c_local_anims : public c_singleton<c_local_anims>
{
public:
	void init();
	static void fix_balance_adjust_animation();
};

