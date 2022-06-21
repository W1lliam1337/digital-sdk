#pragma once
#include "../../../../sdk/sdk.hpp"

class c_movement : public c_singleton<c_movement>
{
public:
	static void bunny_hop();
	void auto_strafer();
};

