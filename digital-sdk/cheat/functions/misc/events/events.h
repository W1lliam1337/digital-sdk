#pragma once
#include "../../../sdk/sdk.hpp"

class c_events : public i_game_event_listener
{
public:
	void player_hurt(i_game_event* event, const char* event_name) const;
	void player_death(i_game_event* event, const char* event_name) const;
	void fire_game_event(i_game_event* event);
	void init();
};

inline c_events g_events = c_events();