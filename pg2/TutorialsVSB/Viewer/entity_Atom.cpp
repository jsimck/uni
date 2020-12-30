#include "entity_Atom.h"

void Entity_Atom::update() {
	textureShift += 0.001f;

	if (pulseDir) {
		if (pulse + 0.025f > 1) {
			pulseDir = !pulseDir;
		}
		else {
			pulse += 0.025f;
		}
	}
	else {
		if (pulse - 0.005f < 0.5f) {
			pulseDir = !pulseDir;
		}
		else {
			pulse -= 0.005f;
		}
	}
}