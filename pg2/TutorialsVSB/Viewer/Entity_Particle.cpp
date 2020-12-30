#pragma once

#include "Entity_Particle.h"
#include <random>
#include <ctime>
#include "entity_Electron.h"

glm::vec3 Entity_Particle::colors[colorsCount] = {
	glm::vec3(0.8f, 0.1, 0.2),
	glm::vec3(0.87f, 0.13f, 0.16f),
	glm::vec3(1.0f, 0.2f, 0.16f),
	glm::vec3(0.8f, 0.1, 0.2),
	glm::vec3(0.86f, 0.29f, 0.22f),
	glm::vec3(0.94f, 0.5f, 0.5f),
	glm::vec3(0.8f, 0.1, 0.2)
};

Entity_Particle::Entity_Particle(Entity *electron, VAO* vao) : Entity_VAO(vao) {
	std::srand(std::time(nullptr));
	respawn(electron);
}

float Entity_Particle::rand(float min, float max) {
	static std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(min, max);

	return distribution(generator);
}

inline void Entity_Particle::draw(const unsigned int eid) {
	if (!m_isInitialized) return;

	glBindVertexArray(m_vao->m_object);
	glDrawArrays(GL_POINTS, 0, 1);
	glPointSize(size);
	glBindVertexArray(0);
}

inline void Entity_Particle::applyTransforms() {
	glm::mat4 model = glm::mat4();

	model = glm::translate(model, origin);
	glm::quat q = glm::angleAxis(angle, rotationAxis);
	model *= glm::mat4_cast(q);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(scale));

	m_modelMatrix = model;
}

void Entity_Particle::update() {
	life -= 0.005f;
	velocity -= 0.000001f;
	size *= 0.999f;
	angle += velocity;
	applyTransforms();
}

inline void Entity_Particle::respawn(Entity *electron) {
	Entity_Electron *e = (Entity_Electron*)electron;

	position.x = e->position.x - rand(-0.15f, 0.15f);
	position.y = e->position.y - rand(-0.15f, 0.15f);
	position.z = e->position.z - rand(-0.15f, 0.15f);
	origin = e->origin;
	rotationAxis = e->rotationAxis;
	scale = rand(0.10f, 0.15f);
	life = rand(0.95f, 1.0f);
	size = rand(1.0f, 5.0f);
	angle = e->angle - rand(0.01f, 0.1f);
	velocity = e->velocity * 0.8f;
	color = colors[static_cast<int>(rand(0.0f, colorsCount)) % colorsCount];
	applyTransforms();
}