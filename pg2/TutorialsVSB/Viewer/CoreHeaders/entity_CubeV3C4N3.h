////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	entity_CubeV3C4N3.h
//
// summary:	Declaration and implementation of Entity_Cube class
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ENTITY_CUBE_V3C4N3_H_
#define __ENTITY_CUBE_V3C4N3_H_

#include "stdafx.h"
#include "entity_VAO.h"

class Entity_CubeV3C4N3 : public Entity_VAO
{
public:
	Entity_CubeV3C4N3(VAO* vao = nullptr) : Entity_VAO(vao) {}
	~Entity_CubeV3C4N3(void) {}

	void draw(const unsigned int eid = 0);
};

inline void Entity_CubeV3C4N3::draw(const unsigned int eid)
{
	if (!m_isInitialized) return;

	glBindVertexArray(m_vao->m_object);
	glDrawArrays(GL_QUADS, 0, 24);
	glBindVertexArray(0);
}

#endif