#pragma once
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <FMOD/fmod.hpp>

#include "cModel.h"
#include "iMovable.h"
#include "RigidBody.h"

class cMeshObject : public iMovable {
public:
	cMeshObject(cModel* parent);
	~cMeshObject();

	cModel*			m_parentModel;

	std::string		m_meshName;
	glm::vec3		m_position;
	glm::vec3		m_rotation;
	glm::vec3		m_scale;
	glm::quat		qRotation;
	bool			m_bUse_RGBA_colour; // When true, it will overwrite the vertex colours
	glm::vec4		m_RGBA_colour; // Diffuse Colour RGB & Alpha, 0,0,0,1 (black, with transparency of 1.0)
	glm::vec4		m_specular_colour_and_power;  // RGB is the specular highlight colour; w is the power
	bool			m_isWireframe;
	bool			m_bDoNotLight;
	bool			m_bIsVisible;
	bool			m_displayBoundingBox;

	physics::iRigidBody* physicsBody;

	FMOD::Channel* attached_sound;

	std::vector< cMeshObject* > vecChildMeshes;

	std::string textures[8];
	float textureRatios[8];
	int m_numOfTexturesLoaded;
	bool useDiscardTexture;
	bool isTextureImposter;

	glm::vec3	getGLMPosition();
	Vector3		getVECPosition();
	void		setPosition(glm::vec3 pos);
	void		setPosition(Vector3 pos);
};

