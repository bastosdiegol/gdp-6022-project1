#pragma once
#include "cProjectManager.h"
#include "cEnemyTypeA.h"
#include "cEnemyTypeB.h"
#include "cEnemyTypeC.h"
#include <PhysicsFactory.h>
#include <iPhysicsFactory.h>
#include <PhysicsWorld.h>

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

extern cProjectManager* g_ProjectManager;
extern PhysicsSystem* g_PhysicsSystem;
extern GLFWwindow* window;
extern glm::vec3* g_cameraEye;
extern glm::vec3* g_cameraTarget;
extern cMeshObject* g_actor;
extern glm::vec3 g_actorFacing;
extern FModManager* g_FModManager;
extern cMeshFactory* g_MeshFactory;

physics::iPhysicsFactory* physicsFactory;
physics::iPhysicsWorld* world;
physics::iCollisionListener* collisionListener;

void PhysicsProjTwoStartingUp();
void PhysicsProjTwoNewGame();
void PhysicsProjTwoRunning();
void PhysicsProjTwoShutdown();

physics::RigidBodyDesc createRigidBodyDesc(bool isStatic, float mass, Vector3 position, Vector3 linearVelocity);
void setMeshObjectAsStaticPhysObjectAABB(std::string name);
void setMeshObjectAsPhysObjectAABB(cMeshObject* obj);

void PhysicsProjTwoGameLoop() {
	switch (g_ProjectManager->m_GameLoopState) {
	case GameState::STARTING_UP:
		PhysicsProjTwoStartingUp();
		break;
	case GameState::NEW_GAME:
		PhysicsProjTwoNewGame();
		break;
	case GameState::RUNNING:
		PhysicsProjTwoRunning();
		break;
	case GameState::SHUTING_DOWN:
		PhysicsProjTwoShutdown();
		break;
	}
}

void PhysicsProjTwoStartingUp() {
	// Initialize the Mesh Facoty
	g_MeshFactory = new cMeshFactory(g_ProjectManager->m_VAOManager, g_ProjectManager->m_selectedScene->m_mMeshes);

	// Initialize a Physics Factory
	physicsFactory = new physics::PhysicsFactory();

	// Create Physics World
	world = physicsFactory->CreateWorld();
	world->SetGravity(Vector3(0.0f, -1.f, 0.0f));

	// Create CollisionListener
	collisionListener = physicsFactory->CreateCollisionListener();
	// Register it to the World
	world->RegisterCollisionListener(collisionListener);

	// Sets main actor
	g_actor = g_ProjectManager->m_selectedScene->m_mMeshes.find("Player")->second;
	// Adjust main actor facing direction
	g_actorFacing.x = sin(g_actor->m_rotation.y);
	g_actorFacing.z = cos(g_actor->m_rotation.y);
	physics::iShape* playerBallShape = new physics::SphereShape(2.0f);
	physics::RigidBodyDesc PlayerDesc = createRigidBodyDesc(false, 2.f, g_actor->m_position, glm::vec3(0.f));
	g_actor->physicsBody = physicsFactory->CreateRigidBody(PlayerDesc, playerBallShape);
	world->AddBody(g_actor->physicsBody);

	// Creates all 5 Balls
	for (int i = 1; i <= 5; i++) {
		cMeshObject* newPhysicsBall = g_ProjectManager->m_selectedScene->m_mMeshes.find("Ball" + std::to_string(i))->second;
		// Create a ball 
		physics::iShape* ballShape = new physics::SphereShape(newPhysicsBall->m_scale.x);
		physics::RigidBodyDesc ballDesc = createRigidBodyDesc(false, newPhysicsBall->m_scale.x, newPhysicsBall->m_position, glm::vec3(0.f));
		newPhysicsBall->physicsBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);
		world->AddBody(newPhysicsBall->physicsBody);
	}

	// Create a AABB Plane
	setMeshObjectAsStaticPhysObjectAABB("Plane");

	// Create a AABB Cube
	cMeshObject* newCube = g_MeshFactory->createCubeMesh("Cube1");
	newCube->m_position = glm::vec3(18.f, 20.f, 5.f);
	newCube->m_bUse_RGBA_colour = true;
	newCube->m_RGBA_colour = glm::vec4(1.f, 0.f, 1.f, 1.f);
	//setMeshObjectAsPhysObjectAABB(newCube);

	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void PhysicsProjTwoNewGame() {
	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void PhysicsProjTwoRunning() {
	// *********
	// MAIN LOOP
	// *********	
	world->TimeStep(0.1f);
	g_ProjectManager->Step();

	while (collisionListener->AccountForCollision()) {
		// FMOD sounds for Sphere Sphere Collision
		g_FModManager->playSound("Hit", "ch2 fx");
	}

}

void PhysicsProjTwoShutdown() {
	// Closing the Application
	glfwSetWindowShouldClose(window, true);
	// Deletes thigs
	delete physicsFactory;
	delete world;
}

// Creates a Description for a Rigid Body
physics::RigidBodyDesc createRigidBodyDesc(bool isStatic, float mass, Vector3 position, Vector3 linearVelocity) {
	physics::RigidBodyDesc desc;
	desc.isStatic = isStatic;
	desc.mass = mass;
	desc.position = position;
	desc.linearVelocity = linearVelocity;
	return desc;
}

// Creates a Description for a AABB and adds it to the World
void setMeshObjectAsStaticPhysObjectAABB(std::string name) {
	cMeshObject* theMesh;
	// Links the MeshObject to the AABB PhysicsObject
	theMesh = g_ProjectManager->m_selectedScene->m_mMeshes.find(name)->second;
	// Creates the AABB structure for the mesh
	float min[3] = { theMesh->m_parentModel->min_x,
					 theMesh->m_parentModel->min_y,
					 theMesh->m_parentModel->min_z };
	float max[3] = { theMesh->m_parentModel->max_x,
					 theMesh->m_parentModel->max_y,
					 theMesh->m_parentModel->max_z };
	physics::iShape* theAABBShape = new physics::AABBShape(min, max, Vector3(0.f, 1.f, 0.f));

	// Adds the AABB to the Physics World
	physics::RigidBodyDesc AABBDesc = createRigidBodyDesc(true, 1.f, theMesh->m_position, glm::vec3(0.f));
	world->AddBody(physicsFactory->CreateRigidBody(AABBDesc, theAABBShape));
}

// Creates a Description for a AABB and adds it to the World
void setMeshObjectAsPhysObjectAABB(cMeshObject* obj) {
	// Creates the AABB structure for the mesh
	float min[3] = { obj->m_parentModel->min_x,
					 obj->m_parentModel->min_y,
					 obj->m_parentModel->min_z };
	float max[3] = { obj->m_parentModel->max_x,
					 obj->m_parentModel->max_y,
					 obj->m_parentModel->max_z };
	physics::iShape* theAABBShape = new physics::AABBShape(min, max, Vector3(0.f, 1.f, 0.f));

	// Adds the AABB to the Physics World
	physics::RigidBodyDesc AABBDesc = createRigidBodyDesc(false, obj->m_scale.x, obj->m_position, glm::vec3(0.f));
	obj->physicsBody = physicsFactory->CreateRigidBody(AABBDesc, theAABBShape);
	world->AddBody(obj->physicsBody);
}