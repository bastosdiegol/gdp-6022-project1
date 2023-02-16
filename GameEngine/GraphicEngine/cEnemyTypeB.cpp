#include "cEnemyTypeB.h"
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

cEnemyTypeB::~cEnemyTypeB() {
}

void cEnemyTypeB::performBehaviour() {
	const float AHEAD_DISTANCE = 3.0f;
	// Grabs the vector towards the Enemy/Agent
	glm::vec3 towardsAgent = glm::normalize(this->position - this->targetPos);
	// Dot product between vector towards the Enemy and the Player Facing vector
	float dot = glm::dot(targetFacing, towardsAgent);


	if (dot < cos(glm::radians(30.0f))) {
		// Player can't see the Enemy
		// Enemy pursues the Player
		this->Pursue(AHEAD_DISTANCE);
	} else {
		// Player can see the Enemy
		// Enemy evades from the Player
		this->Evade(AHEAD_DISTANCE);
	}
}

void cEnemyTypeB::Pursue(const float aheadDistance) {
	// Grabs the target position
	glm::vec3 targetPosition = targetPos;
	// Calculates the direction ahead the Player
	glm::vec3 lookAhead = glm::normalize(targetFacing) * aheadDistance;
	// Simulates the new position
	targetPosition += lookAhead;
	
	// Vector Towards the new position of the Player
	glm::vec3 towardsTarget = glm::normalize(targetPosition - this->position);
	// Pursue the Player new position
	this->position += towardsTarget * MOVE_SPEED;
}

void cEnemyTypeB::Evade(const float aheadDistance) {
	// Grabs the target position
	glm::vec3 targetPosition = targetPos;
	// Calculates the direction ahead the Player
	glm::vec3 lookAhead = glm::normalize(targetFacing) * aheadDistance;
	// Simulates the new position
	targetPosition += lookAhead;

	// Vector Towards the Enemy Based on the new Position of the Player
	glm::vec3 awayFromTarget = glm::normalize(this->position - targetPosition);
	// Evades the Player new Position
	this->position += awayFromTarget * MOVE_SPEED;

}
