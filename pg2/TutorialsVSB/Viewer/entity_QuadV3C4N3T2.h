#pragma once

#include "stdafx.h"
#include "entity_VAO.h"

class Entity_QuadV3C4N3T2 : public Entity_VAO {
public:
	Entity_QuadV3C4N3T2(VAO* vao = nullptr) : Entity_VAO(vao) {}
	~Entity_QuadV3C4N3T2(void) {}

	void draw(const unsigned int eid = 0);
};

inline void Entity_QuadV3C4N3T2::draw(const unsigned int eid) {
	if (!m_isInitialized) return;

	glBindVertexArray(m_vao->m_object);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}