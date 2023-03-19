#include "cMeshFactory.h"

cMeshFactory::cMeshFactory(cVAOManager* vaoManager, std::map<std::string, cMeshObject*>& meshArray) {
	this->m_sceneMeshes = meshArray;

	this->m_cube = vaoManager->PrepareNewModel("cube1x1", "assets/mesh_factory/cube1x1.ply");
	vaoManager->LoadModelIntoVAO(this->m_cube);
	
	this->m_sphere = vaoManager->PrepareNewModel("ball1d", "assets/mesh_factory/ball1d.ply");
	vaoManager->LoadModelIntoVAO(this->m_sphere);
}

cMeshFactory::~cMeshFactory() {
}

cMeshObject* cMeshFactory::createCubeMesh() {
	cMeshObject* newCube = new cMeshObject(m_cube);
	return newCube;
}

cMeshObject* cMeshFactory::createSphereMesh() {
	cMeshObject* newSphere = new cMeshObject(m_sphere);
	return newSphere;
}
