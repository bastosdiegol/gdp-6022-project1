#pragma once

#include "iShape.h"

namespace physics {
	class AABBShape : public iShape {
	public:
		float Min[3];
		float Max[3];

		AABBShape(float min[3], float max[3]);
		virtual ~AABBShape();

		static AABBShape* Cast(iShape* shape);

	protected:
		AABBShape(ShapeType shapeType)
			: iShape(shapeType) {
		}

	private:
		AABBShape(const AABBShape&) : iShape(ShapeType::AABB) {}
		AABBShape& operator=(const AABBShape&) {
			return *this;
		}
	};
}
