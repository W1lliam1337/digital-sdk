#pragma once

class c_base_entity;

class i_move_helper
{
	virtual void unknown_virtual() = 0;
public:
	virtual void set_host(c_base_entity* host) = 0;
private:
	virtual void pad00() = 0;
	virtual void pad01() = 0;
public:
	virtual void process_impacts() = 0;
};
