#include "cEnemyTypeC.h"
#include <glm/geometric.hpp>

cEnemyTypeC::~cEnemyTypeC() {
}

void cEnemyTypeC::performBehaviour() {
	this->Approaches(5.0f);
}

void cEnemyTypeC::Approaches(const float minDistance) {
	// Vector Towards the Player
	glm::vec3 towardsTarget = glm::normalize(this->targetPos - this->position);
	// Checks the Distance between the Enemy and the Player
	if (glm::distance(this->position, this->targetPos) > minDistance) {
		// Approaches the Player
		this->position += towardsTarget * MOVE_SPEED;
	}
}
