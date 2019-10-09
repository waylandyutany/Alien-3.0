#include "StdAfx.h"

#include "dynamic_actor_physx.h"
#include "scene_physx.h"
#include "../utils_physx.h"

#include "../../scene/collision_objects.h"

namespace ae { namespace physic { namespace physx {

	// *****************************************************************************************************
	// *****************************************************************************************************

	void C_DynamicActor::UpdateShape(const PxGeometry& geometry, const PxTransform& localPose)
	{
		// @todo do it faster, don't recreate if same geometry but just change the scale
		PxShape *pOldShape,*pNewShape;
		PxMaterial* pCurrentMaterial;
		m_pDynamic->getShapes(&pOldShape,1,0);
		AE_ASSERT(pOldShape && "Dynamic actor must contain valid shape !");
		pOldShape->getMaterials(&pCurrentMaterial,1);
		AE_ASSERT(pOldShape && "Dynamic actor's shape must contain valid material !");

		pNewShape = m_pDynamic->createShape(geometry,*pCurrentMaterial,localPose);

		pNewShape->setFlag(PxShapeFlag::eUSE_SWEPT_BOUNDS, true); // enable CCD
		pOldShape->release(); // release old shape
	}

	void C_DynamicActor::UpdateShape(const S_Cylinder& cylinder)
	{
		PxConvexMeshGeometry cylinderGeometry;
		PxTransform localPose;
		CylinderToGeometry(cylinder, ae::math::float3(0,0,0), cylinderGeometry, localPose);
		
		UpdateShape(cylinderGeometry, localPose);
	}

	void C_DynamicActor::UpdateShape(const S_Box& box)
	{
		PxBoxGeometry boxGeometry;
		PxTransform localPose;
		BoxToGeometry(box, ae::math::float3(0,0,0), boxGeometry, localPose);

		UpdateShape(boxGeometry, localPose);
	}

	void C_DynamicActor::UpdateShape(const S_Capsule& capsule)
	{
		PxCapsuleGeometry capsuleGeometry;
		PxTransform localPose;
		CapsuleToGeometry(capsule, ae::math::float3(0,0,0), capsuleGeometry, localPose);

		UpdateShape(capsuleGeometry, localPose);
	}

	void C_DynamicActor::UpdateShape(const S_Sphere& sphere)
	{
		PxSphereGeometry sphereGeometry;
		PxTransform localPose;
		SphereToGeometry(sphere, ae::math::float3(0,0,0), sphereGeometry, localPose);

		UpdateShape(sphereGeometry, localPose);
	}

} } } // namespace ae { namespace physic { namespace physx {
