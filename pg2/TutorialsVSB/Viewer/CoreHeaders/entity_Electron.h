#pragma once

#include <dynArray.h>
#include <iostream>
#include "vao.h"
#include "entity_OBJ.h"

class Entity_Electron : public Entity_OBJ {
private:
	VAO *vaoParticle;
	inline void applyTransforms();
public:
	const int particleCount = 900;
	DynArray<Entity*> particleEntities;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotationAxis;

	float scale;
	float angle;
	float velocity;

	Entity_Electron(Model* model, VAO* vao, VAO* vaoParticle) : Entity_OBJ(model, vao), 
		particleEntities(DynArray<Entity*>(particleCount)), vaoParticle(vaoParticle) {}
	virtual ~Entity_Electron();

	virtual void update() override;
};