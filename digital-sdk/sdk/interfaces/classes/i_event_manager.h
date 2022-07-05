#pragma once
#include <cstdint>

class bf_write;
class bf_read;

class i_game_event
{
public:
	virtual					~i_game_event() {};
	virtual const char* get_name() const = 0;

	virtual bool			is_reliable() const = 0;
	virtual bool			is_local() const = 0;
	virtual bool			is_empty(const char* keyname = nullptr) = 0;

	virtual bool			get_bool(const char* keyname = nullptr, bool default_value = false) = 0;
	virtual int				get_int(const char* keyname = nullptr, int default_value = 0) = 0;
	virtual uint64_t		get_uint64(const char* keyname = nullptr, uint64_t default_value = 0) = 0;
	virtual float			get_float(const char* keyname = nullptr, float default_value = 0.0f) = 0;
	virtual const char* get_string(const char* keyname = nullptr, const char* default_value = "") = 0;
	virtual const wchar_t* get_wstring(const char* keyname = nullptr, const wchar_t* default_value = L"") = 0;
	virtual const void* get_ptr(const char* keyname = nullptr, const void* default_values = nullptr) = 0;

	virtual void			set_bool(const char* keyname, bool value) = 0;
	virtual void			set_int(const char* keyname, int value) = 0;
	virtual void			set_uint64(const char* keyname, uint64_t value) = 0;
	virtual void			set_float(const char* keyname, float value) = 0;
	virtual void			set_string(const char* keyname, const char* value) = 0;
	virtual void			set_wstring(const char* keyname, const wchar_t* value) = 0;
	virtual void			set_ptr(const char* keyname, const void* value) = 0;
};

class i_game_event_listener
{
public:
	virtual ~i_game_event_listener() {}

	virtual void fire_game_event(i_game_event* event_) = 0;
	virtual int get_event_debug_id() { return 42; }
};

class i_game_event_manager
{
public:
	virtual ~i_game_event_manager() = 0;
	virtual int load_events_from_file(const char* filename) = 0;
	virtual void reset() = 0;
	virtual bool add_listener(i_game_event_listener* listener, const char* name, bool server_side) = 0;
	virtual bool find_listener(i_game_event_listener* listener, const char* name) = 0;
	virtual int remove_listener(i_game_event_listener* listener) = 0;
	virtual i_game_event* create_event(const char* name, bool force, unsigned int unknown) = 0;
	virtual bool fire_event(i_game_event* event, bool dont_broadcast = false) = 0;
	virtual bool fire_event_clientside(i_game_event* event) = 0;
	virtual i_game_event* duplicate_event(i_game_event* event) = 0;
	virtual void free_event(i_game_event* event) = 0;
	virtual bool serialize_event(i_game_event* event, bf_write* buf) = 0;
	virtual i_game_event* unserialize_event(bf_read* buf) = 0;
};