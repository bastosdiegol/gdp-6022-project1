#pragma once

#include <ShapeType.h>
#include <iShape.h>
#include <iRigidBody.h>
#include <iSoftBody.h>
#include <iPhysicsWorld.h>
#include <iPhysicsFactory.h>
#include <SphereShape.h>
#include <PlaneShape.h>
#include <BoxShape.h>

#define Bullet_Physics

#if defined GDP_Physics
#pragma comment(lib, "GDPPhysics.lib")
#include <physics\gdp\PhysicsFactory.h>
#elif defined Bullet_Physics
#pragma comment(lib, "BulletSource.lib")
#pragma comment(lib, "BulletPhysics.lib")
#include <PhysicsFactory.h>

#elif defined PhysX_Physics
#pragma comment(lib, "PhysXPhysics.lib")
#pragma comment(lib, "PhysXSource.lib")
#include <physics\physx\PhysicsFactory.h>
#endif

typedef physics::PhysicsFactory PhysicsFactoryType;