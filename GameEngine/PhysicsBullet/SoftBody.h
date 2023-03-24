#pragma once

#include <iSoftBody.h>
#include <SoftBodyDesc.h>

namespace physics
{
	class SoftBody : public iSoftBody
	{
	public:
		SoftBody(const SoftBodyDesc& desc);
		virtual ~SoftBody();

		static SoftBody* Cast(iCollisionBody* body);

		virtual unsigned int GetNumNodes();
		virtual void GetNodePosition(unsigned int nodeIndex, glm::vec3& nodePosition);

	private:
		SoftBody(const SoftBody& other) { }
		SoftBody& operator=(const SoftBody& other0) {
			return *this;
		}
	};
}