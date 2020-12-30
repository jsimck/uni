#pragma once

#include "vao.h"
#include "entity_OBJ.h"

class Entity_Atom : public Entity_OBJ {
private:
	bool pulseDir;
public:
	float textureShift;
	float pulse;

	Entity_Atom(Model* model, VAO* vao) : Entity_OBJ(model, vao), pulse(1.0f), textureShift(0.0f), pulseDir(false) {}
	virtual ~Entity_Atom() {}

	virtual void update() override;
};