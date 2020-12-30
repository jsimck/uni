#pragma once

#include "entity_VAO.h"

class Entity_Particle : public Entity_VAO {
private:
	inline void applyTransforms();
public:
	static float rand(float min, float max);
	static const unsigned int colorsCount = 7;
	static glm::vec3 colors[colorsCount];

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotationAxis;
	glm::vec3 color;

	float scale;
	float angle;
	float velocity;
	float life;
	float size;

	Entity_Particle(Entity *electron, VAO* vao = nullptr);
	virtual ~Entity_Particle() {}

	inline void respawn(Entity *electron);
	inline void draw(const unsigned int eid = 0);
	virtual void update() override;
};