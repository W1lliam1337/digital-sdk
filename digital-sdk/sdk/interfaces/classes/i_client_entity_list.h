#pragma once

#include "../../other/handle/base_handle.h"

class i_client_networkable;
class i_client_entity;

class i_client_entity_list {
public:
	virtual i_client_networkable* get_client_networkable( int entnum ) = 0;
	virtual void* vtablepad0x1( void ) = 0;
	virtual void* vtablepad0x2( void ) = 0;
	virtual i_client_entity* get_client_entity( int ent_num ) = 0;
	virtual i_client_entity* get_client_entity_from_handle( c_base_handle ent ) = 0;
	virtual int number_of_entities( bool include_non_networkable ) = 0;
	virtual int get_highest_entity_index( void ) = 0;
	virtual void set_max_entities( int max_ents ) = 0;
	virtual int get_max_entities( ) = 0;
};