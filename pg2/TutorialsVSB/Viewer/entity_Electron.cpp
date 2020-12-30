#include "entity_Particle.h"
#include "entity_Electron.h"

inline void Entity_Electron::applyTransforms() {
	glm::mat4 model = glm::mat4();

	model = glm::translate(model, origin);
	glm::quat q = glm::angleAxis(angle, rotationAxis);
	model *= glm::mat4_cast(q);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(scale));

	m_modelMatrix = model;
}

void Entity_Electron::update() {
	angle += velocity;
	applyTransforms();

	// spawn new particle each iteration until all 100 are spawned
	if (particleEntities.size() < particleCount) {
		Entity_Particle *p = new Entity_Particle(this, vaoParticle);
		p->init();
		particleEntities.push_back(p);
	}

	// update particles
	for (int i = 0; i < particleEntities.size(); i++) {
		Entity_Particle *p = (Entity_Particle *)particleEntities[i];

		if (p->life < 0) {
			p->respawn(this);
		}

		p->update();
	}
}

Entity_Electron::~Entity_Electron() {
	for (unsigned int i = 0; i < particleEntities.size(); i++)
		SAFE_DELETE(particleEntities[i]);
}