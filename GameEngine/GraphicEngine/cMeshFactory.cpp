#include "cMeshFactory.h"

cMeshFactory::cMeshFactory(cVAOManager* vaoManager, std::map<std::string, cMeshObject*>& meshArray)
	: m_sceneMeshes(meshArray) {

	this->m_cube = vaoManager->PrepareNewModel("cube1x1", "assets/mesh_factory/cube1x1.ply");
	vaoManager->LoadModelIntoVAO(this->m_cube);

	this->m_sphere = vaoManager->PrepareNewModel("ball1d", "assets/mesh_factory/ball1d.ply");
	vaoManager->LoadModelIntoVAO(this->m_sphere);
}

cMeshFactory::~cMeshFactory() {
}

cMeshObject* cMeshFactory::createCubeMesh(std::string name) {
	cMeshObject* newCube = new cMeshObject(m_cube);
	newCube->m_meshName = name;
	this->m_sceneMeshes.try_emplace(name, newCube);
	return newCube;
}

cMeshObject* cMeshFactory::createSphereMesh(std::string name) {
	cMeshObject* newSphere = new cMeshObject(m_sphere);
	newSphere->m_meshName = name;
	this->m_sceneMeshes.try_emplace(name, newSphere);
	return newSphere;
}