#include <RayTracing/Sky.h>

#include <RayTracing/Ray.h>

#include <Utility/Math.h>

using namespace RayTracing;
using namespace CppUtility::Other;
using namespace glm;

const AABB Sky::box(vec3(10e-20f), vec3(10e20f));

HitRst Sky::RayIn(Ray::Ptr & ray) const {
	if (ray->GetTMax() != FLT_MAX)
		return HitRst::InValid;

	HitRst hitRst(true);
	vec3 normal = normalize(ray->GetDir());
	vec2 uv = Math::Sphere2UV(normal);
	hitRst.record = HitRecord(ray, normal, -normal, uv[0], uv[1]);
	hitRst.material = GetMat();
	hitRst.isMatCoverable = IsMatCoverable();

	return hitRst;
}