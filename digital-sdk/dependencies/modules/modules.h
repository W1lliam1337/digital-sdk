#pragma once
#include "../../sdk/sdk.hpp"
#include "../utils.h"

class c_modules : public c_singleton<c_modules>
{
public:
	void init();
};

