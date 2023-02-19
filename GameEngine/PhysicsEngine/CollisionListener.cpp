#include "CollisionListener.h"

CollisionListener::CollisionListener() {
}

CollisionListener::~CollisionListener() {
}

void CollisionListener::NotifyCollision(physics::iCollisionBody* bodyA, physics::iCollisionBody* bodyB) {
	sCollisionNotification newNotification;
	newNotification.bodyA = bodyA;
	newNotification.bodyB = bodyB;
	this->m_CollisionArray.push_back(newNotification);
}
