#include "PhysicsWorld.h"
#include "Conversion.h"
#include "DebugDrawer.h"

namespace physics
{
	PhysicsWorld::PhysicsWorld()
		: iPhysicsWorld()
	{
		m_CollisionConfiguration = new btDefaultCollisionConfiguration();
		m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
		m_OverlappingPairCache = new btDbvtBroadphase();
		m_Solver = new btSequentialImpulseConstraintSolver;
		m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, 
			m_OverlappingPairCache, m_Solver, m_CollisionConfiguration);

		//m_DebugDrawer = new DebugDrawer(7);
		//m_DynamicsWorld->setDebugDrawer(m_DebugDrawer);
		//int debugMode = 1;
		//m_DynamicsWorld->getDebugDrawer()->setDebugMode(debugMode);
	}

	PhysicsWorld::~PhysicsWorld()
	{
		delete m_DynamicsWorld;
		delete m_CollisionConfiguration;
		delete m_Dispatcher;
		delete m_OverlappingPairCache;
		delete m_Solver;
	}

	void PhysicsWorld::SetGravity(const Vector3& gravity)
	{
		btVector3 btGravity;
		CastBulletVector3(gravity, &btGravity);
		m_DynamicsWorld->setGravity(btGravity);
	}

	void PhysicsWorld::AddBody(iCollisionBody* body)
	{
		btRigidBody* bulletBody = CastBulletRigidBody(body);
		m_DynamicsWorld->addRigidBody(bulletBody);
	}

	void PhysicsWorld::RemoveBody(iCollisionBody* body)
	{
		btRigidBody* bulletBody = CastBulletRigidBody(body);
		m_DynamicsWorld->removeRigidBody(bulletBody);
	}

	void PhysicsWorld::TimeStep(float dt)
	{
		m_DynamicsWorld->stepSimulation(dt);
	}

	void PhysicsWorld::DebugDraw()
	{
		m_DynamicsWorld->debugDrawWorld();
	}

	void PhysicsWorld::TransformRigidBodyPosition(iCollisionBody* body, const Vector3& position) {

		btRigidBody* bulletBody = CastBulletRigidBody(body);

		// Kill all forces
		bulletBody->clearForces();
		bulletBody->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
		bulletBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));

		// Prepares the Position transformation
		btTransform newTransform;
		newTransform.setIdentity();
		newTransform.setOrigin(btVector3(position.x, position.y, position.z));		
		// Set the new transformation for the rigid body
		bulletBody->setWorldTransform(newTransform);

		// Restore the rigid body's dynamics properties
		bulletBody->setCollisionFlags(bulletBody->getCollisionFlags() & ~btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
		bulletBody->setActivationState(ACTIVE_TAG);
		bulletBody->setLinearVelocity(btVector3(0, 0, 0));
		bulletBody->setAngularVelocity(btVector3(0, 0, 0));

	}
}
