#include "AABBShape.h"

#include <memory>

namespace physics {
	AABBShape::AABBShape(float min[3], float max[3])
		: iShape(ShapeType::AABB) {
		memcpy(&(Min[0]), &(min[0]), 3 * sizeof(float));
		memcpy(&(Max[0]), &(max[0]), 3 * sizeof(float));
	}

	AABBShape::~AABBShape() {
	}

	AABBShape* AABBShape::Cast(iShape* shape) {
		return dynamic_cast<AABBShape*>(shape);
	}
}