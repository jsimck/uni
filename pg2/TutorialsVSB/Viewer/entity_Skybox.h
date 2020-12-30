#pragma once

#include "stdafx.h"
#include "entity_VAO.h"

class Entity_Skybox : public Entity_VAO {
public:
	Entity_Skybox(VAO* vao = nullptr) : Entity_VAO(vao) {}
	~Entity_Skybox(void) {}

	void draw(const unsigned int eid = 0);
};

inline void Entity_Skybox::draw(const unsigned int eid) {
	if (!m_isInitialized) return;

	glBindVertexArray(m_vao->m_object);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}