#pragma once

#include <cstring>

#pragma once

#include <cassert>

template< class T, class I = int >
class c_utl_memory
{
public:
	// constructor, destructor
	c_utl_memory(int grow_size = 0, int init_allocation_count = 0);
	c_utl_memory(T* memory, int num_elements);
	c_utl_memory(const T* memory, int num_elements);
	~c_utl_memory();

	// Set the size by which the memory grows
	void init(int grow_size = 0, int init_size = 0);

	class iterator_t
	{
	public:
		iterator_t(I i) : index(i) {}
		I index;

		bool operator==(const iterator_t it) const { return index == it.index; }
		bool operator!=(const iterator_t it) const { return index != it.index; }
	};

	[[nodiscard]] iterator_t first() const { return iterator_t(is_idx_valid(0) ? 0 : invalid_index()); }
	[[nodiscard]] iterator_t next(const iterator_t& it) const { return iterator_t(is_idx_valid(it.index + 1) ? it.index + 1 : invalid_index()); }
	[[nodiscard]] I get_index(const iterator_t& it) const { return it.index; }
	[[nodiscard]] bool is_idx_after(I i, const iterator_t& it) const { return i > it.index; }
	[[nodiscard]] bool is_valid_iterator(const iterator_t& it) const { return is_idx_valid(it.index); }
	[[nodiscard]] iterator_t invalid_iterator() const { return iterator_t(invalid_index()); }

	// element access
	T& operator[](I i);
	const T& operator[](I i) const;
	T& element(I i);
	[[nodiscard]] const T& element(I i) const;

	[[nodiscard]] bool is_idx_valid(I i) const;

	static const I INVALID_INDEX = (I)-1; // For use with COMPILE_TIME_ASSERT
	static I invalid_index() { return INVALID_INDEX; }

	T* base();
	[[nodiscard]] const T* base() const;

	void set_external_buffer(T* memory, int num_elements);
	void set_external_buffer(const T* memory, int num_elements);
	void assume_memory(T* memory, int size);
	T* detach();
	void* detach_memory();

	void swap(c_utl_memory< T, I >& mem) noexcept;
	void convert_to_growable_memory(int grow_size);
	[[nodiscard]] int num_allocated() const;
	[[nodiscard]] int count() const;
	void grow(int num = 1);
	void ensure_capacity(int num);
	void purge();
	void purge(int num_elements);
	[[nodiscard]] bool is_externally_allocated() const;
	[[nodiscard]] bool is_read_only() const;
	void set_grow_size(int size);

protected:
	void validate_grow_size()
	{

	}

	enum
	{
		external_buffer_marker = -1,
		external_const_buffer_marker = -2,
	};

	T* m_memory;
	int m_allocation_count_;
	int m_grow_size;
};

//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------

template< class T, class I >
c_utl_memory<T, I>::c_utl_memory(const int grow_size, const int init_allocation_count) : m_memory(nullptr),
                                                                                         m_allocation_count_(init_allocation_count), m_grow_size(grow_size)
{
	validate_grow_size();
	assert(grow_size >= 0);
	if (m_allocation_count_) {
		m_memory = (T*)new unsigned char[m_allocation_count_ * sizeof(T)];
		//m_memory = (T*)malloc(m_nAllocationcount * sizeof(T));
	}
}

template< class T, class I >
c_utl_memory<T, I>::c_utl_memory(T* memory, int num_elements) : m_memory(memory),
m_allocation_count_(num_elements)
{
	// Special marker indicating externally supplied modifyable memory
	m_grow_size = external_buffer_marker;
}

template< class T, class I >
c_utl_memory<T, I>::c_utl_memory(const T* memory, int num_elements) : m_memory((T*)memory),
m_allocation_count_(num_elements)
{
	// Special marker indicating externally supplied modifyable memory
	m_grow_size = external_const_buffer_marker;
}

template< class T, class I >
c_utl_memory<T, I>::~c_utl_memory()
{
	purge();
}

template< class T, class I >
void c_utl_memory<T, I>::init(int grow_size /*= 0*/, int init_size /*= 0*/)
{
	purge();

	m_grow_size = grow_size;
	m_allocation_count_ = init_size;
	validate_grow_size();
	assert(grow_size >= 0);
	if (m_allocation_count_) {
		m_memory = (T*)malloc(m_allocation_count_ * sizeof(T));
	}
}

//-----------------------------------------------------------------------------
// Fast swap
//-----------------------------------------------------------------------------
template< class T, class I >
void c_utl_memory<T, I>::swap(c_utl_memory<T, I>& mem) noexcept
{
	V_swap(m_grow_size, mem.m_grow_size);
	V_swap(m_memory, mem.m_memory);
	V_swap(m_allocation_count_, mem.m_allocation_count_);
}


//-----------------------------------------------------------------------------
// Switches the buffer from an external memory buffer to a reallocatable buffer
//-----------------------------------------------------------------------------
template< class T, class I >
void c_utl_memory<T, I>::convert_to_growable_memory(const int grow_size)
{
	if (!is_externally_allocated())
		return;

	m_grow_size = grow_size;
	if (m_allocation_count_) {
		const int nNumBytes = m_allocation_count_ * sizeof(T);
		T* memory = (T*)malloc(nNumBytes);
		memcpy(memory, m_memory, nNumBytes);
		m_memory = memory;
	}
	else {
		m_memory = NULL;
	}
}


//-----------------------------------------------------------------------------
// Attaches the buffer to external memory....
//-----------------------------------------------------------------------------
template< class T, class I >
void c_utl_memory<T, I>::set_external_buffer(T* memory, const int num_elements)
{
	// Blow away any existing allocated memory
	purge();

	m_memory = memory;
	m_allocation_count_ = num_elements;

	// Indicate that we don't own the memory
	m_grow_size = external_buffer_marker;
}

template< class T, class I >
void c_utl_memory<T, I>::set_external_buffer(const T* memory, int num_elements)
{
	// Blow away any existing allocated memory
	purge();

	m_memory = const_cast<T*>(memory);
	m_allocation_count_ = num_elements;

	// Indicate that we don't own the memory
	m_grow_size = external_const_buffer_marker;
}

template< class T, class I >
void c_utl_memory<T, I>::assume_memory(T* memory, int num_elements)
{
	// Blow away any existing allocated memory
	purge();

	// Simply take the pointer but don't mark us as external
	m_memory = memory;
	m_allocation_count_ = num_elements;
}

template< class T, class I >
void* c_utl_memory<T, I>::detach_memory()
{
	if (is_externally_allocated())
		return nullptr;

	void* memory = m_memory;
	m_memory = 0;
	m_allocation_count_ = 0;
	return memory;
}

template< class T, class I >
T* c_utl_memory<T, I>::detach()
{
	return (T*)detach_memory();
}


//-----------------------------------------------------------------------------
// element access
//-----------------------------------------------------------------------------
template< class T, class I >
T& c_utl_memory<T, I>::operator[](I i)
{
	assert(!is_read_only());
	assert(is_idx_valid(i));
	return m_memory[i];
}

template< class T, class I >
const T& c_utl_memory<T, I>::operator[](I i) const
{
	assert(is_idx_valid(i));
	return m_memory[i];
}

template< class T, class I >
T& c_utl_memory<T, I>::element(I i)
{
	assert(!is_read_only());
	assert(is_idx_valid(i));
	return m_memory[i];
}

template< class T, class I >
const T& c_utl_memory<T, I>::element(I i) const
{
	assert(is_idx_valid(i));
	return m_memory[i];
}


//-----------------------------------------------------------------------------
// is the memory externally allocated?
//-----------------------------------------------------------------------------
template< class T, class I >
bool c_utl_memory<T, I>::is_externally_allocated() const
{
	return (m_grow_size < 0);
}


//-----------------------------------------------------------------------------
// is the memory read only?
//-----------------------------------------------------------------------------
template< class T, class I >
bool c_utl_memory<T, I>::is_read_only() const
{
	return (m_grow_size == external_const_buffer_marker);
}


template< class T, class I >
void c_utl_memory<T, I>::set_grow_size(const int size)
{
	assert(!is_externally_allocated());
	assert(size >= 0);
	m_grow_size = size;
	validate_grow_size();
}


//-----------------------------------------------------------------------------
// Gets the base address (can change when adding elements!)
//-----------------------------------------------------------------------------
template< class T, class I >
T* c_utl_memory<T, I>::base()
{
	assert(!is_read_only());
	return m_memory;
}

template< class T, class I >
const T* c_utl_memory<T, I>::base() const
{
	return m_memory;
}


//-----------------------------------------------------------------------------
// Size
//-----------------------------------------------------------------------------
template< class T, class I >
int c_utl_memory<T, I>::num_allocated() const
{
	return m_allocation_count_;
}

template< class T, class I >
int c_utl_memory<T, I>::count() const
{
	return m_allocation_count_;
}


//-----------------------------------------------------------------------------
// Is element index valid?
//-----------------------------------------------------------------------------
template< class T, class I >
bool c_utl_memory<T, I>::is_idx_valid(I i) const
{
	// GCC warns if I is an unsigned type and we do a ">= 0" against it (since the comparison is always 0).
	// We Get the warning even if we cast inside the expression. It only goes away if we assign to another variable.
	const long x = i;
	return (x >= 0) && (x < m_allocation_count_);
}

//-----------------------------------------------------------------------------
// Grows the memory
//-----------------------------------------------------------------------------
inline int utl_memory_calc_new_allocation_count(int allocation_count, const int grow_size, const int new_size, const int bytes_item)
{
	if (grow_size) {
		allocation_count = ((1 + ((new_size - 1) / grow_size)) * grow_size);
	}
	else {
		if (!allocation_count) {
			// Compute an allocation which is at least as big as a cache line...
			allocation_count = (31 + bytes_item) / bytes_item;
		}

		while (allocation_count < new_size) {
#ifndef _X360
			allocation_count *= 2;
#else
			int nNewAllocationcount = (nAllocationcount * 9) / 8; // 12.5 %
			if (nNewAllocationcount > nAllocationcount)
				nAllocationcount = nNewAllocationcount;
			else
				nAllocationcount *= 2;
#endif
		}
	}

	return allocation_count;
}

template< class T, class I >
void c_utl_memory<T, I>::grow(int num)
{
	assert(num > 0);

	if (is_externally_allocated()) {
		// Can't grow a buffer whose memory was externally allocated 
		assert(0);
		return;
	}


	auto old_allocation_count = m_allocation_count_;
	// Make sure we have at least numallocated + num allocations.
	// Use the grow rules specified for this memory (in m_grow_size)
	int allocation_requested = m_allocation_count_ + num;

	int new_allocation_count = utl_memory_calc_new_allocation_count(m_allocation_count_, m_grow_size, allocation_requested, sizeof(T));

	// if m_nAllocationRequested wraps index type I, recalculate
	if ((int)(I)new_allocation_count < allocation_requested) {
		if ((int)(I)new_allocation_count == 0 && (int)(I)(new_allocation_count - 1) >= allocation_requested) {
			--new_allocation_count; // deal w/ the common case of m_nAllocationcount == MAX_USHORT + 1
		}
		else {
			if ((int)(I)allocation_requested != allocation_requested) {
				// we've been asked to grow memory to a size s.t. the index type can't address the requested amount of memory
				assert(0);
				return;
			}
			while ((int)(I)new_allocation_count < allocation_requested) {
				new_allocation_count = (new_allocation_count + allocation_requested) / 2;
			}
		}
	}

	m_allocation_count_ = new_allocation_count;

	if (m_memory) {
		auto ptr = new unsigned char[m_allocation_count_ * sizeof(T)];

		memcpy(ptr, m_memory, old_allocation_count * sizeof(T));
		m_memory = (T*)ptr;
	}
	else {
		m_memory = (T*)new unsigned char[m_allocation_count_ * sizeof(T)];
	}
}


//-----------------------------------------------------------------------------
// Makes sure we've got at least this much memory
//-----------------------------------------------------------------------------
template< class T, class I >
void c_utl_memory<T, I>::ensure_capacity(int num)
{
	if (m_allocation_count_ >= num)
		return;

	if (is_externally_allocated()) {
		// Can't grow a buffer whose memory was externally allocated 
		assert(0);
		return;
	}
	m_allocation_count_ = num;

	if (m_memory) {
		m_memory = (T*)realloc(m_memory, m_allocation_count_ * sizeof(T));
	}
	else {
		m_memory = (T*)malloc(m_allocation_count_ * sizeof(T));
	}
}


//-----------------------------------------------------------------------------
// Memory deallocation
//-----------------------------------------------------------------------------
template< class T, class I >
void c_utl_memory<T, I>::purge()
{
	if (!is_externally_allocated()) {
		if (m_memory) {
			free((void*)m_memory);
			m_memory = 0;
		}
		m_allocation_count_ = 0;
	}
}

template< class T, class I >
void c_utl_memory<T, I>::purge(int num_elements)
{
	assert(num_elements >= 0);

	if (num_elements > m_allocation_count_) {
		// Ensure this isn't a grow request in disguise.
		assert(num_elements <= m_allocation_count_);
		return;
	}

	// If we have zero elements, simply do a purge:
	if (num_elements == 0) {
		purge();
		return;
	}

	if (is_externally_allocated()) {
		// Can't shrink a buffer whose memory was externally allocated, fail silently like purge 
		return;
	}

	// If the number of elements is the same as the allocation count, we are done.
	if (num_elements == m_allocation_count_) {
		return;
	}


	if (!m_memory) {
		// Allocation count is non zero, but memory is null.
		assert(m_memory);
		return;
	}
	m_allocation_count_ = num_elements;
	m_memory = (T*)realloc(m_memory, m_allocation_count_ * sizeof(T));
}

//-----------------------------------------------------------------------------
// The CUtlMemory class:
// A growable memory class which doubles in size by default.
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
class c_utl_memory_aligned : public c_utl_memory<T>
{
public:
	// constructor, destructor
	c_utl_memory_aligned(int grow_size = 0, int nInitSize = 0);
	c_utl_memory_aligned(T* memory, int num_elements);
	c_utl_memory_aligned(const T* memory, int num_elements);
	~c_utl_memory_aligned();

	// Attaches the buffer to external memory....
	void set_external_buffer(T* memory, int num_elements);
	void set_external_buffer(const T* memory, int num_elements);

	// Grows the memory, so that at least allocated + num elements are allocated
	void grow(int num = 1);

	// Makes sure we've got at least this much memory
	void ensure_capacity(int num);

	// Memory deallocation
	void purge();

	// Purge all but the given number of elements (NOT IMPLEMENTED IN CUtlMemoryAligned)
	static void purge(int num_elements) { assert(0); }

private:
	static void* align(const void* pAddr);
};


//-----------------------------------------------------------------------------
// Aligns a pointer
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
void* c_utl_memory_aligned<T, nAlignment>::align(const void* pAddr)
{
	const size_t alignment_mask = nAlignment - 1;
	return reinterpret_cast<void*>((reinterpret_cast<size_t>(pAddr) + alignment_mask) & (~alignment_mask));
}


//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
c_utl_memory_aligned<T, nAlignment>::c_utl_memory_aligned(int grow_size, int nInitAllocationcount)
{
	c_utl_memory<T>::m_memory = 0;
	c_utl_memory<T>::m_nAllocationcount = nInitAllocationcount;
	c_utl_memory<T>::m_grow_size = grow_size;
	this->ValidateGrowSize();

	// Alignment must be a power of two
	COMPILE_TIME_ASSERT((nAlignment & (nAlignment - 1)) == 0);
	assert((grow_size >= 0) && (grow_size != CUtlMemory<T>::EXTERNAL_BUFFER_MARKER));
	if (c_utl_memory<T>::m_nAllocationcount) {
		c_utl_memory<T>::m_memory = (T*)_aligned_malloc(nInitAllocationcount * sizeof(T), nAlignment);
	}
}

template< class T, int nAlignment >
c_utl_memory_aligned<T, nAlignment>::c_utl_memory_aligned(T* memory, int num_elements)
{
	// Special marker indicating externally supplied memory
	c_utl_memory<T>::m_grow_size = c_utl_memory<T>::EXTERNAL_BUFFER_MARKER;

	c_utl_memory<T>::m_memory = (T*)align(memory);
	c_utl_memory<T>::m_nAllocationcount = ((int)(memory + num_elements) - (int)c_utl_memory<T>::m_memory) / sizeof(T);
}

template< class T, int nAlignment >
c_utl_memory_aligned<T, nAlignment>::c_utl_memory_aligned(const T* memory, int num_elements)
{
	// Special marker indicating externally supplied memory
	c_utl_memory<T>::m_grow_size = c_utl_memory<T>::EXTERNAL_CONST_BUFFER_MARKER;

	c_utl_memory<T>::m_memory = (T*)align(memory);
	c_utl_memory<T>::m_nAllocationcount = ((int)(memory + num_elements) - (int)c_utl_memory<T>::m_memory) / sizeof(T);
}

template< class T, int nAlignment >
c_utl_memory_aligned<T, nAlignment>::~c_utl_memory_aligned()
{
	purge();
}


//-----------------------------------------------------------------------------
// Attaches the buffer to external memory....
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
void c_utl_memory_aligned<T, nAlignment>::set_external_buffer(T* memory, int num_elements)
{
	// Blow away any existing allocated memory
	purge();

	c_utl_memory<T>::m_memory = (T*)align(memory);
	c_utl_memory<T>::m_nAllocationcount = ((int)(memory + num_elements) - (int)c_utl_memory<T>::m_memory) / sizeof(T);

	// Indicate that we don't own the memory
	c_utl_memory<T>::m_grow_size = c_utl_memory<T>::EXTERNAL_BUFFER_MARKER;
}

template< class T, int nAlignment >
void c_utl_memory_aligned<T, nAlignment>::set_external_buffer(const T* memory, int num_elements)
{
	// Blow away any existing allocated memory
	purge();

	c_utl_memory<T>::m_memory = (T*)align(memory);
	c_utl_memory<T>::m_nAllocationcount = ((int)(memory + num_elements) - (int)c_utl_memory<T>::m_memory) / sizeof(T);

	// Indicate that we don't own the memory
	c_utl_memory<T>::m_grow_size = c_utl_memory<T>::EXTERNAL_CONST_BUFFER_MARKER;
}


//-----------------------------------------------------------------------------
// Grows the memory
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
void c_utl_memory_aligned<T, nAlignment>::grow(int num)
{
	assert(num > 0);

	if (this->IsExternallyAllocated()) {
		// Can't grow a buffer whose memory was externally allocated 
		assert(0);
		return;
	}

	// Make sure we have at least numallocated + num allocations.
	// Use the grow rules specified for this memory (in m_grow_size)
	const int allocation_requested = c_utl_memory<T>::m_nAllocationcount + num;

	c_utl_memory<T>::m_nAllocationcount = utl_memory_calc_new_allocation_count(c_utl_memory<T>::m_nAllocationcount, c_utl_memory<T>::m_grow_size, allocation_requested, sizeof(T));

	if (c_utl_memory<T>::m_memory) {
		c_utl_memory<T>::m_memory = (T*)MemAlloc_ReallocAligned(c_utl_memory<T>::m_memory, c_utl_memory<T>::m_nAllocationcount * sizeof(T), nAlignment);
		assert(CUtlMemory<T>::m_memory);
	}
	else {
		c_utl_memory<T>::m_memory = (T*)MemAlloc_AllocAligned(c_utl_memory<T>::m_nAllocationcount * sizeof(T), nAlignment);
		assert(CUtlMemory<T>::m_memory);
	}
}


//-----------------------------------------------------------------------------
// Makes sure we've got at least this much memory
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
void c_utl_memory_aligned<T, nAlignment>::ensure_capacity(int num)
{
	if (c_utl_memory<T>::m_nAllocationcount >= num)
		return;

	if (this->IsExternallyAllocated()) {
		// Can't grow a buffer whose memory was externally allocated 
		assert(0);
		return;
	}

	c_utl_memory<T>::m_nAllocationcount = num;

	if (c_utl_memory<T>::m_memory) {
		c_utl_memory<T>::m_memory = (T*)MemAlloc_ReallocAligned(c_utl_memory<T>::m_memory, c_utl_memory<T>::m_nAllocationcount * sizeof(T), nAlignment);
	}
	else {
		c_utl_memory<T>::m_memory = (T*)MemAlloc_AllocAligned(c_utl_memory<T>::m_nAllocationcount * sizeof(T), nAlignment);
	}
}


//-----------------------------------------------------------------------------
// Memory deallocation
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
void c_utl_memory_aligned<T, nAlignment>::purge()
{
	if (!this->IsExternallyAllocated()) {
		if (c_utl_memory<T>::m_memory) {
			MemAlloc_FreeAligned(c_utl_memory<T>::m_memory);
			c_utl_memory<T>::m_memory = 0;
		}
		c_utl_memory<T>::m_nAllocationcount = 0;
	}
}
template< class T, class A = c_utl_memory<T> >
class c_utl_vector
{
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef A c_allocator;
public:
	typedef T elem_type_t;

	// constructor, destructor
	c_utl_vector(int grow_size = 0, int init_size = 0);
	c_utl_vector(T* memory, int allocation_count, int num_elements = 0);
	~c_utl_vector();

	// Copy the array.
	c_utl_vector<T, A>& operator=(const c_utl_vector<T, A>& other);

	// element access
	T& operator[](int i);
	const T& operator[](int i) const;
	T& element(int i);
	[[nodiscard]] const T& element(int i) const;
	T& head();
	[[nodiscard]] const T& head() const;
	T& tail();
	[[nodiscard]] const T& tail() const;

	// Gets the base address (can change when adding elements!)
	T* base() { return m_memory.base(); }
	[[nodiscard]] const T* base() const { return m_memory.base(); }
	// Returns the number of elements in the vector
	[[nodiscard]] int count() const;
	// Is element index valid?
	[[nodiscard]] bool is_valid_index(int i) const;
	static int invalid_index();
	// Adds an element, uses default constructor
	int add_to_head();
	int add_to_tail();
	int insert_before(int elem);
	int insert_after(int elem);
	// Adds an element, uses copy constructor
	int add_to_head(const T& src);
	int add_to_tail(const T& src);
	int insert_before(int elem, const T& src);
	int insert_after(int elem, const T& src);
	// Adds multiple elements, uses default constructor
	int add_multiple_to_head(int num);
	int add_multiple_to_tail(int num);
	int add_multiple_to_tail(int num, const T* pToCopy);
	int insert_multiple_before(int elem, int num);
	int insert_multiple_before(int elem, int num, const T* pToCopy);
	int insert_multiple_after(int elem, int num);
	// Calls RemoveAll() then AddMultipleToTail.
	void set_size(int size);
	void set_count(int count);
	void set_count_non_destructively(int count); //sets count by adding or removing elements to tail TODO: This should probably be the default behavior for Setcount
	void copy_array(const T* pArray, int size); //Calls SetSize and copies each element.
											   // Fast swap
	void swap(c_utl_vector< T, A >& vec) noexcept;
	// Add the specified array to the tail.
	int add_vector_to_tail(c_utl_vector<T, A> const& src);
	// Finds an element (element needs operator== defined)
	[[nodiscard]] int get_offset(const T& src) const;
	void fill_with_value(const T& src);
	[[nodiscard]] bool has_element(const T& src) const;
	// Makes sure we have enough memory allocated to store a requested # of elements
	void ensure_capacity(int num);
	// Makes sure we have at least this many elements
	void ensure_count(int num);
	// element removal
	void fast_remove(int elem);    // doesn't preserve order
	void remove(int elem);        // preserves order, shifts elements
	bool find_and_remove(const T& src);    // removes first occurrence of src, preserves order, shifts elements
	bool find_and_fast_remove(const T& src);    // removes first occurrence of src, doesn't preserve order
	void remove_multiple(int elem, int num);    // preserves order, shifts elements
	void remove_multiple_from_head(int num); // removes num elements from tail
	void remove_multiple_from_tail(int num); // removes num elements from tail
	void remove_all();                // doesn't deallocate memory
	void purge(); // Memory deallocation
				  // Purges the list and calls delete on each element in it.
	void purge_and_delete_elements();
	// Compacts the vector to the number of elements actually in use 
	void compact();
	// Set the size by which it grows when it needs to allocate more memory.
	void set_grow_size(int size) { m_memory.SetGrowSize(size); }
	[[nodiscard]] int num_allocated() const;    // Only use this if you really know what you're doing!
	void sort(int(__cdecl* pfn_compare)(const T*, const T*));

	iterator begin() { return base(); }
	[[nodiscard]] const_iterator begin() const { return base(); }
	iterator end() { return base() + count(); }
	[[nodiscard]] const_iterator end() const { return base() + count(); }

protected:
	// Can't copy this unless we explicitly do it!
	c_utl_vector(c_utl_vector const& vec) { assert(0); }

	// Grows the vector
	void grow_vector(int num = 1);

	// Shifts elements....
	void shift_elements_right(int elem, int num = 1);
	void shift_elements_left(int elem, int num = 1);

public:
	c_allocator m_memory;
	int m_size;

	// For easier access to the elements through the debugger
	// it's in release builds so this can be used in libraries correctly
	T* m_elements;

	void reset_dbg_info()
	{
		m_elements = base();
	}
};


//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------
template< typename T, class A >
c_utl_vector<T, A>::c_utl_vector(int grow_size, int init_size) :
	m_memory(grow_size, init_size), m_size(0)
{
	reset_dbg_info();
}

template< typename T, class A >
c_utl_vector<T, A>::c_utl_vector(T* memory, int allocation_count, const int num_elements) :
	m_memory(memory, allocation_count), m_size(num_elements)
{
	reset_dbg_info();
}

template< typename T, class A >
c_utl_vector<T, A>::~c_utl_vector()
{
	purge();
}

template< typename T, class A >
c_utl_vector<T, A>& c_utl_vector<T, A>::operator=(const c_utl_vector<T, A>& other)
{
	const int ncount = other.count();
	set_size(ncount);
	for (int i = 0; i < ncount; i++) {
		(*this)[i] = other[i];
	}
	return *this;
}


//-----------------------------------------------------------------------------
// element access
//-----------------------------------------------------------------------------
template< typename T, class A >
T& c_utl_vector<T, A>::operator[](int i)
{
	assert(i < m_size);
	return m_memory[i];
}

template< typename T, class A >
const T& c_utl_vector<T, A>::operator[](int i) const
{
	assert(i < m_size);
	return m_memory[i];
}

template< typename T, class A >
T& c_utl_vector<T, A>::element(int i)
{
	assert(i < m_size);
	return m_memory[i];
}

template< typename T, class A >
const T& c_utl_vector<T, A>::element(int i) const
{
	assert(i < m_size);
	return m_memory[i];
}

template< typename T, class A >
T& c_utl_vector<T, A>::head()
{
	assert(m_size > 0);
	return m_memory[0];
}

template< typename T, class A >
const T& c_utl_vector<T, A>::head() const
{
	assert(m_size > 0);
	return m_memory[0];
}

template< typename T, class A >
T& c_utl_vector<T, A>::tail()
{
	assert(m_size > 0);
	return m_memory[m_size - 1];
}

template< typename T, class A >
const T& c_utl_vector<T, A>::tail() const
{
	assert(m_size > 0);
	return m_memory[m_size - 1];
}


//-----------------------------------------------------------------------------
// count
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::count() const
{
	return m_size;
}


//-----------------------------------------------------------------------------
// Is element index valid?
//-----------------------------------------------------------------------------
template< typename T, class A >
bool c_utl_vector<T, A>::is_valid_index(int i) const
{
	return (i >= 0) && (i < m_size);
}


//-----------------------------------------------------------------------------
// Returns in invalid index
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::invalid_index()
{
	return -1;
}


//-----------------------------------------------------------------------------
// Grows the vector
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::grow_vector(int num)
{
	if (m_size + num > m_memory.NumAllocated()) {
		m_memory.Grow(m_size + num - m_memory.NumAllocated());
	}

	m_size += num;
	reset_dbg_info();
}


//-----------------------------------------------------------------------------
// Sorts the vector
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::sort(int(__cdecl* pfn_compare)(const T*, const T*))
{
	typedef int(__cdecl* q_sort_compare_func_t)(const void*, const void*);
	if (count() <= 1)
		return;

	if (base()) {
		qsort(base(), count(), sizeof(T), (q_sort_compare_func_t)(pfn_compare));
	}
	else {
		assert(0);
		// this path is untested
		// if you want to sort vectors that use a non-sequential memory allocator,
		// you'll probably want to patch in a quicksort algorithm here
		// I just threw in this bubble sort to have something just in case...

		for (int i = m_size - 1; i >= 0; --i) {
			for (int j = 1; j <= i; ++j) {
				if (pfn_compare(&element(j - 1), &element(j)) < 0) {
					V_swap(element(j - 1), element(j));
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Makes sure we have enough memory allocated to store a requested # of elements
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::ensure_capacity(int num)
{
	m_memory.EnsureCapacity(num);
	reset_dbg_info();
}


//-----------------------------------------------------------------------------
// Makes sure we have at least this many elements
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::ensure_count(const int num)
{
	if (count() < num) {
		add_multiple_to_tail(num - count());
	}
}


//-----------------------------------------------------------------------------
// Shifts elements
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::shift_elements_right(const int elem, const int num)
{
	assert(is_valid_index(elem) || (m_size == 0) || (num == 0));
	const int num_to_move = m_size - elem - num;
	if ((num_to_move > 0) && (num > 0))
		memmove(&element(elem + num), &element(elem), num_to_move * sizeof(T));
}

template< typename T, class A >
void c_utl_vector<T, A>::shift_elements_left(const int elem, int num)
{
	assert(is_valid_index(elem) || (m_size == 0) || (num == 0));
	const int num_to_move = m_size - elem - num;
	if ((num_to_move > 0) && (num > 0)) {
		memmove(&element(elem), &element(elem + num), num_to_move * sizeof(T));

#ifdef _DEBUG
		memset(&element(m_size - num), 0xDD, num * sizeof(T));
#endif
	}
}


//-----------------------------------------------------------------------------
// Adds an element, uses default constructor
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::add_to_head()
{
	return insert_before(0);
}

template< typename T, class A >
int c_utl_vector<T, A>::add_to_tail()
{
	return insert_before(m_size);
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_after(const int elem)
{
	return insert_before(elem + 1);
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_before(const int elem)
{
	// Can insert at the end
	assert((elem == count()) || is_valid_index(elem));

	grow_vector();
	shift_elements_right(elem);
	Construct(&element(elem));
	return elem;
}


//-----------------------------------------------------------------------------
// Adds an element, uses copy constructor
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::add_to_head(const T& src)
{
	// Can't insert something that's in the list... reallocation may hose us
	assert((base() == NULL) || (&src < base()) || (&src >= (base() + count())));
	return insert_before(0, src);
}

template< typename T, class A >
int c_utl_vector<T, A>::add_to_tail(const T& src)
{
	// Can't insert something that's in the list... reallocation may hose us
	assert((base() == NULL) || (&src < base()) || (&src >= (base() + count())));
	return insert_before(m_size, src);
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_after(const int elem, const T& src)
{
	// Can't insert something that's in the list... reallocation may hose us
	assert((base() == NULL) || (&src < base()) || (&src >= (base() + count())));
	return insert_before(elem + 1, src);
}

//-----------------------------------------------------------------------------
// Adds multiple elements, uses default constructor
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::add_multiple_to_head(const int num)
{
	return insert_multiple_before(0, num);
}

template< typename T, class A >
int c_utl_vector<T, A>::add_multiple_to_tail(const int num)
{
	return insert_multiple_before(m_size, num);
}

template< typename T, class A >
int c_utl_vector<T, A>::add_multiple_to_tail(const int num, const T* pToCopy)
{
	// Can't insert something that's in the list... reallocation may hose us
	assert((base() == NULL) || !pToCopy || (pToCopy + num <= base()) || (pToCopy >= (base() + count())));

	return insert_multiple_before(m_size, num, pToCopy);
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_multiple_after(const int elem, const int num)
{
	return insert_multiple_before(elem + 1, num);
}


template< typename T, class A >
void c_utl_vector<T, A>::set_count(int count)
{
	remove_all();
	add_multiple_to_tail(count);
}

template< typename T, class A >
void c_utl_vector<T, A>::set_size(int size)
{
	set_count(size);
}

template< typename T, class A >
void c_utl_vector<T, A>::set_count_non_destructively(int count)
{
	const int delta = count - m_size;
	if (delta > 0) add_multiple_to_tail(delta);
	else if (delta < 0) remove_multiple_from_tail(-delta);
}

template< typename T, class A >
void c_utl_vector<T, A>::copy_array(const T* pArray, int size)
{
	// Can't insert something that's in the list... reallocation may hose us
	assert((base() == NULL) || !pArray || (base() >= (pArray + size)) || (pArray >= (base() + count())));

	set_size(size);
	for (int i = 0; i < size; i++) {
		(*this)[i] = pArray[i];
	}
}

template< typename T, class A >
void c_utl_vector<T, A>::swap(c_utl_vector< T, A >& vec) noexcept
{
	m_memory.Swap(vec.m_memory);
	V_swap(m_size, vec.m_size);
#ifndef _X360
	V_swap(m_elements, vec.m_elements);
#endif
}

template< typename T, class A >
int c_utl_vector<T, A>::add_vector_to_tail(c_utl_vector const& src)
{
	assert(&src != this);

	const int base = count();

	// Make space.
	const int src_count = src.count();
	ensure_capacity(base + src_count);

	// Copy the elements.	
	m_size += src_count;
	for (int i = 0; i < src_count; i++) {
		CopyConstruct(&element(base + i), src[i]);
	}
	return base;
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_multiple_before(const int elem, int num)
{
	if (num == 0)
		return elem;

	// Can insert at the end
	assert((elem == count()) || is_valid_index(elem));

	grow_vector(num);
	shift_elements_right(elem, num);

	// Invoke default constructors
	for (int i = 0; i < num; ++i) {
		Construct(&element(elem + i));
	}

	return elem;
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_multiple_before(int elem, int num, const T* pToInsert)
{
	if (num == 0)
		return elem;

	// Can insert at the end
	assert((elem == count()) || is_valid_index(elem));

	grow_vector(num);
	shift_elements_right(elem, num);

	// Invoke default constructors
	if (!pToInsert) {
		for (int i = 0; i < num; ++i) {
			Construct(&element(elem + i));
		}
	}
	else {
		for (int i = 0; i < num; i++) {
			CopyConstruct(&element(elem + i), pToInsert[i]);
		}
	}

	return elem;
}


//-----------------------------------------------------------------------------
// Finds an element (element needs operator== defined)
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::get_offset(const T& src) const
{
	for (int i = 0; i < count(); ++i) {
		if (element(i) == src)
			return i;
	}
	return -1;
}

template< typename T, class A >
void c_utl_vector<T, A>::fill_with_value(const T& src)
{
	for (int i = 0; i < count(); i++) {
		element(i) = src;
	}
}

template< typename T, class A >
bool c_utl_vector<T, A>::has_element(const T& src) const
{
	return (get_offset(src) >= 0);
}


//-----------------------------------------------------------------------------
// element removal
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::fast_remove(int elem)
{
	assert(is_valid_index(elem));

	Destruct(&element(elem));
	if (m_size > 0) {
		if (elem != m_size - 1)
			memcpy(&element(elem), &element(m_size - 1), sizeof(T));
		--m_size;
	}
}

template< typename T, class A >
void c_utl_vector<T, A>::remove(int elem)
{
	Destruct(&element(elem));
	shift_elements_left(elem);
	--m_size;
}

template< typename T, class A >
bool c_utl_vector<T, A>::find_and_remove(const T& src)
{
	const int elem = get_offset(src);
	if (elem != -1) {
		remove(elem);
		return true;
	}
	return false;
}

template< typename T, class A >
bool c_utl_vector<T, A>::find_and_fast_remove(const T& src)
{
	const int elem = get_offset(src);
	if (elem != -1) {
		fast_remove(elem);
		return true;
	}
	return false;
}

template< typename T, class A >
void c_utl_vector<T, A>::remove_multiple(const int elem, const int num)
{
	assert(elem >= 0);
	assert(elem + num <= count());

	for (int i = elem + num; --i >= elem; )
		Destruct(&element(i));

	shift_elements_left(elem, num);
	m_size -= num;
}

template< typename T, class A >
void c_utl_vector<T, A>::remove_multiple_from_head(const int num)
{
	assert(num <= count());

	for (int i = num; --i >= 0; )
		Destruct(&element(i));

	shift_elements_left(0, num);
	m_size -= num;
}

template< typename T, class A >
void c_utl_vector<T, A>::remove_multiple_from_tail(const int num)
{
	assert(num <= count());

	for (int i = m_size - num; i < m_size; i++)
		Destruct(&element(i));

	m_size -= num;
}

//===== Copyright � 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose:
//
// $NoKeywords: $
//
//===========================================================================//
#pragma once

#include <malloc.h>
#include <climits>
#include <cfloat>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <new.h>

#define COMPILER_MSVC

//-----------------------------------------------------------------------------
// NOTE: All compiler defines are Set up in the base VPC scripts
// COMPILER_MSVC, COMPILER_MSVC32, COMPILER_MSVC64, COMPILER_MSVCX360
// COMPILER_GCC
// The rationale for this is that we need COMPILER_MSVC for the pragma blocks
// #pragma once that occur at the top of all header files, therefore we can't
// place the defines for these in here.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Set up platform defines.
//-----------------------------------------------------------------------------
#ifdef _WIN32
#define IsPlatformLinux()	false
#define IsPlatformPosix()	false
#define IsPlatformOSX()		false
#define IsPlatformPS3()		false
#define IsPlatformWindows() true
#ifndef PLATFORM_WINDOWS
#define PLATFORM_WINDOWS	1
#endif

#ifndef _X360
#define IsPlatformX360() false
#define IsPlatformWindowsPC() true
#define PLATFORM_WINDOWS_PC 1

#ifdef _WIN64
#define IsPlatformWindowsPC64() true
#define IsPlatformWindowsPC32() false
#define PLATFORM_WINDOWS_PC64 1
#else
#define IsPlatformWindowsPC64() false
#define IsPlatformWindowsPC32() true
#define PLATFORM_WINDOWS_PC32 1
#endif

#else // _X360

#define IsPlatformWindowsPC()	false
#define IsPlatformWindowsPC64() false
#define IsPlatformWindowsPC32() false
#define IsPlatformX360()		true
#define PLATFORM_X360 1

#endif // _X360

#elif defined(POSIX)
#define IsPlatformX360()		false
#define IsPlatformPS3()			false
#define IsPlatformWindows()		false
#define IsPlatformWindowsPC()	false
#define IsPlatformWindowsPC64()	false
#define IsPlatformWindowsPC32()	false
#define IsPlatformPosix()		true
#ifndef PLATFORM_POSIX
#define PLATFORM_POSIX 1
#endif

#if defined( LINUX )
#define IsPlatformLinux() true
#define IsPlatformOSX() false
#ifndef PLATFORM_LINUX
#define PLATFORM_LINUX 1
#endif
#elif defined ( OSX )
#define IsPlatformLinux() false
#define IsPlatformOSX() true
#ifndef PLATFORM_OSX
#define PLATFORM_OSX 1
#endif
#else
#define IsPlatformLinux() false
#define IsPlatformOSX() false
#endif

#else
#error
#endif

//-----------------------------------------------------------------------------
// Set up platform type defines.
//-----------------------------------------------------------------------------
#ifdef PLATFORM_X360
#ifndef _CONSOLE
#define _CONSOLE
#endif
#define IsPC()		false
#define IsConsole() true
#else
#define IsPC()		true
#define IsConsole() false
#endif



//-----------------------------------------------------------------------------
// Set up build configuration defines.
//-----------------------------------------------------------------------------
#ifdef _CERT
#define IsCert() true
#else
#define IsCert() false
#endif

#ifdef _DEBUG
#define IsRelease() false
#define IsDebug() true
#else
#define IsRelease() true
#define IsDebug() false
#endif

#ifdef _RETAIL
#define IsRetail() true
#else
#define IsRetail() false
#endif

// Maximum and minimum representable values
#if !defined(PLATFORM_OSX) && !defined(__STDC_LIMIT_MACROS)

#ifndef INT8_MAX
#define  INT8_MAX			SCHAR_MAX
#endif
#ifndef INT16_MAX
#define  INT16_MAX			SHRT_MAX
#endif
#ifndef INT32_MAX
#define  INT32_MAX			LONG_MAX
#endif
#ifndef INT64_MAX
#define  INT64_MAX			(((int64_t)~0) >> 1)
#endif

#ifndef INT8_MIN
#define  INT8_MIN			SCHAR_MIN
#endif
#ifndef INT16_MIN
#define  INT16_MIN			SHRT_MIN
#endif
#ifndef INT32_MIN
#define  INT32_MIN			LONG_MIN
#endif
#ifndef INT64_MIN
#define  INT64_MIN			(((int64_t)1) << 63)
#endif

#ifndef UINT8_MAX
#define  UINT8_MAX			((uint8_t)~0)
#endif
#ifndef UINT16_MAX
#define  UINT16_MAX			((uint16)~0)
#endif
#ifndef UINT32_MAX
#define  UINT32_MAX			((uint32_t)~0)
#endif
#ifndef UINT16_MAX
#define  UINT64_MAX			((uint64_t)~0)
#endif

#ifndef UINT8_MIN
#define  UINT8_MIN			0
#endif
#ifndef UINT16_MIN
#define  UINT16_MIN			0
#endif
#ifndef UINT32_MIN
#define  UINT32_MIN			0
#endif
#ifndef UINT64_MIN
#define  UINT64_MIN			0
#endif

#endif // !PLATFORM_OSX && !__STDC_LIMIT_MACROS

#ifndef  UINT_MIN
#define  UINT_MIN			UINT32_MIN
#endif

#define  FLOAT32_MAX		FLT_MAX
#define  FLOAT64_MAX		DBL_MAX

#ifdef GNUC
#undef offsetof
//#define offsetof( type, var ) __builtin_offsetof( type, var ) 
#define offsetof(s,m)	(size_t)&(((s *)0)->m)
#else
#include <cstddef>
#undef offsetof
#define offsetof(s,m)	(size_t)&(((s *)0)->m)
#endif


#define  FLOAT32_MIN		FLT_MIN
#define  FLOAT64_MIN		DBL_MIN

//-----------------------------------------------------------------------------
// Long is evil because it's treated differently by different compilers
// Preventing its use is nasty however. This #define, which should be
// turned on in individual VPC files, causes you to include tier0/valve_off.h
// before standard C + windows headers, and include tier0/valve_on.h after
// standard C + windows headers. So, there's some painful overhead to disabling long
//-----------------------------------------------------------------------------
#ifdef DISALLOW_USE_OF_LONG
#define long			long_is_the_devil_stop_using_it_use_int32_or_int64
#endif


//-----------------------------------------------------------------------------
// Various compiler-specific keywords
//-----------------------------------------------------------------------------
#ifdef COMPILER_MSVC

#ifdef FORCEINLINE
#undef FORCEINLINE
#endif
#define STDCALL					__stdcall
#ifndef FASTCALL
#define  FASTCALL			__fastcall
#endif
#define FORCEINLINE				__forceinline
#define FORCEINLINE_TEMPLATE	__forceinline
#define NULLTERMINATED			__nullterminated

// This can be used to ensure the size of pointers to members when declaring
// a pointer type for a class that has only been forward declared
#define SINGLE_INHERITANCE		__single_inheritance
#define MULTIPLE_INHERITANCE	__multiple_inheritance
#define EXPLICIT				explicit
#define NO_VTABLE				__declspec( novtable )

// gcc doesn't allow storage specifiers on explicit template instatiation, but visual studio needs them to avoid link errors.
#define TEMPLATE_STATIC			static

// Used for dll exporting and importing
#define DLL_EXPORT				extern "C" __declspec( dllexport )
#define DLL_IMPORT				extern "C" __declspec( dllimport )

// Can't use extern "C" when DLL exporting a class
#define DLL_CLASS_EXPORT		__declspec( dllexport )
#define DLL_CLASS_IMPORT		__declspec( dllimport )

// Can't use extern "C" when DLL exporting a global
#define DLL_GLOBAL_EXPORT		extern __declspec( dllexport )
#define DLL_GLOBAL_IMPORT		extern __declspec( dllimport )

// Pass hints to the compiler to prevent it from generating unnessecary / stupid code
// in certain situations.  Several compilers other than MSVC also have an equivilent
// construct.
//
// Essentially the 'Hint' is that the condition specified is assumed to be true at
// that point in the compilation.  If '0' is passed, then the compiler assumes that
// any subsequent code in the same 'basic block' is unreachable, and thus usually
// removed.
#define HINT(THE_HINT)			__assume((THE_HINT))

// decls for aligning data
#define DECL_ALIGN(x)			__declspec( align( x ) )

// GCC had a few areas where it didn't construct objects in the same order 
// that Windows does. So when CVProfile::CVProfile() would access g_pMemAlloc,
// it would crash because the allocator wasn't initalized yet.
#define CONSTRUCT_EARLY

#define SELECTANY				__declspec(selectany)

#define RESTRICT				__restrict
#define RESTRICT_FUNC			__declspec(restrict)
#define FMTFUNCTION( a, b )
#define NOINLINE

#if !defined( NO_THREAD_LOCAL )
#define DECL_THREAD_LOCAL		__declspec(thread)
#endif 

#define DISABLE_VC_WARNING( x ) __pragma(warning(disable:4310) )
#define DEFAULT_VC_WARNING( x ) __pragma(warning(default:4310) )


#elif defined ( COMPILER_GCC )

#if (CROSS_PLATFORM_VERSION >= 1) && !defined( PLATFORM_64BITS )
#define  STDCALL			__attribute__ ((__stdcall__))
#else
#define  STDCALL
#define  __stdcall			__attribute__ ((__stdcall__))
#endif

#define  FASTCALL
#ifdef _LINUX_DEBUGGABLE
#define  FORCEINLINE
#else
#define  FORCEINLINE		inline
#endif

// GCC 3.4.1 has a bug in supporting forced inline of templated functions
// this macro lets us not force inlining in that case
#define FORCEINLINE_TEMPLATE	inline
#define SINGLE_INHERITANCE
#define MULTIPLE_INHERITANCE
#define EXPLICIT
#define NO_VTABLE

#define NULLTERMINATED			

#define TEMPLATE_STATIC

// Used for dll exporting and importing
#define DLL_EXPORT				extern "C" __attribute__ ((visibility("default")))
#define DLL_IMPORT				extern "C"

// Can't use extern "C" when DLL exporting a class
#define DLL_CLASS_EXPORT		__attribute__ ((visibility("default")))
#define DLL_CLASS_IMPORT

// Can't use extern "C" when DLL exporting a global
#define DLL_GLOBAL_EXPORT		__attribute__((visibility("default")))
#define DLL_GLOBAL_IMPORT		extern

#define HINT(THE_HINT)			0
#define DECL_ALIGN(x)			__attribute__( ( aligned( x ) ) )
#define CONSTRUCT_EARLY			__attribute__((init_priority(101)))
#define SELECTANY				__attribute__((weak))
#define RESTRICT
#define RESTRICT_FUNC
#define FMTFUNCTION( fmtargnumber, firstvarargnumber ) __attribute__ (( format( printf, fmtargnumber, firstvarargnumber )))
#define NOINLINE				__attribute__ ((noinline))

#if !defined( NO_THREAD_LOCAL )
#define DECL_THREAD_LOCAL		__thread
#endif

#define DISABLE_VC_WARNING( x )
#define DEFAULT_VC_WARNING( x )

#else

#define DECL_ALIGN(x)			/* */
#define SELECTANY				static

#endif

#if defined( GNUC )
// gnuc has the align decoration at the end
#define ALIGN4
#define ALIGN8 
#define ALIGN16
#define ALIGN32
#define ALIGN128

#define ALIGN4_POST DECL_ALIGN(4)
#define ALIGN8_POST DECL_ALIGN(8)
#define ALIGN16_POST DECL_ALIGN(16)
#define ALIGN32_POST DECL_ALIGN(32)
#define ALIGN128_POST DECL_ALIGN(128)
#else
// MSVC has the align at the start of the struct
#define ALIGN4 DECL_ALIGN(4)
#define ALIGN8 DECL_ALIGN(8)
#define ALIGN16 DECL_ALIGN(16)
#define ALIGN32 DECL_ALIGN(32)
#define ALIGN128 DECL_ALIGN(128)

#define ALIGN4_POST
#define ALIGN8_POST
#define ALIGN16_POST
#define ALIGN32_POST
#define ALIGN128_POST
#endif


// This can be used to declare an abstract (interface only) class.
// Classes marked abstract should not be instantiated.  If they are, and access violation will occur.
//
// Example of use:
//
// abstract_class CFoo
// {
//      ...
// }
//
// MSDN __declspec(novtable) documentation: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclang/html/_langref_novtable.asp
//
// Note: NJS: This is not enabled for regular PC, due to not knowing the implications of exporting a class with no no vtable.
//       It's probable that this shouldn't be an issue, but an experiment should be done to verify this.
//
#ifndef COMPILER_MSVCX360
#define abstract_class class
#else
#define abstract_class class NO_VTABLE
#endif


//-----------------------------------------------------------------------------
// Why do we need this? It would be nice to make it die die die
//-----------------------------------------------------------------------------
// Alloca defined for this platform
#if defined( COMPILER_MSVC ) && !defined( WINDED )
#if defined(_M_IX86)
#define __i386__	1
#endif
#endif

#if defined __i386__ && !defined __linux__
#define id386	1
#else
#define id386	0
#endif  // __i386__


//-----------------------------------------------------------------------------
// Disable annoying unhelpful warnings
//-----------------------------------------------------------------------------
#ifdef COMPILER_MSVC
// Remove warnings from warning level 4.
#pragma warning(disable : 4514) // warning C4514: 'acosl' : unreferenced inline function has been removed
#pragma warning(disable : 4100) // warning C4100: 'hwnd' : unreferenced formal parameter
#pragma warning(disable : 4127) // warning C4127: conditional expression is constant
#pragma warning(disable : 4512) // warning C4512: 'InFileRIFF' : assignment operator could not be generated
#pragma warning(disable : 4611) // warning C4611: interaction between '_setjmp' and C++ object destruction is non-portable
#pragma warning(disable : 4710) // warning C4710: function 'x' not inlined
#pragma warning(disable : 4702) // warning C4702: unreachable code
#pragma warning(disable : 4505) // unreferenced local function has been removed
#pragma warning(disable : 4239) // nonstandard extension used : 'argument' ( conversion from class Vector to class Vector& )
#pragma warning(disable : 4097) // typedef-name 'baseClass' used as synonym for class-name 'CFlexCycler::CbaseFlex'
#pragma warning(disable : 4324) // Padding was added at the end of a structure
#pragma warning(disable : 4244) // type conversion warning.
#pragma warning(disable : 4305)	// truncation from 'const double ' to 'float '
#pragma warning(disable : 4786)	// Disable warnings about long symbol names
#pragma warning(disable : 4250) // 'X' : inherits 'Y::Z' via dominance
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union

#if _MSC_VER >= 1300
#pragma warning(disable : 4511)	// Disable warnings about private copy constructors
#pragma warning(disable : 4121)	// warning C4121: 'symbol' : alignment of a member was sensitive to packing
#pragma warning(disable : 4530)	// warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc (disabled due to std headers having exception syntax)
#endif

#if _MSC_VER >= 1400
#pragma warning(disable : 4996)	// functions declared deprecated
#endif

// When we port to 64 bit, we'll have to resolve the int, ptr vs size_t 32/64 bit problems...
#if !defined( COMPILER_MSVC64 )
#if ( CROSS_PLATFORM_VERSION < 1 )
#pragma warning( disable : 4267 )	// conversion from 'size_t' to 'int', possible loss of data
#pragma warning( disable : 4311 )	// pointer truncation from 'char *' to 'int'
#pragma warning( disable : 4312 )	// conversion from 'unsigned int' to 'memhandle_t' of greater size
#endif
#endif

#endif


//-----------------------------------------------------------------------------
// Stack-based allocation related helpers
//-----------------------------------------------------------------------------
#if defined( COMPILER_GCC )

#define stackalloc( _size )		alloca( ALIGN_VALUE( _size, 16 ) )

#ifdef PLATFORM_OSX
#define mallocsize( _p )	( malloc_size( _p ) )
#else
#define mallocsize( _p )	( malloc_usable_size( _p ) )
#endif

#elif defined ( COMPILER_MSVC )

#define stackalloc( _size )		_alloca( ALIGN_VALUE( _size, 16 ) )
#define mallocsize( _p )		( _msize( _p ) )

#endif

#define  stackfree( _p )			0


//-----------------------------------------------------------------------------
// Used to break into the debugger
//-----------------------------------------------------------------------------
#ifdef COMPILER_MSVC64
#define DebuggerBreak()		__debugbreak()
#elif COMPILER_MSVC32
#define DebuggerBreak()		__asm { int 3 }
#elif COMPILER_MSVCX360
#define DebuggerBreak()		DebugBreak()
#elif COMPILER_GCC 
#if defined( PLATFORM_CYGWIN ) || defined( PLATFORM_POSIX )
#define DebuggerBreak()		__asm__( "int $0x3;")
#else
#define DebuggerBreak()		asm( "int3" )
#endif
#endif


//-----------------------------------------------------------------------------
// DLL export for platform utilities
//-----------------------------------------------------------------------------
#ifndef STATIC_TIER0

#ifdef TIER0_DLL_EXPORT
#define PLATFORM_INTERFACE	DLL_EXPORT
#define PLATFORM_OVERLOAD	DLL_GLOBAL_EXPORT
#define PLATFORM_CLASS		DLL_CLASS_EXPORT
#else
#define PLATFORM_INTERFACE	DLL_IMPORT
#define PLATFORM_OVERLOAD	DLL_GLOBAL_IMPORT
#define PLATFORM_CLASS		DLL_CLASS_IMPORT
#endif

#else	// BUILD_AS_DLL

#define PLATFORM_INTERFACE	extern
#define PLATFORM_OVERLOAD
#define PLATFORM_CLASS

#endif	// BUILD_AS_DLL

//-----------------------------------------------------------------------------
// Posix platform helpers
//-----------------------------------------------------------------------------
#ifdef PLATFORM_POSIX

// Visual Studio likes to put an underscore in front of anything that looks like a portable function.
#define _strupr strupr
#define _getcwd getcwd
#define _open open
#define _lseek lseek
#define _read read
#define _close close
#define _vsnprintf vsnprintf
#define _stat stat
#define _O_RDONLY O_RDONLY
#define _stricmp strcasecmp
#define _finite finite
#define _unlink unlink
#define _putenv putenv
#define _chdir chdir
#define _access access
#define _strtoi64 strtoll

#if !defined( _snprintf )	// some vpc's define this on the command line
#define _snprintf snprintf
#endif

#include <alloca.h>
#include <unistd.h>											// Get unlink
#include <errno.h>

#endif // PLATFORM_POSIX


//-----------------------------------------------------------------------------
// Generally useful platform-independent macros (move to another file?)
//-----------------------------------------------------------------------------

// need macro for constant expression
#define ALIGN_VALUE( val, alignment ) ( ( val + alignment - 1 ) & ~( alignment - 1 ) ) 

// Force a function call site -not- to inlined. (useful for profiling)
#define DONT_INLINE(a) (((int)(a)+1)?(a):(a))

// Marks the codepath from here until the next branch entry point as unreachable,
// and asserts if any attempt is made to execute it.
#define UNREACHABLE() { Assert(0); HINT(0); }

// In cases where no default is present or appropriate, this causes MSVC to generate
// as little code as possible, and throw an assertion in debug.
#define NO_DEFAULT default: UNREACHABLE();

// Defines MAX_PATH
#ifndef MAX_PATH
#define MAX_PATH  260
#endif


//-----------------------------------------------------------------------------
// FP exception handling
//-----------------------------------------------------------------------------
//#define CHECK_FLOAT_EXCEPTIONS		1
//#define CHECK_FPU_CONTROL_WORD_SET	1	// x360 only

#if defined( COMPILER_MSVC64 )

inline void SetupFPUControlWord()
{
}

#elif defined ( COMPILER_MSVC32 )

inline void SetupFPUControlWordForceExceptions()
{
	// use local to Get and store control word
	uint16 tmpCtrlW;
	__asm
	{
		fnclex						/* clear all current exceptions */
		fnstcw word ptr[tmpCtrlW]	/* Get current control word */
		and [tmpCtrlW], 0FCC0h		/* Keep infinity control + rounding control */
		or [tmpCtrlW], 0230h		/* Set to 53-bit, mask only inexact, underflow */
		fldcw word ptr[tmpCtrlW]	/* put new control word in FPU */
	}
}

#ifdef CHECK_FLOAT_EXCEPTIONS

inline void SetupFPUControlWord()
{
	SetupFPUControlWordForceExceptions();
}

#else

inline void SetupFPUControlWord()
{
	// use local to Get and store control word
	uint16 tmpCtrlW;
	__asm
	{
		fnstcw word ptr[tmpCtrlW]	/* Get current control word */
		and [tmpCtrlW], 0FCC0h		/* Keep infinity control + rounding control */
		or [tmpCtrlW], 023Fh		/* Set to 53-bit, mask only inexact, underflow */
		fldcw word ptr[tmpCtrlW]	/* put new control word in FPU */
	}
}

#endif

#elif defined ( COMPILER_GCC )

inline void SetupFPUControlWord()
{
	__volatile unsigned short int __cw;
	__asm __volatile("fnstcw %0" : "=m" (__cw));
	__cw = __cw & 0x0FCC0;	// keep infinity control, keep rounding mode
	__cw = __cw | 0x023F;	// Set 53-bit, no exceptions
	__asm __volatile("fldcw %0" : : "m" (__cw));
}

#elif defined( COMPILER_MSVCX360 )

#ifdef CHECK_FPU_CONTROL_WORD_SET
FORCEINLINE bool IsFPUControlWordSet()
{
	float f = 0.996f;
	union
	{
		double flResult;
		int pResult[2];
	};
	flResult = __fctiw(f);
	return (pResult[1] == 1);
}
#else
#define IsFPUControlWordSet() true
#endif

inline void SetupFPUControlWord()
{
	// Set round-to-nearest in FPSCR
	// (cannot assemble, must use op-code form)
	__emit(0xFF80010C);	// mtfsfi  7,0

						// Favour compatibility over speed (make sure the VPU Set to Java-compliant mode)
						// NOTE: the VPU *always* uses round-to-nearest
	__vector4  a = { 0.0f, 0.0f, 0.0f, 0.0f };
	a;				//	Avoid compiler warning
	__asm
	{
		mtvscr a;	// Clear the Vector Status & Control Register to zero
	}
}

#endif // COMPILER_MSVCX360


//-----------------------------------------------------------------------------
// Purpose: Standard functions for handling endian-ness
//-----------------------------------------------------------------------------

//-------------------------------------
// Basic swaps
//-------------------------------------

template <typename T>
T WordSwapC(T w)
{
	uint16_t temp = ((*((uint16_t*)&w) & 0xff00) >> 8);
	temp |= ((*((uint16_t*)&w) & 0x00ff) << 8);

	return *((T*)&temp);
}

template <typename T>
T DWordSwapC(T dw)
{
	uint32_t temp = *((uint32_t*)&dw) >> 24;
	temp |= ((*((uint32_t*)&dw) & 0x00FF0000) >> 8);
	temp |= ((*((uint32_t*)&dw) & 0x0000FF00) << 8);
	temp |= ((*((uint32_t*)&dw) & 0x000000FF) << 24);

	return *((T*)&temp);
}

//-------------------------------------
// Fast swaps
//-------------------------------------

#if defined( COMPILER_MSVCX360 )

#define WordSwap  WordSwap360Intr
#define DWordSwap DWordSwap360Intr

template <typename T>
inline T WordSwap360Intr(T w)
{
	T output;
	__storeshortbytereverse(w, 0, &output);
	return output;
}

template <typename T>
inline T DWordSwap360Intr(T dw)
{
	T output;
	__storewordbytereverse(dw, 0, &output);
	return output;
}

#elif defined( COMPILER_MSVC32 )

#define WordSwap  WordSwapAsm
#define DWordSwap DWordSwapAsm

#pragma warning(push)
#pragma warning (disable:4035) // no return value

template <typename T>
inline T WordSwapAsm(T w)
{
	__asm
	{
		mov ax, w
		xchg al, ah
	}
}

template <typename T>
inline T DWordSwapAsm(T dw)
{
	__asm
	{
		mov eax, dw
		bswap eax
	}
}

#pragma warning(pop)

#else

#define WordSwap  WordSwapC
#define DWordSwap DWordSwapC

#endif

//-------------------------------------
// The typically used methods.
//-------------------------------------

#if defined( _SGI_SOURCE ) || defined( PLATFORM_X360 )
#define	PLAT_BIG_ENDIAN 1
#else
#define PLAT_LITTLE_ENDIAN 1
#endif


// If a swapped float passes through the fpu, the bytes may Get changed.
// Prevent this by swapping floats as DWORDs.
#define SafeSwapFloat( pOut, pIn )	(*((unsigned int*)pOut) = DWordSwap( *((unsigned int*)pIn) ))

#if defined(PLAT_LITTLE_ENDIAN)
#define BigShort( val )				WordSwap( val )
#define BigWord( val )				WordSwap( val )
#define BigLong( val )				DWordSwap( val )
#define BigDWord( val )				DWordSwap( val )
#define LittleShort( val )			( val )
#define LittleWord( val )			( val )
#define LittleLong( val )			( val )
#define LittleDWord( val )			( val )
#define SwapShort( val )			BigShort( val )
#define SwapWord( val )				BigWord( val )
#define SwapLong( val )				BigLong( val )
#define SwapDWord( val )			BigDWord( val )

// Pass floats by pointer for swapping to avoid truncation in the fpu
#define BigFloat( pOut, pIn )		SafeSwapFloat( pOut, pIn )
#define LittleFloat( pOut, pIn )	( *pOut = *pIn )
#define SwapFloat( pOut, pIn )		BigFloat( pOut, pIn )

#elif defined(PLAT_BIG_ENDIAN)

#define BigShort( val )				( val )
#define BigWord( val )				( val )
#define BigLong( val )				( val )
#define BigDWord( val )				( val )
#define LittleShort( val )			WordSwap( val )
#define LittleWord( val )			WordSwap( val )
#define LittleLong( val )			DWordSwap( val )
#define LittleDWord( val )			DWordSwap( val )
#define SwapShort( val )			LittleShort( val )
#define SwapWord( val )				LittleWord( val )
#define SwapLong( val )				LittleLong( val )
#define SwapDWord( val )			LittleDWord( val )

// Pass floats by pointer for swapping to avoid truncation in the fpu
#define BigFloat( pOut, pIn )		( *pOut = *pIn )
#define LittleFloat( pOut, pIn )	SafeSwapFloat( pOut, pIn )
#define SwapFloat( pOut, pIn )		LittleFloat( pOut, pIn )

#else

// @Note (toml 05-02-02): this technique expects the compiler to
// optimize the expression and eliminate the other path. On any new
// platform/compiler this should be tested.
inline short BigShort(short val) { int test = 1; return (*(char*)&test == 1) ? WordSwap(val) : val; }
inline uint16 BigWord(uint16 val) { int test = 1; return (*(char*)&test == 1) ? WordSwap(val) : val; }
inline long BigLong(long val) { int test = 1; return (*(char*)&test == 1) ? DWordSwap(val) : val; }
inline uint32_t BigDWord(uint32_t val) { int test = 1; return (*(char*)&test == 1) ? DWordSwap(val) : val; }
inline short LittleShort(short val) { int test = 1; return (*(char*)&test == 1) ? val : WordSwap(val); }
inline uint16 LittleWord(uint16 val) { int test = 1; return (*(char*)&test == 1) ? val : WordSwap(val); }
inline long LittleLong(long val) { int test = 1; return (*(char*)&test == 1) ? val : DWordSwap(val); }
inline uint32_t LittleDWord(uint32_t val) { int test = 1; return (*(char*)&test == 1) ? val : DWordSwap(val); }
inline short SwapShort(short val) { return WordSwap(val); }
inline uint16 SwapWord(uint16 val) { return WordSwap(val); }
inline long SwapLong(long val) { return DWordSwap(val); }
inline uint32_t SwapDWord(uint32_t val) { return DWordSwap(val); }

// Pass floats by pointer for swapping to avoid truncation in the fpu
inline void BigFloat(float* pOut, const float* pIn) { int test = 1; (*(char*)&test == 1) ? SafeSwapFloat(pOut, pIn) : (*pOut = *pIn); }
inline void LittleFloat(float* pOut, const float* pIn) { int test = 1; (*(char*)&test == 1) ? (*pOut = *pIn) : SafeSwapFloat(pOut, pIn); }
inline void SwapFloat(float* pOut, const float* pIn) { SafeSwapFloat(pOut, pIn); }

#endif

inline uint32_t load_little_dword(uint32_t* base, unsigned int dwordIndex)
{
	return LittleDWord(base[dwordIndex]);
}

inline void store_little_dword(uint32_t* base, unsigned int dwordIndex, uint32_t dword)
{
	base[dwordIndex] = LittleDWord(dword);
}

// Protect against bad auto operator=
#define DISALLOW_OPERATOR_EQUAL( _classname )			\
	private:											\
		_classname &operator=( const _classname & );	\
	public:

// Define a reasonable operator=
#define IMPLEMENT_OPERATOR_EQUAL( _classname )			\
	public:												\
		_classname &operator=( const _classname &src )	\
		{												\
			memcpy( this, &src, sizeof(_classname) );	\
			return *this;								\
		}

#if _X360
#define Plat_FastMemset XMemSet
#define Plat_FastMemcpy XMemCpy
#else
#define Plat_FastMemset memset
#define Plat_FastMemcpy memcpy
#endif

//-----------------------------------------------------------------------------
// XBOX Components valid in PC compilation space
//-----------------------------------------------------------------------------

#define XBOX_DVD_SECTORSIZE			2048
#define XBOX_DVD_ECC_SIZE			32768 // driver reads in quantum ECC blocks
#define XBOX_HDD_SECTORSIZE			512

// Custom windows messages for Xbox input
#define WM_XREMOTECOMMAND					(WM_USER + 100)
#define WM_XCONTROLLER_KEY					(WM_USER + 101)
#define WM_SYS_UI							(WM_USER + 102)
#define WM_SYS_SIGNINCHANGED				(WM_USER + 103)
#define WM_SYS_STORAGEDEVICESCHANGED		(WM_USER + 104)
#define WM_SYS_PROFILESETTINGCHANGED		(WM_USER + 105)
#define WM_SYS_MUTELISTCHANGED				(WM_USER + 106)
#define WM_SYS_INPUTDEVICESCHANGED			(WM_USER + 107)
#define WM_SYS_INPUTDEVICECONFIGCHANGED		(WM_USER + 108)
#define WM_LIVE_CONNECTIONCHANGED			(WM_USER + 109)
#define WM_LIVE_INVITE_ACCEPTED				(WM_USER + 110)
#define WM_LIVE_LINK_STATE_CHANGED			(WM_USER + 111)
#define WM_LIVE_CONTENT_INSTALLED			(WM_USER + 112)
#define WM_LIVE_MEMBERSHIP_PURCHASED		(WM_USER + 113)
#define WM_LIVE_VOICECHAT_AWAY				(WM_USER + 114)
#define WM_LIVE_PRESENCE_CHANGED			(WM_USER + 115)
#define WM_FRIENDS_PRESENCE_CHANGED			(WM_USER + 116)
#define WM_FRIENDS_FRIEND_ADDED				(WM_USER + 117)
#define WM_FRIENDS_FRIEND_REMOVED			(WM_USER + 118)
#define WM_CUSTOM_GAMEBANNERPRESSED			(WM_USER + 119)
#define WM_CUSTOM_ACTIONPRESSED				(WM_USER + 120)
#define WM_XMP_STATECHANGED					(WM_USER + 121)
#define WM_XMP_PLAYBACKBEHAVIORCHANGED		(WM_USER + 122)
#define WM_XMP_PLAYBACKCONTROLLERCHANGED	(WM_USER + 123)

inline const char* GetPlatformExt(void)
{
	return IsPlatformX360() ? ".360" : "";
}

// flat view, 6 hw threads
#define XBOX_PROCESSOR_0			( 1<<0 )
#define XBOX_PROCESSOR_1			( 1<<1 )
#define XBOX_PROCESSOR_2			( 1<<2 )
#define XBOX_PROCESSOR_3			( 1<<3 )
#define XBOX_PROCESSOR_4			( 1<<4 )
#define XBOX_PROCESSOR_5			( 1<<5 )

// core view, 3 cores with 2 hw threads each
#define XBOX_CORE_0_HWTHREAD_0		XBOX_PROCESSOR_0
#define XBOX_CORE_0_HWTHREAD_1		XBOX_PROCESSOR_1
#define XBOX_CORE_1_HWTHREAD_0		XBOX_PROCESSOR_2
#define XBOX_CORE_1_HWTHREAD_1		XBOX_PROCESSOR_3
#define XBOX_CORE_2_HWTHREAD_0		XBOX_PROCESSOR_4
#define XBOX_CORE_2_HWTHREAD_1		XBOX_PROCESSOR_5

//-----------------------------------------------------------------------------
// Include additional dependant header components.
//-----------------------------------------------------------------------------
#if defined( PLATFORM_X360 )
#include "xbox/xbox_core.h"
#endif
#include <winnt.h>

//-----------------------------------------------------------------------------
// Methods to invoke the constructor, copy constructor, and destructor
//-----------------------------------------------------------------------------

template <class T>
T* construct(T* memory)
{
	return ::new(memory) T;
}

template <class T, typename ARG1>
T* construct(T* memory, ARG1 a1)
{
	return ::new(memory) T(a1);
}

template <class T, typename ARG1, typename ARG2>
T* construct(T* memory, ARG1 a1, ARG2 a2)
{
	return ::new(memory) T(a1, a2);
}

template <class T, typename ARG1, typename ARG2, typename ARG3>
T* construct(T* memory, ARG1 a1, ARG2 a2, ARG3 a3)
{
	return ::new(memory) T(a1, a2, a3);
}

template <class T, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
T* construct(T* memory, ARG1 a1, ARG2 a2, ARG3 a3, ARG4 a4)
{
	return ::new(memory) T(a1, a2, a3, a4);
}

template <class T, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
T* construct(T* memory, ARG1 a1, ARG2 a2, ARG3 a3, ARG4 a4, ARG5 a5)
{
	return ::new(memory) T(a1, a2, a3, a4, a5);
}

template <class T>
T* copy_construct(T* memory, T const& src)
{
	return ::new(memory) T(src);
}

template <class T>
void destruct(T* memory)
{
	memory->~T();

#ifdef _DEBUG
	memset(memory, 0xDD, sizeof(T));
#endif
}


//
// GET_OUTER()
//
// A platform-independent way for a contained class to Get a pointer to its
// owner. If you know a class is exclusively used in the context of some
// "outer" class, this is a much more space efficient way to Get at the outer
// class than having the inner class store a pointer to it.
//
//	class COuter
//	{
//		class CInner // Note: this does not need to be a nested class to work
//		{
//			void PrintAddressOfOuter()
//			{
//				printf( "Outer is at 0x%x\n", GET_OUTER( COuter, m_Inner ) );
//			}
//		};
//
//		CInner m_Inner;
//		friend class CInner;
//	};

#define GET_OUTER( OuterType, OuterMember ) \
   ( ( OuterType * ) ( (uint8_t *)this - offsetof( OuterType, OuterMember ) ) )


/*	TEMPLATE_FUNCTION_TABLE()

(Note added to platform.h so platforms that correctly support templated
functions can handle portions as templated functions rather than wrapped
functions)

Helps automate the process of creating an array of function
templates that are all specialized by a single integer.
This sort of thing is often useful in optimization work.

For example, using TEMPLATE_FUNCTION_TABLE, this:

TEMPLATE_FUNCTION_TABLE(int, Function, ( int blah, int blah ), 10)
{
return argument * argument;
}

is equivilent to the following:

(NOTE: the function has to be wrapped in a class due to code
generation bugs involved with directly specializing a function
based on a constant.)

template<int argument>
class FunctionWrapper
{
public:
int Function( int blah, int blah )
{
return argument*argument;
}
}

typedef int (*FunctionType)( int blah, int blah );

class FunctionName
{
public:
enum { count = 10 };
FunctionType functions[10];
};

FunctionType FunctionName::functions[] =
{
FunctionWrapper<0>::Function,
FunctionWrapper<1>::Function,
FunctionWrapper<2>::Function,
FunctionWrapper<3>::Function,
FunctionWrapper<4>::Function,
FunctionWrapper<5>::Function,
FunctionWrapper<6>::Function,
FunctionWrapper<7>::Function,
FunctionWrapper<8>::Function,
FunctionWrapper<9>::Function
};
*/

PLATFORM_INTERFACE bool vtune(bool resume);


#define TEMPLATE_FUNCTION_TABLE(RETURN_TYPE, NAME, ARGS, count)			  \
                                                                      \
typedef RETURN_TYPE (FASTCALL *__Type_##NAME) ARGS;						        \
																		                                  \
template<const int nArgument>											                    \
struct __Function_##NAME												                      \
{																		                                  \
	static RETURN_TYPE FASTCALL Run ARGS;								                \
};																		                                \
																		                                  \
template <const int i>														                    \
struct __MetaLooper_##NAME : __MetaLooper_##NAME<i-1>					        \
{																		                                  \
	__Type_##NAME func;													                        \
	inline __MetaLooper_##NAME() { func = __Function_##NAME<i>::Run; }	\
};																		                                \
																		                                  \
template<>																                            \
struct __MetaLooper_##NAME<0>											                    \
{																		                                  \
	__Type_##NAME func;													                        \
	inline __MetaLooper_##NAME() { func = __Function_##NAME<0>::Run; }	\
};																		                                \
																		                                  \
class NAME																                            \
{																		                                  \
private:																                              \
    static const __MetaLooper_##NAME<count> m;							          \
public:																	                              \
	enum { count = count };												                      \
	static const __Type_##NAME* functions;								              \
};																		                                \
const __MetaLooper_##NAME<count> NAME::m;								              \
const __Type_##NAME* NAME::functions = (__Type_##NAME*)&m;				    \
template<const int nArgument>													                \
RETURN_TYPE FASTCALL __Function_##NAME<nArgument>::Run ARGS


#define LOOP_INTERCHANGE(BOOLEAN, CODE)\
	if( (BOOLEAN) )\
	{\
		CODE;\
	} else\
	{\
		CODE;\
	}

#ifdef COMPILER_MSVC
FORCEINLINE uint32_t rotate_bits_left32(uint32_t nValue, int nRotateBits)
{
	return _rotl(nValue, nRotateBits);
}
FORCEINLINE uint64_t rotate_bits_left64(uint64_t nValue, int nRotateBits)
{
	return _rotl64(nValue, nRotateBits);
}
FORCEINLINE uint32_t rotate_bits_right32(uint32_t nValue, int nRotateBits)
{
	return _rotr(nValue, nRotateBits);
}
FORCEINLINE uint64_t rotate_bits_right64(uint64_t nValue, int nRotateBits)
{
	return _rotr64(nValue, nRotateBits);
}
#endif

template< typename T, class A >
void c_utl_vector<T, A>::remove_all()
{
	for (int i = m_size; --i >= 0; ) {
		destruct(&element(i));
	}

	m_size = 0;
}


//-----------------------------------------------------------------------------
// Memory deallocation
//-----------------------------------------------------------------------------

template< typename T, class A >
void c_utl_vector<T, A>::purge()
{
	remove_all();
	m_memory.Purge();
	reset_dbg_info();
}


template< typename T, class A >
void c_utl_vector<T, A>::purge_and_delete_elements()
{
	for (int i = 0; i < m_size; i++) {
		delete element(i);
	}
	purge();
}

template< typename T, class A >
void c_utl_vector<T, A>::compact()
{
	m_memory.Purge(m_size);
}

template< typename T, class A >
int c_utl_vector<T, A>::num_allocated() const
{
	return m_memory.NumAllocated();
}


//-----------------------------------------------------------------------------
// Data and memory validation
//-----------------------------------------------------------------------------
#ifdef DBGFLAG_VALIDATE
template< typename T, class A >
void CUtlVector<T, A>::Validate(CValidator& validator, char* pchName)
{
	validator.Push(typeid(*this).name(), this, pchName);

	m_Memory.Validate(validator, "m_Memory");

	validator.Pop();
}
#endif // DBGFLAG_VALIDATE

// A vector class for storing pointers, so that the elements pointed to by the pointers are deleted
// on exit.
template<class T> class c_utl_vector_auto_purge : public c_utl_vector< T, c_utl_memory< T, int> >
{
public:
	~c_utl_vector_auto_purge(void)
	{
		this->PurgeAndDeleteelements();
	}
};

// easy string list class with dynamically allocated strings. For use with V_SplitString, etc.
// Frees the dynamic strings in destructor.
class c_utl_string_list : public c_utl_vector_auto_purge< char*>
{
public:
	void copy_and_add_to_tail(char const* pString)			// clone the string and add to the end
	{
		const auto new_str = new char[1 + strlen(pString)];
		strcpy_s(new_str, 1 + strlen(pString), pString);
		add_to_tail(new_str);
	}

	static int __cdecl sort_func(char* const* sz1, char* const* sz2)
	{
		return strcmp(*sz1, *sz2);
	}

};
