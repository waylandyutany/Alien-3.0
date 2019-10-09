#include "StdAfx.h"

#include "utils_physx.h"

#include "../scene/collision_objects.h"
#include "scene/scene_physx.h"

namespace ae { namespace physic { namespace physx {

	void CapsuleToGeometry(const ae::physic::S_Capsule& capsule, const ae::math::float3& pos, PxCapsuleGeometry& capsuleGeometry, PxTransform& pose)
	{
		pose = PxTransform(PxVec3(pos.x, pos.y, pos.z), PxQuat(PxHalfPi, PxVec3(0,0,1)));

		capsuleGeometry.radius = capsule.radius;
		capsuleGeometry.halfHeight = (capsule.height * 0.5f) - (capsule.radius * 1.0f);
	}

	void CylinderToGeometry(const ae::physic::S_Cylinder& cylinder, const ae::math::float3& pos, PxConvexMeshGeometry& cylinderGeometry, PxTransform& pose)
	{
		pose = PxTransform(PxVec3(pos.x, pos.y, pos.z));//, PxQuat(PxHalfPi, PxVec3(0,0,1)));

		PxMeshScale scale(PxVec3(cylinder.radius, cylinder.height, cylinder.radius), PxQuat::createIdentity());
		cylinderGeometry = PxConvexMeshGeometry(C_Scene::convexCylinderMesh, scale);
	}

	void BoxToGeometry(const ae::physic::S_Box& box, const ae::math::float3& pos, PxBoxGeometry& boxGeometry, PxTransform& pose)
	{
		pose = PxTransform(PxVec3(pos.x, pos.y, pos.z));

		boxGeometry.halfExtents.x = box.width  * 0.5f;
		boxGeometry.halfExtents.y = box.height * 0.5f;
		boxGeometry.halfExtents.z = box.depth  * 0.5f;
	}

	void SphereToGeometry(const ae::physic::S_Sphere& sphere, const ae::math::float3& pos, PxSphereGeometry& sphereGeometry, PxTransform& pose)
	{
		pose = PxTransform(PxVec3(pos.x, pos.y, pos.z));

		sphereGeometry.radius = sphere.radius;
	}

} } } // namespace ae { namespace physic { namespace physx {

