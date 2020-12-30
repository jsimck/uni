#pragma once

#include "vao.h"

class VAO_QuadV3C4T2 : public VAO
{
public:
	VAO_QuadV3C4T2() : VAO() {}
	virtual ~VAO_QuadV3C4T2() {}

	virtual void init() {
		float tmpVertices[6 * 9] = {
		//    x      y     z          r    g     b     a         u     v
			-1.0f, -1.0f, 0.0f,     1.0f, 0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,     0.0f, 1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,     1.0f, 0.0f, 0.0f, 0.0f,     1.0f, 1.0f,

		    -1.0f, -1.0f, 0.0f,     1.0f, 0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,     1.0f, 0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
		};

		GLuint tmpBuffer;
		glGenBuffers(1, &tmpBuffer);
		initBuffer(tmpBuffer, tmpVertices, 6 * 9 * sizeof(float), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_buffers.push_back(tmpBuffer);

		glGenVertexArrays(1, &m_object);
		glBindVertexArray(m_object);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
		
		glEnableVertexAttribArray(0); // position
		glEnableVertexAttribArray(1); // color
		glEnableVertexAttribArray(2); // texture

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (GLvoid *)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (GLvoid *)(7 * sizeof(float)));

		// Unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_isInitialized = true;
	};
};
