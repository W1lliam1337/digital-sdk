#ifndef ENT_LIST_H
#define ENT_LIST_H

#include "../sdk/entities/player.hh"
#include <functional>

class c_ent_listener final {
    std::vector< c_entity* > m_players{ };
public:
    c_ent_listener( ) { this->m_players.reserve( 64 ); }

    ~c_ent_listener( ) = default;

    void add( c_entity* player );
    void remove( c_entity* player );
    void reset( ) { this->m_players.clear( ); }
    void loop( const std::function< void( c_entity* ) >& fn ) const;
};

inline const std::unique_ptr< c_ent_listener > g_listener{ new c_ent_listener( ) };

#endif