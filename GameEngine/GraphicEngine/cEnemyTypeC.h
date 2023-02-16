#pragma once
#include <glm/ext/vector_float3.hpp>
#include "iEnemy.h"

class cEnemyTypeC : public iEnemy {
public:
	/// <summary>
	/// Default Constructor
	/// </summary>
	/// <param name="pos">Enemy Position</param>
	/// <param name="rot">Enemy Rotation</param>
	/// <param name="tarPos">Player Position</param>
	/// <param name="tarRot">Player Rotation</param>
	/// <param name="tarFacing">Player Direction</param>
	cEnemyTypeC(glm::vec3& pos, glm::vec3& rot, glm::vec3& tarPos, glm::vec3& tarRot, glm::vec3& tarFacing)
		: position(pos), rotation(rot), targetPos(tarPos), targetRot(tarRot), targetFacing(tarFacing) {
	}
	/// <summary>
	/// Destructor
	/// </summary>
	~cEnemyTypeC();

	/// <summary>
	/// Perform Behaviour for Enemy Type A
	/// Seeks or Flee the Player based on Player Facing Direction
	/// Default Angle for Pursue or Evade: 45�
	/// </summary>
	void performBehaviour();

private:
	const float MOVE_SPEED = 0.02f;

	glm::vec3& position;
	glm::vec3& rotation;

	glm::vec3& targetPos;
	glm::vec3& targetRot;
	glm::vec3& targetFacing;

	/// <summary>
	/// Approaches the current position of the Player
	/// But maintains a certain minimum distance
	/// </summary>
	void Approaches(const float minDistance);
};

