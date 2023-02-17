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

physics::iPhysicsFactory* physicsFactory;
physics::iPhysicsWorld* world;

void PhysicsProjOneStartingUp();
void PhysicsProjOneNewGame();
void PhysicsProjOneRunning();
void PhysicsProjOneShutdown();

iEnemy* enemyA = nullptr;
iEnemy* enemyB = nullptr;
iEnemy* enemyC = nullptr;

void PhysicsProjOneGameLoop() {
	switch (g_ProjectManager->m_GameLoopState) {
	case GameState::STARTING_UP:
		PhysicsProjOneStartingUp();
		break;
	case GameState::NEW_GAME:
		PhysicsProjOneNewGame();
		break;
	case GameState::RUNNING:
		PhysicsProjOneRunning();
		break;
	case GameState::SHUTING_DOWN:
		PhysicsProjOneShutdown();
		break;
	}
}

void PhysicsProjOneStartingUp() {
	// Initialize a Physics Factory
	physicsFactory = new physics::PhysicsFactory();

	// Create Physics World
	world = physicsFactory->CreateWorld();
	world->SetGravity(Vector3(0.0f, -0.98f, 0.0f));

	// Sets main actor
	g_actor = g_ProjectManager->m_selectedScene->m_mMeshes.find("Triangle-Actor")->second;
	// Adjust main actor facing direction
	g_actorFacing.x = sin(g_actor->m_rotation.y);
	g_actorFacing.z = cos(g_actor->m_rotation.y);

	cMeshObject* newPhysicsBall1 = g_ProjectManager->m_selectedScene->m_mMeshes.find("Ball1")->second;
	// Create a ball 
	physics::RigidBodyDesc ballDesc;
	ballDesc.isStatic = false;
	ballDesc.mass = 10.f;
	ballDesc.position = newPhysicsBall1->m_position;
	ballDesc.angularVelocity = Vector3(10.f, 0.f, 0.f);

	physics::iShape* ballShape = new physics::SphereShape(1.0f);
	
	newPhysicsBall1->physicsBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);
	world->AddBody(newPhysicsBall1->physicsBody);

	// Create a plane
	physics::RigidBodyDesc planeDesc;
	planeDesc.isStatic = true;
	planeDesc.position = Vector3(0.0f);
	physics::iShape* planeShape = new physics::PlaneShape(10.0f, Vector3(1.0f));
	world->AddBody(physicsFactory->CreateRigidBody(planeDesc, planeShape));

	// Gets the First enemy Mesh
	//cMeshObject* pObjEnemy = g_ProjectManager->m_selectedScene->m_mMeshes.find("Ball1")->second;
	// Instantiate the Enemy Red Ball - Seeks and Flees
	//enemyA = new cEnemyTypeA(pObjEnemy->m_position, pObjEnemy->m_rotation, g_actor->m_position, g_actor->m_rotation, g_actorFacing);
	// Gets the Second enemy Mesh
	cMeshObject* pObjEnemy = g_ProjectManager->m_selectedScene->m_mMeshes.find("Ball2")->second;
	// Instantiate the Enemy Green Ball - Pursues and Evades
	enemyB = new cEnemyTypeB(pObjEnemy->m_position, pObjEnemy->m_rotation, g_actor->m_position, g_actor->m_rotation, g_actorFacing);
	// Gets the First enemy Mesh
	pObjEnemy = g_ProjectManager->m_selectedScene->m_mMeshes.find("Ball3")->second;
	// Instantiate the Blue Ball - Approaches
	enemyC = new cEnemyTypeC(pObjEnemy->m_position, pObjEnemy->m_rotation, g_actor->m_position, g_actor->m_rotation, g_actorFacing);
	// Sets the game state to running
	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void PhysicsProjOneNewGame() {
	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void PhysicsProjOneRunning() {
	// *********
	// MAIN LOOP
	// *********
	// Applies each enemy behavior
	//enemyA->performBehaviour();
	//enemyB->performBehaviour();
	//enemyC->performBehaviour();
	
	world->TimeStep(0.1f);
	g_ProjectManager->Step();

	// FMOD SOUNDS !
	//g_FModManager->playSound("Hit", "ch2 fx");
}

void PhysicsProjOneShutdown() {
	// Closing the Application
	glfwSetWindowShouldClose(window, true);
	// Deletes thigs
	delete enemyA;
	delete enemyB;
	delete enemyC;
	delete physicsFactory;
	delete world;
}