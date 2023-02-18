#pragma once

#include <vector>

#include "RigidBody.h"
#include "SoftBody.h"
#include "SphereShape.h"
#include "PlaneShape.h"
#include "AABBShape.h"

namespace physics
{
	struct CollidingBodies
	{
		CollidingBodies(iCollisionBody* a, iCollisionBody* b) : bodyA(a), bodyB(b) { }
		iCollisionBody* bodyA;
		iCollisionBody* bodyB;
	};

	class CollisionHandler
	{
	public:
		CollisionHandler();
		~CollisionHandler();

		// Helper functions
		glm::vec3 ClosestPtPointPlane(const glm::vec3& pt, const glm::vec3& planeNormal, float planeDotProduct);
		float SqDistPointAABB(Vector3 p, physics::AABBShape* b);

		// Shape Collisions
		bool CollideSphereSphere(float dt, RigidBody* bodyA, SphereShape* sphereA, RigidBody* bodyB, SphereShape* sphereB);
		bool CollideSpherePlane(float dt, RigidBody* sphere, SphereShape* sphereShape, RigidBody* plane, PlaneShape* planeShape);
		bool CollideSphereAABB(float dt, RigidBody* sphere, SphereShape* sphereA, RigidBody* bodyAABB, AABBShape* shapeAABBShape);

		//bool CollideSphereBox(float dt, RigidBody* sphere, SphereShape* sphereShape,
		//	RigidBody* plane, PlaneShape* planeShape);

		void Collide(float dt, std::vector<iCollisionBody*>& bodies, std::vector<CollidingBodies>& collisions);

		// Body Collisions
		bool CollideRigidRigid(float dt, RigidBody* rigidA, RigidBody* rigidB);
		bool CollideRigidSoft(float dt, RigidBody* rigidA, SoftBody* softB);
		bool CollideSoftSoft(float dt, SoftBody* softA, SoftBody* softB);
		void CollideInternalSoftBody(float dt, SoftBody* softBody);
	};
}
