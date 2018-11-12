#ifndef _SKY_H_
#define _SKY_H_

#include <RayTracing/Hitable.h>

namespace RayTracing {
	class Sky : public Hitable {
		HEAP_OBJ_SETUP(Sky)
	public:
		using Hitable::Hitable;

		virtual HitRst RayIn(Ray::Ptr & ray) const;

		virtual AABB BoundingBox() const;
	};
}

#endif // !_SKY_H_