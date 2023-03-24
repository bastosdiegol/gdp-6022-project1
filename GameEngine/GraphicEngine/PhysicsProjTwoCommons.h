#pragma once
#include "cProjectManager.h"

#include <SphereShape.h>
#include <PlaneShape.h>
#include <AABBShape.h>
#include <CylinderShape.h>
#include <myMath.h>

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

#define MIN_SCALE 0.5f
#define MAX_SCALE 5.0f

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
unsigned int g_numOfSpheres = 0;

void PhysicsProjTwoStartingUp();
void PhysicsProjTwoNewGame();
void PhysicsProjTwoRunning();
void PhysicsProjTwoShutdown();
float RandFloat(float min, float max);
/// <summary>
/// Creates a Mesh Sphere with Random Size
/// And adds it to the PhysicsWorld
/// </summary>
void GenerateSphere();

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
	// Calculates the Half Extents
	glm::vec3 aabbHalfExtents = glm::vec3((thePlane->m_parentModel->max_x - thePlane->m_parentModel->min_x) * thePlane->m_scale.x / 2.0f,
										  (thePlane->m_parentModel->max_y - thePlane->m_parentModel->min_y) * thePlane->m_scale.y / 2.0f,
										  (thePlane->m_parentModel->max_z - thePlane->m_parentModel->min_z) * thePlane->m_scale.z / 2.0f);
	// Creates the Shape and Description
	physics::iShape* planeShape = new physics::BoxShape(aabbHalfExtents);
	physics::RigidBodyDesc desc;
	desc.isStatic = true;
	desc.mass = 0;
	desc.position = thePlane->m_position;
	desc.linearVelocity = glm::vec3(0.f);
	// Creates the RigidBody
	thePlane->physicsBody = g_PhysicsFactory->CreateRigidBody(desc, planeShape);
	// Adds the RigidBody to the World
	g_PhysicsWorld->AddBody(thePlane->physicsBody);

	// Create a Cube Wall
	// Wall Initial Position
	glm::vec3 wallPosition(-13.0f, 1.0f, 8.0f);
	for (int line = 0; line < 6; line++) {
		for (int column = 0; column < 10; column++) {
			// Creates the Mesh
			cMeshObject* newCube = g_MeshFactory->createCubeMesh("Cube["+std::to_string(line)+","+std::to_string(column) + "]");
			newCube->m_position = wallPosition;
			newCube->defineInitialPosition();
			wallPosition.x += 1.0f;
			newCube->m_bUse_RGBA_colour = true;
			newCube->m_RGBA_colour = glm::vec4(1.f, 0.f, 1.f, 1.f);
			// Calculates the Half Extents
			aabbHalfExtents = glm::vec3((newCube->m_parentModel->max_x - newCube->m_parentModel->min_x) * newCube->m_scale.x / 2.0f,
										(newCube->m_parentModel->max_y - newCube->m_parentModel->min_y) * newCube->m_scale.y / 2.0f,
										(newCube->m_parentModel->max_z - newCube->m_parentModel->min_z) * newCube->m_scale.z / 2.0f);
			// Creates the Shape and Description
			physics::iShape* boxShape = new physics::BoxShape(aabbHalfExtents);
			desc.isStatic = false;
			desc.mass = 0.5;
			desc.position = newCube->m_position;
			desc.linearVelocity = glm::vec3(0.f);
			// Creates the RigidBody
			newCube->physicsBody = g_PhysicsFactory->CreateRigidBody(desc, boxShape);
			// Adds the RigidBody to the World
			g_PhysicsWorld->AddBody(newCube->physicsBody);
		}
		wallPosition.x = -13.f;
		wallPosition.y += 1.5f;
	}

	// Create a couple of static boxes
	int numOfStaticBoxes = 8;
	for (int i = 0; i < numOfStaticBoxes; i++) {
		// Creates the Mesh
		cMeshObject* newCube = g_MeshFactory->createCubeMesh("StaticBox[" + std::to_string(i) + "]");
		newCube->m_scale = glm::vec3(RandFloat(MIN_SCALE, MAX_SCALE));
		newCube->m_position = glm::vec3(RandFloat(-16.0f + newCube->m_scale.x, 0.0f), 
										0.5f, 
										RandFloat(-16.0f + newCube->m_scale.z, 0.0f));
		newCube->defineInitialPosition();
		newCube->m_bUse_RGBA_colour = true;
		newCube->m_RGBA_colour = glm::vec4(0.f, 0.f, 0.5f, 1.f);
		// Calculates the Half Extents
		aabbHalfExtents = glm::vec3((newCube->m_parentModel->max_x - newCube->m_parentModel->min_x) * newCube->m_scale.x / 2.0f,
			                        (newCube->m_parentModel->max_y - newCube->m_parentModel->min_y) * newCube->m_scale.y / 2.0f,
			                        (newCube->m_parentModel->max_z - newCube->m_parentModel->min_z) * newCube->m_scale.z / 2.0f);
		// Creates the Shape and Description
		physics::iShape* boxShape = new physics::BoxShape(aabbHalfExtents);
		desc.isStatic = true;
		desc.mass = 0;
		desc.position = newCube->m_position;
		desc.linearVelocity = glm::vec3(0.f);
		// Creates the RigidBody
		newCube->physicsBody = g_PhysicsFactory->CreateRigidBody(desc, boxShape);
		// Adds the RigidBody to the World
		g_PhysicsWorld->AddBody(newCube->physicsBody);
	}

	// Creates cylinders to act as a bowling pins
	// Number of rows for disposition of pins
	int numRows = 4;
	// Pin Initial Position
	glm::vec3 pinPosition(5.0f, 1.0f, -8.0f);
	// Default spacing between pins
	float spacing = 2.0f;
	// Loop through each row and column and create each pin
	for (int row = 0; row < numRows; row++) {

		// Adjust the starting position of the pin in each row
		int numPinsInRow = row + 1;
		// Adjust the starting position of the pin in each row
		glm::vec3 rowStartPosition = pinPosition - glm::vec3((numPinsInRow - 1) * spacing / 2.0f, 0.f, 0.f) + glm::vec3(row * spacing / 2.0f, 0.f, 0.f);
		for (int column = 0; column < numPinsInRow; column++) {
			// Creates the Mesh
			cMeshObject* newCylinder = g_MeshFactory->createCylinderMesh("Pin[" + std::to_string(row) + "," + std::to_string(column) + "]");
			newCylinder->m_position = rowStartPosition + glm::vec3(column * spacing, 0.f, row * spacing);
			newCylinder->defineInitialPosition();
			newCylinder->m_bUse_RGBA_colour = true;
			newCylinder->m_RGBA_colour = glm::vec4(0.6f, 0.4f, 0.2f, 1.f);
			// Calculates the Half Extents
			aabbHalfExtents = glm::vec3((newCylinder->m_parentModel->max_x - newCylinder->m_parentModel->min_x) * newCylinder->m_scale.x / 2.0f,
										(newCylinder->m_parentModel->max_y - newCylinder->m_parentModel->min_y) * newCylinder->m_scale.y / 2.0f,
										(newCylinder->m_parentModel->max_z - newCylinder->m_parentModel->min_z) * newCylinder->m_scale.z / 2.0f);
			// Creates the Shape and Description
			physics::iShape* boxShape = new physics::CylinderShape(aabbHalfExtents);
			desc.isStatic = false;
			desc.mass = 0.5;
			desc.position = newCylinder->m_position;
			desc.linearVelocity = glm::vec3(0.f);
			// Creates the RigidBody
			newCylinder->physicsBody = g_PhysicsFactory->CreateRigidBody(desc, boxShape);
			// Adds the RigidBody to the World
			g_PhysicsWorld->AddBody(newCylinder->physicsBody);
		}
	}

	// Sets main actor
	g_actor = g_MeshFactory->createSphereMesh("Player");
	g_actor->m_position = glm::vec3(0.0f, 5.0f, 0.0f);
	g_actor->defineInitialPosition();
	g_actor->m_scale = glm::vec3(1.0f);
	g_actor->m_bUse_RGBA_colour = true;
	g_actor->m_RGBA_colour = glm::vec4(0.f, 1.f, 0.f, 1.f);
	// Creates the Shape and Description
	physics::iShape* ballShape = new physics::SphereShape(g_actor->m_scale.x);
	desc.isStatic = false;
	desc.mass = g_actor->m_scale.x;
	desc.position = g_actor->m_position;
	desc.linearVelocity = glm::vec3(0.f);
	// Creates the RigidBody
	g_actor->physicsBody = g_PhysicsFactory->CreateRigidBody(desc, ballShape);
	// Adds the RigidBody to the World
	g_PhysicsWorld->AddBody(g_actor->physicsBody);

	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
}

void PhysicsProjTwoNewGame() {
	g_ProjectManager->m_GameLoopState = GameState::RUNNING;
	std::map<std::string, cMeshObject*>::iterator itMeshes;
	itMeshes = g_ProjectManager->m_selectedScene->m_mMeshes.begin();
	// Iterates through all meshes
	while (itMeshes != g_ProjectManager->m_selectedScene->m_mMeshes.end()) {
		cMeshObject* pCurrentMeshObject = itMeshes->second;
		++itMeshes;
		// If its not the plane
		if (pCurrentMeshObject->m_meshName != "Plane") {
			// Checks if the Object is a Generated Sphere
			if (pCurrentMeshObject->m_meshName.find("Sphere") == 0) {
				// TODO: FIX the Deletion
				//if (!pCurrentMeshObject->physicsBody) {
				//	g_PhysicsWorld->RemoveBody(pCurrentMeshObject->physicsBody);
				//	delete pCurrentMeshObject->physicsBody;
				//}
				//g_ProjectManager->m_selectedScene->m_mMeshes.erase(pCurrentMeshObject->m_meshName);
				//delete pCurrentMeshObject;
			}
			// Transform the RigidBody Position to Initial Position
			g_PhysicsWorld->TransformRigidBodyPosition(pCurrentMeshObject->physicsBody,
				pCurrentMeshObject->m_InitialPosition);
		}
	}

	g_numOfSpheres = 0;
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

// Utility function for a random range of two floats
float RandFloat(float min, float max) {
	//DEBUG_PRINT("RandFloat(%f, %f)\n", min, max);
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}

void GenerateSphere() {
	// Creates the Mesh
	cMeshObject* newSphere = g_MeshFactory->createSphereMesh("Sphere[" + std::to_string(g_numOfSpheres) + "]");
	g_numOfSpheres++;
	newSphere->m_scale = glm::vec3(RandFloat(MIN_SCALE, MAX_SCALE));
	newSphere->m_position = glm::vec3(RandFloat(16.0f - newSphere->m_scale.x, 0.0f),
									  0.5f,
									  RandFloat(16.0f - newSphere->m_scale.z, 0.0f));
	newSphere->defineInitialPosition();
	newSphere->m_bUse_RGBA_colour = true;
	newSphere->m_RGBA_colour = glm::vec4(0.2f, 0.4f, 0.6f, 1.f);
	// Creates the Shape and Description
	physics::iShape* ballShape = new physics::SphereShape(newSphere->m_scale.x * 0.5);
	physics::RigidBodyDesc desc;
	desc.isStatic = false;
	desc.mass = newSphere->m_scale.x;
	desc.position = newSphere->m_position;
	desc.linearVelocity = glm::vec3(0.f);
	// Creates the RigidBody
	newSphere->physicsBody = g_PhysicsFactory->CreateRigidBody(desc, ballShape);
	// Adds the RigidBody to the World
	g_PhysicsWorld->AddBody(newSphere->physicsBody);
}