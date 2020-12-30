#pragma once

#include "vao.h"

class VAO_Vertex : public VAO
{
public:
	VAO_Vertex() : VAO() {}
	virtual ~VAO_Vertex() {}

	virtual void init() {
		float tmpVertices[3] = { 0.0f, 0.0f, 0.0f };

		GLuint tmpBuffer;
		glGenBuffers(1, &tmpBuffer);
		initBuffer(tmpBuffer, tmpVertices, 3 * sizeof(float), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_buffers.push_back(tmpBuffer);

		glGenVertexArrays(1, &m_object);
		glBindVertexArray(m_object);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);

		glEnableVertexAttribArray(0); // position

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		// Unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_isInitialized = true;
	};
};
