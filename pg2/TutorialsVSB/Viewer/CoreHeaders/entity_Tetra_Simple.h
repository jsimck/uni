////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Entity_Tetra_Simple.h
//
// summary:	Declaration and implementation of Entity_Cube class
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ENTITY_TETRA_SIMPLE_H_
#define __ENTITY_TETRA_SIMPLE_H_

#include "stdafx.h"
#include "entity_VAO.h"

class Entity_Tetra_Simple : public Entity_VAO
{
public:
	Entity_Tetra_Simple(VAO* vao = nullptr) : Entity_VAO(vao) {}
	~Entity_Tetra_Simple(void) {}

	void draw(const unsigned int eid = 0);
};

inline void Entity_Tetra_Simple::draw(const unsigned int eid)
{
	if (!m_isInitialized) return;

	glBindVertexArray(m_vao->m_object);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

#endif