#pragma once
#include "handle_entity.h"

#define NUM_ENT_ENTRY_BITS         (11 + 2)
#define NUM_ENT_ENTRIES            (1 << NUM_ENT_ENTRY_BITS)
#define INVALID_EHANDLE_INDEX 0xffffffff
#define NUM_SERIAL_NUM_BITS        16 // (32 - num_ent_entry_bits)
#define NUM_SERIAL_NUM_SHIFT_BITS (32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK             (( 1 << NUM_SERIAL_NUM_BITS) - 1)

class c_base_handle
{
public:
	c_base_handle();
	c_base_handle(const c_base_handle& other);
	c_base_handle(unsigned long value);
	c_base_handle(int iEntry, int iSerialNumber);

	void init(int iEntry, int iSerialNumber);
	void term();

	// Even if this returns true, Get() still can return return a non-null value.
	// This just tells if the handle has been initted with any values.
	bool is_valid() const;

	int get_entry_index() const;
	int get_serial_number() const;

	int to_int() const;
	bool operator !=(const c_base_handle& other) const;
	bool operator ==(const c_base_handle& other) const;
	bool operator ==(const i_handle_entity* pEnt) const;
	bool operator !=(const i_handle_entity* pEnt) const;
	bool operator <(const c_base_handle& other) const;

	// Assign a value to the handle.
	const c_base_handle& operator=(const i_handle_entity* pEntity);
	const c_base_handle& set(const i_handle_entity* pEntity);

	static i_handle_entity* get();
protected:
	unsigned long  m_Index;
};

inline c_base_handle::c_base_handle()
{
	m_Index = INVALID_EHANDLE_INDEX;
}

inline c_base_handle::c_base_handle(const c_base_handle& other)
{
	m_Index = other.m_Index;
}

inline c_base_handle::c_base_handle(unsigned long value)
{
	m_Index = value;
}

inline c_base_handle::c_base_handle(int iEntry, int iSerialNumber)
{
	init(iEntry, iSerialNumber);
}

inline void c_base_handle::init(int iEntry, int iSerialNumber)
{
	m_Index = (unsigned long)(iEntry | (iSerialNumber << NUM_SERIAL_NUM_SHIFT_BITS));
}

inline void c_base_handle::term()
{
	m_Index = INVALID_EHANDLE_INDEX;
}

inline bool c_base_handle::is_valid() const
{
	return m_Index != INVALID_EHANDLE_INDEX;
}

inline int c_base_handle::get_entry_index() const
{
	// There is a hack here: due to a bug in the original implementation of the 
	// entity handle system, an attempt to look up an invalid entity index in 
	// certain cirumstances might fall through to the the mask operation below.
	// This would mask an invalid index to be in fact a lookup of entity number
	// NUM_ENT_ENTRIES, so invalid ent indexes end up actually looking up the
	// last slot in the entities array. Since this slot is always empty, the 
	// lookup returns NULL and the expected behavior occurs through this unexpected
	// route.
	// A lot of code actually depends on this behavior, and the bug was only exposed
	// after a change to NUM_SERIAL_NUM_BITS increased the number of allowable
	// static props in the world. So the if-stanza below detects this case and 
	// retains the prior (bug-submarining) behavior.
	if (!is_valid())
		return NUM_ENT_ENTRIES - 1;
	return m_Index & ENT_ENTRY_MASK;
}

inline int c_base_handle::get_serial_number() const
{
	return m_Index >> NUM_SERIAL_NUM_SHIFT_BITS;
}

inline int c_base_handle::to_int() const
{
	return (int)m_Index;
}

inline bool c_base_handle::operator !=(const c_base_handle& other) const
{
	return m_Index != other.m_Index;
}

inline bool c_base_handle::operator ==(const c_base_handle& other) const
{
	return m_Index == other.m_Index;
}

inline bool c_base_handle::operator ==(const i_handle_entity* pEnt) const
{
	return get() == pEnt;
}

inline bool c_base_handle::operator !=(const i_handle_entity* pEnt) const
{
	return get() != pEnt;
}

inline bool c_base_handle::operator <(const c_base_handle& other) const
{
	return m_Index < other.m_Index;
}

inline const c_base_handle& c_base_handle::operator=(const i_handle_entity* pEntity)
{
	return set(pEntity);
}

inline const c_base_handle& c_base_handle::set(const i_handle_entity* pEntity)
{
	if (pEntity)
		*this = pEntity->get_ref_ehandle();
	else
		m_Index = INVALID_EHANDLE_INDEX;

	return *this;
}


template< class T >
class c_handle : public c_base_handle
{
public:

	c_handle();
	c_handle(int iEntry, int iSerialNumber);
	c_handle(const c_base_handle& handle);
	c_handle(T* pVal);

	// The index should have come from a call to ToInt(). If it hasn't, you're in trouble.
	static c_handle<T> from_index(int index);

	T* get() const;
	void	set(const T* pval);

	operator T* ();
	operator T* () const;

	bool	operator !() const;
	bool	operator==(T* val) const;
	bool	operator!=(T* val) const;
	const c_base_handle& operator=(const T* val);

	T* operator->() const;
};


// ----------------------------------------------------------------------- //
// Inlines.
// ----------------------------------------------------------------------- //

template<class T>
c_handle<T>::c_handle()
{
}


template<class T>
c_handle<T>::c_handle(int iEntry, int iSerialNumber)
{
	init(iEntry, iSerialNumber);
}


template<class T>
c_handle<T>::c_handle(const c_base_handle& handle)
	: c_base_handle(handle)
{
}


template<class T>
c_handle<T>::c_handle(T* pObj)
{
	term();
	Set(pObj);
}


template<class T>
inline c_handle<T> c_handle<T>::from_index(int index)
{
	c_handle<T> ret;
	ret.m_Index = index;
	return ret;
}


template<class T>
inline T* c_handle<T>::get() const
{
	return (T*)c_base_handle::get();
}


template<class T>
inline c_handle<T>::operator T* ()
{
	return get();
}

template<class T>
inline c_handle<T>::operator T* () const
{
	return get();
}


template<class T>
inline bool c_handle<T>::operator !() const
{
	return !get();
}

template<class T>
inline bool c_handle<T>::operator==(T* val) const
{
	return get() == val;
}

template<class T>
inline bool c_handle<T>::operator!=(T* val) const
{
	return get() != val;
}

template<class T>
void c_handle<T>::set(const T* pVal)
{
	c_base_handle::set(reinterpret_cast<const i_handle_entity*>(pVal));
}

template<class T>
inline const c_base_handle& c_handle<T>::operator=(const T* val)
{
	set(val);
	return *this;
}

template<class T>
T* c_handle<T>::operator -> () const
{
	return get();
}