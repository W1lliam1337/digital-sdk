#ifndef MATH_UTL_H
#define MATH_UTL_H

template < class T, class I = int > class c_utl_memory {
public:
    [[nodiscard]] bool is_valid_index( I i ) const {
        long x = i;
        return x >= 0 && x < m_alloc_count;
    }

    T& operator[]( I i );
    const T& operator[]( I i ) const;

    T* base( ) { return m_memory; }

    [[nodiscard]] int num_allocated( ) const { return m_alloc_count; }

    void grow( const int num ) {
        auto calc_new_allocation_count = [ & ]( int allocation_count, const int grow_size, const int new_size,
                                                const int bytes_item ) {
            if ( grow_size )
                allocation_count = ( 1 + ( new_size - 1 ) / grow_size ) * grow_size;
            else {
                if ( !allocation_count )
                    allocation_count = ( 31 + bytes_item ) / bytes_item;

                while ( allocation_count < new_size ) allocation_count *= 2;
            }

            return allocation_count;
        };

        auto old_alloc_count = m_alloc_count;
        int alloc_requested = m_alloc_count + num;
        int new_alloc_count = calc_new_allocation_count( m_alloc_count, m_grow_size, alloc_requested, sizeof( T ) );

        if ( ( int ) ( I ) new_alloc_count < alloc_requested ) {
            if ( ( int ) ( I ) new_alloc_count == 0 && ( int ) ( I ) ( new_alloc_count - 1 ) >= alloc_requested )
                --new_alloc_count;
            else {
                if ( ( int ) ( I ) alloc_requested != alloc_requested ) {
                    return;
                }
                while ( ( int ) ( I ) new_alloc_count < alloc_requested ) {
                    new_alloc_count = ( new_alloc_count + alloc_requested ) / 2;
                }
            }
        }

        m_alloc_count = new_alloc_count;

        if ( m_memory ) {
            auto ptr = new unsigned char[ m_alloc_count * sizeof( T ) ];

            memcpy( ptr, m_memory, old_alloc_count * sizeof( T ) );
            m_memory = ( T* ) ptr;
        } else
            m_memory = ( T* ) new unsigned char[ m_alloc_count * sizeof( T ) ];
    }
protected:
    T* m_memory{ };
    int m_alloc_count{ };
    int m_grow_size{ };
};

template < class T, class I > T& c_utl_memory< T, I >::operator[]( I i ) {
    return m_memory[ i ];
}

template < class T, class I > const T& c_utl_memory< T, I >::operator[]( I i ) const {
    return m_memory[ i ];
}

template < class T > void destruct( T* memory ) {
    memory->~T( );
}

template < class T > T* construct( T* memory ) {
    return ::new ( memory ) T;
}

template < class T > T* copy_construct( T* memory, T const& src ) {
    return ::new ( memory ) T( src );
}

template < class T, class A = c_utl_memory< T > > class c_utl_vector {
    typedef A c_allocator;

    typedef T* iterator;
    typedef const T* const_iterator;
public:
    T& operator[]( int i );
    const T& operator[]( int i ) const;

    T& element( int i ) { return m_memory[ i ]; }

    [[nodiscard]] const T& element( int i ) const { return m_memory[ i ]; }

    T* base( ) { return m_memory.base( ); }

    [[nodiscard]] int count( ) const { return m_size; }

    void remove_all( ) {
        for ( int i = m_size; --i >= 0; ) destruct( &element( i ) );
        m_size = 0;
    }

    [[nodiscard]] bool is_valid_index( const int i ) const { return i >= 0 && i < m_size; }

    void shift_elements_right( const int elem, const int num = 1 ) {
        const auto num_to_move = m_size - elem - num;
        if ( num_to_move > 0 && num > 0 )
            memmove( &element( elem + num ), &element( elem ), num_to_move * sizeof( T ) );
    }

    void shift_elements_left( const int elem, const int num = 1 ) {
        const auto num_to_move = m_size - elem - num;
        if ( num_to_move > 0 && num > 0 )
            memmove( &element( elem ), &element( elem + num ), num_to_move * sizeof( T ) );
    }

    void grow_vector( const int num = 1 ) {
        if ( m_size + num > m_memory.num_allocated( ) ) {
            m_memory.grow( m_size + num - m_memory.num_allocated( ) );
        }

        m_size += num;
    }

    int insert_before( const int elem ) {
        grow_vector( );
        shift_elements_right( elem );
        construct( &element( elem ) );
        return elem;
    }

    int add_to_head( ) { return insert_before( 0 ); }
    int add_to_tail( ) { return insert_before( m_size ); }

    int insert_before( const int elem, const T& src ) {
        grow_vector( );
        shift_elements_right( elem );
        copy_construct( &element( elem ), src );
        return elem;
    }

    int add_to_tail( const T& src ) { return insert_before( m_size, src ); }

    [[nodiscard]] int find( const T& src ) const {
        for ( int i = 0; i < count( ); ++i ) {
            if ( element( i ) == src )
                return i;
        }
        return -1;
    }

    void remove( const int elem ) {
        destruct( &element( elem ) );
        shift_elements_left( elem );
        --m_size;
    }

    bool find_and_remove( const T& src ) {
        const auto elem = find( src );
        if ( elem != -1 ) {
            remove( elem );
            return true;
        }
        return false;
    }

    iterator begin( ) { return base( ); }
    [[nodiscard]] const_iterator begin( ) const { return base( ); }

    iterator end( ) { return base( ) + count( ); }
    [[nodiscard]] const_iterator end( ) const { return base( ) + count( ); }
protected:
    c_allocator m_memory{ };
    int m_size{ };
    T* m_elements{ };
};

template < typename T, class A > T& c_utl_vector< T, A >::operator[]( int i ) {
    return m_memory[ i ];
}

template < typename T, class A > const T& c_utl_vector< T, A >::operator[]( int i ) const {
    return m_memory[ i ];
}

#endif