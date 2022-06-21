#pragma once
#include "../../math/vec2_t.h"

class vertex_t {
public:
	vector_2d m_pos;
	vector_2d m_coord;
	__forceinline vertex_t() {}

	__forceinline vertex_t(float x, float y) : m_pos{ x, y }, m_coord{ 0.f, 0.f } {}
	__forceinline vertex_t(const vector_2d& pos) : m_pos{ pos }, m_coord{ 0.f, 0.f } {}
	__forceinline vertex_t(const vector_2d& pos, const vector_2d& coord) : m_pos{ pos }, m_coord{ coord } {}

	__forceinline void init(const vector_2d& pos, const vector_2d& coord = { 0, 0 }) {
		m_pos = pos;
		m_coord = coord;
	}
};