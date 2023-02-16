#include "cEnemyTypeA.h"
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

cEnemyTypeA::~cEnemyTypeA() {
}

void cEnemyTypeA::performBehaviour() {
	// Grabs the vector towards the Enemy/Agent
	glm::vec3 towardsAgent = glm::normalize(this->position - this->targetPos);
	// Dot product between vector towards the Enemy and the Player Facing vector
	float dot = glm::dot(targetFacing, towardsAgent);
	// If the Enemy is inside the Player 45º vision field
	if (dot <= cos(glm::radians(45.f))) {
		// Player can't see the Enemy
		// Enemy seeks the Player
		this->Seek();
	} else {
		// Player can see the Enemy
		// Enemy flees from the Player
		this->Flee();
	}
}

void cEnemyTypeA::Seek() {
	// Vector Towards the Player
	glm::vec3 towardsTarget = glm::normalize(this->targetPos - this->position);
	// Moves the Enemy Towards the Player
	this->position += towardsTarget * MOVE_SPEED;
}

void cEnemyTypeA::Flee() {
	// Vector Towards the Enemy
	glm::vec3 awayFromTarget = glm::normalize(this->position - this->targetPos);
	// Moves the Enemy Always from the Player
	this->position += awayFromTarget * MOVE_SPEED;
}
