#pragma once
#include "../../../../sdk/sdk.h"
#include "../../../../includes.h"

class c_events : public i_game_event_listener
{
public:
	void player_hurt(i_game_event* event, const char* event_name) const;
	void player_death(i_game_event* event, const char* event_name) const;
	void fire_game_event(i_game_event* event);
	void init() noexcept;
};

inline const auto g_events = std::make_unique<c_events>();