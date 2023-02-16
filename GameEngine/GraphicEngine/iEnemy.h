#pragma once
#include <glm/ext/vector_float3.hpp>

class iEnemy {
public:
	virtual ~iEnemy() {};

	virtual void performBehaviour() = 0;
};