#pragma once
#include "cProjectManager.h"

#include <SphereShape.h>
#include <PlaneShape.h>
#include <AABBShape.h>
#include <myMath.h>

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

extern cProjectManager* g_ProjectManager;
extern GLFWwindow* window;
extern glm::vec3* g_cameraEye;
extern glm::vec3* g_cameraTarget;
extern cMeshObject* g_actor;
extern glm::vec3 g_actorFacing;
extern FModManager* g_FModManager;
extern cMeshFactory* g_MeshFactory;

// Global Physics Factory
extern PhysicsFactoryType* g_PhysicsFactory;
// Global Physics World
extern physics::iPhysicsWorld* g_PhysicsWorld;
unsigned int yPosition = 10;

void PhysicsProjTwoStartingUp();
void PhysicsProjTwoNewGame();
void PhysicsProjTwoRunning();
void PhysicsProjTwoShutdown();

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
	g_PhysicsFactory = new PhysicsFactoryType();
	// Create Physics World
	g_PhysicsWorld = g_PhysicsFactory->CreateWorld();
	// Set Gravity
	g_PhysicsWorld->SetGravity(glm::vec3(0, -9.81, 0));

	// Create CollisionListener
	//collisionListener = physicsFactory->CreateCollisionListener();
	// Register it to the World
	//world->RegisterCollisionListener(collisionListener);

	// Create a Plane (PlaneShape)
	//cMeshObject* thePlane = g_ProjectManager->m_selectedScene->m_mMeshes.find("Plane")->second;
	//physics::iShape* planeShape = new physics::PlaneShape(0.0f, glm::vec3(0.f, 1.f, 0.f));
	//physics::RigidBodyDesc desc;
	//desc.isStatic = true;
	//desc.mass = 0;
	//desc.position = glm::vec3(0.f);
	//desc.linearVelocity = glm::vec3(0.f);
	//thePlane->physicsBody = g_PhysicsFactory->CreateRigidBody(desc, planeShape);
	//g_PhysicsWorld->AddBody(thePlane->physicsBody);

	// Create a AABB BoxPlane
	cMeshObject* thePlane = g_ProjectManager->m_selectedScene->m_mMeshes.find("Plane")->second;
	// Creates the AABB structure for the mesh
	float min[3] = { thePlane->m_parentModel->min_x,
					 thePlane->m_parentModel->min_y,
					 thePlane->m_parentModel->min_z };
	float max[3] = { thePlane->m_parentModel->max_x,
					 thePlane->m_parentModel->max_y,
					 thePlane->m_parentModel->max_z };
	glm::vec3 aabbHalfExtends = glm::vec3((max[0] - min[0]) / 2.0f,
										  (max[1] - min[1]) / 2.0f,
										  (max[2] - min[2]) / 2.0f);
	physics::iShape* planeShape = new physics::BoxShape(aabbHalfExtends);
	physics::RigidBodyDesc desc;
	desc.isStatic = true;
	desc.mass = 0;
	desc.position = thePlane->m_position;
	desc.linearVelocity = glm::vec3(0.f);
	thePlane->physicsBody = g_PhysicsFactory->CreateRigidBody(desc, planeShape);
	g_PhysicsWorld->AddBody(thePlane->physicsBody);

	// Create a Cube Wall
	// Wall Initial Position
	glm::vec3 wallPosition(-10.0f, 1.0f, 8.0f);
	for (int line = 0; line < 10; line++) {
		for (int column = 0; column < 6; column++) {
			cMeshObject* newCube = g_MeshFactory->createCubeMesh("Cube["+std::to_string(line)+","+std::to_string(column) + "]");
			newCube->m_position = wallPosition;
			wallPosition.x += 1.0f;
			newCube->m_bUse_RGBA_colour = true;
			newCube->m_RGBA_colour = glm::vec4(1.f, 0.f, 1.f, 1.f);
			// Creates the AABB structure for the mesh
			aabbHalfExtends = glm::vec3((newCube->m_parentModel->max_x - newCube->m_parentModel->min_x) / 2.0f,
										(newCube->m_parentModel->max_y - newCube->m_parentModel->min_y) / 2.0f,
										(newCube->m_parentModel->max_z - newCube->m_parentModel->min_z) / 2.0f);
			physics::iShape* boxShape = new physics::BoxShape(aabbHalfExtends);
			physics::RigidBodyDesc desc;
			desc.isStatic = false;
			desc.mass = 0.5;
			desc.position = newCube->m_position;
			desc.linearVelocity = glm::vec3(0.f);
			newCube->physicsBody = g_PhysicsFactory->CreateRigidBody(desc, boxShape);
			g_PhysicsWorld->AddBody(newCube->physicsBody);
		}
		wallPosition.x = -10.f;
		wallPosition.y += 1.5f;
	}

	// Sets main actor
	g_actor = g_MeshFactory->createSphereMesh("Player");
	g_actor->m_position = glm::vec3(0.0f, 5.0f, 0.0f);
	g_actor->m_scale = glm::vec3(1.0f);
	g_actor->m_bUse_RGBA_colour = true;
	g_actor->m_RGBA_colour = glm::vec4(0.f, 1.f, 0.f, 1.f);
	physics::iShape* ballShape = new physics::SphereShape(g_actor->m_scale.x);
	desc.isStatic = false;
	desc.mass = g_actor->m_scale.x;
	desc.position = g_actor->m_position;
	desc.linearVelocity = glm::vec3(0.f);
	g_actor->physicsBody = g_PhysicsFactory->CreateRigidBody(desc, ballShape);
	g_PhysicsWorld->AddBody(g_actor->physicsBody);

	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void PhysicsProjTwoNewGame() {
	g_ProjectManager->m_GameLoopState = GameState::RUNNING;

}

void PhysicsProjTwoRunning() {
	// *********
	// MAIN LOOP
	// *********	
	g_PhysicsWorld->TimeStep(0.1f);
	// Step to update meshs position on the screen based on the physics
	g_ProjectManager->Step();
	
	//while (collisionListener->AccountForCollision()) {
	//	// FMOD sounds for Sphere Sphere Collision
	//	g_FModManager->playSound("Hit", "ch2 fx");
	//}
}

void PhysicsProjTwoShutdown() {
	// Closing the Application
	glfwSetWindowShouldClose(window, true);
	// Deletes thigs
	delete g_PhysicsFactory;
	delete g_PhysicsWorld;
}