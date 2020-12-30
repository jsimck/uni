////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	light.h
//
// summary:	Declaration and implementation of Light structure
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __LIGHT_H_
#define __LIGHT_H_

#include <string.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <sstream>
#include <string>

#include "shaderProgram.h"

static const float lightViewMatrix[6*16] =
{
	0.0f, 0.0f, -1.0f, 0.0f,	0.0f, -1.0f,  0.0f, 0.0f,	-1.0f,	0.0f,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 1.0f,	//posX
	0.0f, 0.0f,  1.0f, 0.0f,	0.0f, -1.0f,  0.0f, 0.0f,	 1.0f,  0.0f,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 1.0f,	//negX
	1.0f, 0.0f,  0.0f, 0.0f,	0.0f,  0.0f, -1.0f, 0.0f,	 0.0f,  1.0f,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 1.0f,	//posY
	1.0f, 0.0f,  0.0f, 0.0f,	0.0f,  0.0f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 1.0f,	//negY
	1.0f, 0.0f,  0.0f, 0.0f,	0.0f, -1.0f,  0.0f, 0.0f,	 0.0f,  0.0f, -1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 1.0f,	//posZ
   -1.f,  0.0f,  0.0f, 0.0f,	0.0f, -1.0f,  0.0f, 0.0f,	 0.0f,  0.0f,  1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 1.0f	//negZ
};


struct Light
{
	float m_position[4];				//Light position   X,Y,Z, directional=0 OR positional=1

	float m_ambient[4];					//Light ambient color
	float m_diffuse[4];					//Light diffuse color
	float m_specular[4];				//Light specular color

	float intensity;
	float constant;
	float linear;
	float quadratic;
	
	float m_spotDirection[4];			//Specifies light direction in LOCAL OBJECT SPACE !!!
	int m_spotExponent;					//Specifies intensity distribution of spotlight
	float m_spotCutoff;					//Specifies maximum spread angle of spotlight (180 = off).
	float m_farZ;						//= light range

	glm::mat4 m_mvMatrix;
	glm::mat4 m_pMatrix;
	glm::mat4 m_mvpMatrix;

	static char tmp[128];

	Light()
	{
		m_position[0] = 5.0f; m_position[1] = 5.0f; m_position[2] = 5.0f; m_position[3] = 1.0f;
		//position[0] = position[1] = -5.0f; position[2] = 5.0f; position[3] = 1.0f;

		m_ambient[0] = m_ambient[1] = m_ambient[2] = 0.2f; m_ambient[3] = 1.0f;
		m_diffuse[0] = m_diffuse[1] = m_diffuse[2] = 0.8f; m_diffuse[3] = 1.0f;
		m_specular[0] = m_specular[1] = m_specular[2] = 0.8f; m_specular[3] = 1.0f;
	
		m_spotDirection[0] = 0.0f;
		m_spotDirection[1] = 0.0f;
		m_spotDirection[2] = -1.0f;
		m_spotDirection[3] = 1.0f;

		m_spotExponent = 10;
		m_spotCutoff = 180.0f;
		intensity = 1.0f;
		constant = 1.0f;
		linear = 0.35f;
		quadratic = 0.44f;

		if (m_position[3] == 1)
		{
			m_farZ = 2.0;
			m_pMatrix = glm::perspective<float>(90, 1.0f, 0.001f, m_farZ);
		}
		else
		{
			m_farZ = 100.0;
			m_pMatrix = glm::perspective<float>(2.0f * m_spotCutoff, 1.0f, 0.001f, m_farZ);
		}
		m_mvMatrix = glm::mat4(1);
		m_mvpMatrix = m_pMatrix;
	}


	inline void setRange(const float range)
	{
		m_farZ = range;
		m_farZ = range;
		m_pMatrix = (m_position[3] == 1) ? glm::perspective<float>(90, 1.0f, 0.001f, m_farZ) : glm::perspective<float>(2.0f * m_spotCutoff, 1.0f, 0.001f, m_farZ);
		m_mvpMatrix = m_pMatrix;
	}

	inline void updateMVP(const GLenum cubeMapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X)
	{
		if (m_position[3] == 1)		//Point Light
		{ 
			unsigned int cubeMapFaceIndex = cubeMapFace - GL_TEXTURE_CUBE_MAP_POSITIVE_X;
			float* ptr = glm::value_ptr(m_mvMatrix);

			memcpy_s(ptr, 16 * sizeof(float), &lightViewMatrix[cubeMapFaceIndex * 16], 16 * sizeof(float));			//set the lightViewMatrix
			//memcpy_s(&ptr[12], 3 * sizeof(float), m_position, 3 * sizeof(float));									//translate to the m_position

			//ptr[12] = -m_position[0];
			//ptr[13] = -m_position[1];
			//ptr[14] = -m_position[2];
			m_mvMatrix = glm::translate(m_mvMatrix, glm::vec3(-m_position[0], -m_position[1], -m_position[2]));

			m_mvpMatrix = m_pMatrix * m_mvMatrix;
		}
		else						//Directional light
		{	
			//TODO
		}
	}

	static void setShaderUniform(const Light* light, const ShaderProgram *spPtr, const char* variableName, int index = -1)
	{
		std::stringstream ss;
		std::string tmp;
		int uniform;

		ss << std::string(variableName);

		if (index < 0) {
			ss << ".";
		} else {
			ss << "[" << index << "].";
		}

		tmp = ss.str() + "position";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform4fv(uniform, 1, light->m_position);

		tmp = ss.str() + "ambient";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform4fv(uniform, 1, light->m_ambient);

		tmp = ss.str() + "diffuse";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform4fv(uniform, 1, light->m_diffuse);

		tmp = ss.str() + "specular";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform4fv(uniform, 1, light->m_specular);

		tmp = ss.str() + "spotDirection";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform4fv(uniform, 1, light->m_spotDirection);

		tmp = ss.str() + "spotExponent";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform1i(uniform, light->m_spotExponent);

		tmp = ss.str() + "spotCutoff";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform1f(uniform, light->m_spotCutoff);

		tmp = ss.str() + "farZ";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform1f(uniform, light->m_farZ);

		tmp = ss.str() + "intensity";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform1f(uniform, light->intensity);

		tmp = ss.str() + "constant";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform1f(uniform, light->constant);

		tmp = ss.str() + "linear";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform1f(uniform, light->linear);

		tmp = ss.str() + "quadratic";
		if ((uniform = glGetUniformLocation(spPtr->m_programObject, tmp.c_str())) >= 0)
			glUniform1f(uniform, light->quadratic);
	}

};

#endif