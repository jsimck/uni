////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	entity.h
//
// summary:	Declaration and implementation of Entity class
// author:	Petr Gajdo�
// 
// Copyright � 2014 Petr Gajdo�. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ENTITY_H_
#define __ENTITY_H_

#include "stdafx.h"

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_inverse.hpp>
#include <gtx/quaternion.hpp>

#include "sceneSetting.h"
#include "shaderProgram.h"
#include "material.h"
#include "light.h"

using namespace glm;

class Entity
{
protected:
	glm::vec3 m_localX;
	glm::vec3 m_localY;
	glm::vec3 m_localZ;

	glm::vec3 m_position;
	glm::quat m_orientation;

public:
	glm::mat4 m_modelMatrix;
	bool m_isInitialized;
	bool m_castShadows;
	Material *m_material;					//Pointer only. The data is stored in sceneData instance

public:
	inline Entity(void) : m_isInitialized(false), m_castShadows(true), m_material(0)
	{
		m_localX = glm::vec3(1.0f, 0.0f, 0.0f);
		m_localY = glm::vec3(0.0f, 1.0f, 0.0f);
		m_localZ = glm::vec3(0.0f, 0.0f, 1.0f);
		m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		m_modelMatrix = glm::mat4(1.0f);
	}
	virtual ~Entity(void){}

	virtual void init(){}
	virtual void draw(const unsigned int eid = 0){}
	virtual void drawForPicking(){}
	virtual void print(){}
	virtual void idle(){}
	virtual void update() {}

	virtual void setPosition(const float x, const float y, const float z);
	virtual void setScale(const float sx, const float sy, const float sz);
	virtual void setOrientation(const float pitchDegrees, const float headingDegrees, const float rollDegrees);
	
	virtual void rotate(const float angle, const float ax, const float ay, const float az);

	virtual glm::vec4 getPosition();
protected:
	virtual void updateTransforms();
	virtual void extractAxes();
};

inline void Entity::setPosition(const float x, const float y, const float z) 
{ 
	m_position.x = x; 
	m_position.y = y;
	m_position.z = z;
	m_modelMatrix[3][0] = m_position.x;
	m_modelMatrix[3][1] = m_position.y;
	m_modelMatrix[3][2] = m_position.z;
}

inline void Entity::setScale(const float sx, const float sy, const float sz) 
{ 
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(sx, sy, sz));
}

inline void Entity::setOrientation(const float pitchDegrees, const float headingDegrees, const float rollDegrees)
{
	m_orientation = glm::quat(glm::vec3(glm::radians(pitchDegrees), glm::radians(headingDegrees), glm::radians(rollDegrees)));
	m_orientation  = glm::normalize(m_orientation);
	updateTransforms();
}

inline void Entity::rotate(const float angle, const float ax, const float ay, const float az)
{
	glm::vec3 axis = glm::normalize(glm::vec3(ax, ay,az));
	glm::quat rot = glm::angleAxis(angle,axis);
	m_orientation = glm::normalize(m_orientation * rot);
	updateTransforms();
}

inline void Entity::updateTransforms()
{
	m_modelMatrix = glm::toMat4(m_orientation);
	m_modelMatrix[3][0] = m_position.x;
	m_modelMatrix[3][1] = m_position.y;
	m_modelMatrix[3][2] = m_position.z;
	extractAxes();
}

inline void Entity::extractAxes()
{
	m_localX = glm::vec3(m_modelMatrix[0][0], m_modelMatrix[1][0], m_modelMatrix[2][0]);
	m_localY = glm::vec3(m_modelMatrix[0][1], m_modelMatrix[1][1], m_modelMatrix[2][1]);
	m_localZ = glm::vec3(m_modelMatrix[0][2], m_modelMatrix[1][2], m_modelMatrix[2][2]);

	m_localX = glm::normalize(m_localX);
	m_localY = glm::normalize(m_localY);
	m_localZ = glm::normalize(m_localZ);
}

inline glm::vec4 Entity::getPosition()
{
	return this->m_modelMatrix * glm::vec4(this->m_position, 1.0f);
}

#endif