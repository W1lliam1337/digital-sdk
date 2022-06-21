#pragma once

template <typename T>
struct bit_flag_t {
	bit_flag_t() = default;
	bit_flag_t(const T& value) { m_value = value; }

	__forceinline bool has(const T& value) const { return m_value & value; }

	__forceinline void add(const T& value) { m_value |= value; }

	__forceinline void remove(const T& value) { m_value &= ~value; }

	__forceinline void clear() { m_value = {}; }

	__forceinline bool empty() const { return m_value == std::numeric_limits<T>::quiet_NaN(); }

	__forceinline operator T() const { return m_value; }

	__forceinline bit_flag_t<T>& operator=(const bit_flag_t<T>& value) {
		m_value = value.m_value;

		return *this;
	}

	__forceinline T& operator=(const T& value) {
		m_value = value;

		return m_value;
	}

	T m_value = {};
};
