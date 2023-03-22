#pragma once
#include "cVAOManager.h"
#include "cMeshObject.h"

class cMeshFactory {
public:
	cMeshFactory(cVAOManager* vaoManager, std::map<std::string, cMeshObject*>& meshArray);
	~cMeshFactory();

<<<<<<< HEAD
	cMeshObject* createCubeMesh(std::string name);
	cMeshObject* createSphereMesh(std::string name);
private:
	std::map<std::string, cMeshObject*>& m_sceneMeshes;

	cModel* m_cube;
	cModel* m_sphere;
};
=======
	cMeshObject* createCubeMesh();
	cMeshObject* createSphereMesh();
private:
	std::map<std::string, cMeshObject*> m_sceneMeshes;

	cModel* m_cube;
	cModel* m_sphere;
};

>>>>>>> main
