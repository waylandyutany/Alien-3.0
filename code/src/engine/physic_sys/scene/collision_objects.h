#pragma once

#ifndef AE_PHYSIC_SYS_COLLISION_OBJECTS_H
#define AE_PHYSIC_SYS_COLLISION_OBJECTS_H

#include "../../math_sys/types.h"

namespace ae { namespace physic {

	struct S_Cylinder
	{
		S_Cylinder(float height, float radius) : height(height), radius(radius) {}
		float height, radius;
	private:
		S_Cylinder();
	};

	struct S_Box
	{
		S_Box(float height, float width, float depth) : height(height), width(width), depth(depth) {}
		float height, width, depth;
	private:
		S_Box();
	};

	struct S_Capsule
	{
		S_Capsule(float height, float radius) : height(height), radius(radius) {}
		float height, radius;
	private:
		S_Capsule();
	};

	struct S_Sphere
	{
		S_Sphere(float radius) : radius(radius) {}
		float radius;
	private:
		S_Sphere();
	};

} } // namespace ae { namespace physic {

#endif // #ifdef AE_PHYSIC_SYS_COLLISION_OBJECTS_H
