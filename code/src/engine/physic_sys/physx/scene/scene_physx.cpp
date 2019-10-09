#include "StdAfx.h"

#include "scene_physx.h"
#include "static_actor_physx.h"
#include "dynamic_actor_physx.h"

#include "../device/device_physx.h"

#include "../../scene/scene.h"
#include "../../scene/collision_params.h"
#include "../../scene/collision_objects.h"

#include "../resources/resources_physx.h"
#include "../../resources/resources.h"

#include "../utils_physx.h"
#include "../../../debug_sys/output/outputs.h"
#include "../../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../../_sys/templates.h"

//#include "../../../math_sys/types_dbg.h"
#include "../../../math_sys/plane.h"


namespace ae { namespace physic { namespace physx {

// *****************************************************************************************************
// *****************************************************************************************************

PxConvexMesh* C_Scene::convexCylinderMesh = 0;
PxConvexMesh* C_Scene::convexCylinderCupMesh = 0;

C_Scene::C_Scene(C_Device& device) : 
	m_Device(device)
{
	AE_ASSERT(!convexCylinderMesh);
	AE_ASSERT(!convexCylinderCupMesh);

	ae::base::C_IncrementalAllocator allocator;
	allocator.Create((char*)::malloc(65536),65536);
	C_OutputStream allocOutput(allocator);

	PxConvexMeshDesc convexDesc;

	const int numCylinderSegment = 16;
	PxVec3 cylinderVerts[numCylinderSegment * 2];
	const float radStep(ae::math::pi2 / numCylinderSegment);
	float rad = radStep * 0.5f; // this initial shift will make l/r/u/b edges axis aligned
	for(int i = 0; i < numCylinderSegment; i++,rad+=radStep)
	{
		cylinderVerts[i].x = cosf(rad);
		cylinderVerts[i].z = -sinf(rad);
		cylinderVerts[i].y = -0.5f;
	}

	convexDesc.points.count     = numCylinderSegment;
	convexDesc.points.stride    = sizeof(PxVec3);
	convexDesc.points.data      = cylinderVerts;
	convexDesc.flags            = PxConvexFlag::eCOMPUTE_CONVEX;
	allocator.Reset(true);

	if(!m_Device.Cooking().cookConvexMesh(convexDesc, allocOutput))
		DBG_ERR << "Unable to cook circle convex mesh !";

	C_InputStream input(allocator.GetBottomPointer(), allocator.GetUsage());
	convexCylinderCupMesh = m_Device.Physics().createConvexMesh(input);

	for(int i = 0; i < numCylinderSegment; i++)
	{
		cylinderVerts[i+numCylinderSegment].x = cylinderVerts[i].x;
		cylinderVerts[i+numCylinderSegment].z = cylinderVerts[i].z;
		cylinderVerts[i+numCylinderSegment].y = 0.5f;
	}

	convexDesc.points.count     = numCylinderSegment * 2;
	convexDesc.points.stride    = sizeof(PxVec3);
	convexDesc.points.data      = cylinderVerts;
	convexDesc.flags            = PxConvexFlag::eCOMPUTE_CONVEX;
	allocator.Reset(true);

	if(!m_Device.Cooking().cookConvexMesh(convexDesc, allocOutput))
		DBG_ERR << "Unable to cook circle convex mesh !";

	C_InputStream input2(allocator.GetBottomPointer(), allocator.GetUsage());
	convexCylinderMesh = m_Device.Physics().createConvexMesh(input2);

	::free(allocator.GetBottomPointer());
}

C_Scene::~C_Scene()
{
	SafeRelease(convexCylinderCupMesh);
	SafeRelease(convexCylinderMesh);
}

// *****************************************************************************************************
bool C_Scene::CreateStaticActor(const ae::math::float4x4& transformation, C_StaticActor& actor, void* pUSerData)
{
	PxTransform pos((PxMat44&)transformation);
	actor.m_pStatic = m_Device.Physics().createRigidStatic(pos);
	actor.m_pStatic->userData = pUSerData;
	return true;
}

void C_Scene::DeleteStaticActor(C_StaticActor& actor)
{
	m_Device.Scene().removeActor(*actor.m_pStatic);
	SafeRelease<>(actor.m_pStatic);
}

void C_Scene::AddStaticActorGeometry(C_StaticActor& actor, C_Mesh& mesh, C_Material& material)
{
	::physx::PxTriangleMeshGeometry meshTriangleGeometry(&mesh.TriangleMesh());
	actor.m_pStatic->createShape(meshTriangleGeometry, material.Material())->setFlag(PxShapeFlag::eUSE_SWEPT_BOUNDS, true); // CCD
	if(actor.m_pStatic->getNbShapes() == 1)
		m_Device.Scene().addActor(*actor.m_pStatic);
}
// *****************************************************************************************************

bool C_Scene::CreateDynamicActor(const PxGeometry& geometry, const PxTransform& localPose, const ae::math::float3& pos, PxMaterial& material, C_DynamicActor& actor, void* pUserData)
{
	PxShape *pNewShape;
	PxTransform _pos((PxVec3&)pos);
	actor.m_pDynamic = m_Device.Physics().createRigidDynamic(_pos);
	pNewShape = actor.m_pDynamic->createShape(geometry, material, localPose);

	actor.m_pDynamic->userData = pUserData;
	pNewShape->setFlag(PxShapeFlag::eUSE_SWEPT_BOUNDS, true); // CCD
	m_Device.Scene().addActor(*actor.m_pDynamic);

	return true;
}

bool C_Scene::CreateDynamicActor(const S_Cylinder& cylinder, const ae::math::float3& pos, C_Material& material, C_DynamicActor& actor, void* pUserData)
{
	PxConvexMeshGeometry cylinderGeometry;
	PxTransform localPose;
	CylinderToGeometry(cylinder, ae::math::float3(0,0,0), cylinderGeometry, localPose);

	return CreateDynamicActor(cylinderGeometry, localPose, pos, material.Material(), actor, pUserData);
}

bool C_Scene::CreateDynamicActor(const S_Capsule& capsule, const ae::math::float3& pos, C_Material& material, C_DynamicActor& actor, void* pUserData)
{
	PxCapsuleGeometry capsuleGeometry;
	PxTransform localPose;
	CapsuleToGeometry(capsule, ae::math::float3(0,0,0), capsuleGeometry, localPose);

	return CreateDynamicActor(capsuleGeometry, localPose, pos, material.Material(), actor, pUserData);
}

bool C_Scene::CreateDynamicActor(const S_Box& box, const ae::math::float3& pos, C_Material& material, C_DynamicActor& actor, void* pUserData)
{
	PxBoxGeometry boxGeometry;
	PxTransform localPose;
	BoxToGeometry(box, ae::math::float3(0,0,0), boxGeometry, localPose);

	return CreateDynamicActor(boxGeometry, localPose, pos, material.Material(), actor, pUserData);
}

bool C_Scene::CreateDynamicActor(const S_Sphere& sphere, const ae::math::float3& pos, C_Material& material, C_DynamicActor& actor, void* pUserData)
{
	PxSphereGeometry sphereGeometry;
	PxTransform localPose;
	SphereToGeometry(sphere, ae::math::float3(0,0,0), sphereGeometry, localPose);

	return CreateDynamicActor(sphereGeometry, localPose, pos, material.Material(), actor, pUserData);
}

void C_Scene::DeleteDynamicActor(C_DynamicActor& actor)
{
	m_Device.Scene().removeActor(*actor.m_pDynamic);
	SafeRelease(actor.m_pDynamic);
}
// *****************************************************************************************************
class C_SingleIgnoreFilter : public ::physx::PxSceneQueryFilterCallback
{
	const ae::physic::C_Actor* m_pIgnoreActor;

	virtual PxSceneQueryHitType::Enum  preFilter (const PxFilterData &filterData, PxShape *shape, PxSceneQueryFilterFlags &filterFlags)
	{
		if(m_pIgnoreActor && shape)
		{
			ae::physic::C_Actor* pCollidingActor = static_cast<ae::physic::C_Actor*>(shape->getActor().userData);
			if(pCollidingActor == m_pIgnoreActor)
				return ::physx::PxSceneQueryHitType::eNONE;
		}
		return  ::physx::PxSceneQueryHitType::eBLOCK;
	}

	virtual PxSceneQueryHitType::Enum  postFilter (const PxFilterData &filterData, const PxSceneQueryHit &hit)
	{
		return  ::physx::PxSceneQueryHitType::eBLOCK;
	}
public:
	C_SingleIgnoreFilter() : m_pIgnoreActor(0) { }
	C_SingleIgnoreFilter(const ae::physic::C_Actor* pIgnoreActor) : m_pIgnoreActor(pIgnoreActor) { }
};


class C_MultiCastIgnoreFilter : public ::physx::PxSceneQueryFilterCallback
{
	const ae::physic::C_Actor* m_pIgnoreActor;

	virtual PxSceneQueryHitType::Enum  preFilter (const PxFilterData &filterData, PxShape *shape, PxSceneQueryFilterFlags &filterFlags)
	{
		if(m_pIgnoreActor && shape)
		{
			ae::physic::C_Actor* pCollidingActor = static_cast<ae::physic::C_Actor*>(shape->getActor().userData);
			if(pCollidingActor == m_pIgnoreActor)
				return ::physx::PxSceneQueryHitType::eNONE;
		}
		return  ::physx::PxSceneQueryHitType::eTOUCH;
	}

	virtual PxSceneQueryHitType::Enum  postFilter (const PxFilterData &filterData, const PxSceneQueryHit &hit)
	{
		return  ::physx::PxSceneQueryHitType::eTOUCH;
	}
public:
	C_MultiCastIgnoreFilter() : m_pIgnoreActor(0) { }
	C_MultiCastIgnoreFilter(const ae::physic::C_Actor* pIgnoreActor) : m_pIgnoreActor(pIgnoreActor) { }
};
// *****************************************************************************************************
bool C_Scene::LineCollision(const ae::math::float3& lineStart, const ae::math::float3& lineEnd, ae::physic::S_CollisionResult& result, ae::physic::C_Actor* pIgnore) const
{
	const ::physx::PxSceneQueryFlags outputFlags = ::physx::PxSceneQueryFlag::eDISTANCE | ::physx::PxSceneQueryFlag::eIMPACT | ::physx::PxSceneQueryFlag::eNORMAL | ::physx::PxSceneQueryFlag::eNORMAL;// | 
	PxSceneQueryFilterFlags filterFlags = PxSceneQueryFilterFlag::eSTATIC|PxSceneQueryFilterFlag::eDYNAMIC|PxSceneQueryFilterFlag::ePREFILTER;
	PxSceneQueryFilterData filterData(filterFlags);
	C_SingleIgnoreFilter ignoreFilter(pIgnore);

	ae::math::float3 dir(lineEnd - lineStart);
	::physx::PxVec3 origin(lineStart.x,lineStart.y,lineStart.z);
	::physx::PxReal maxDistance(dir.size());
	if(maxDistance < ae::math::very_small_float) return false;
	ae::math::float3 ndir(dir.normalize());
	::physx::PxVec3 unitDir(ndir.x,ndir.y,ndir.z);
	::physx::PxRaycastHit hit;

	if(m_Device.Scene().raycastSingle(origin, unitDir, maxDistance, outputFlags, hit, filterData, &ignoreFilter))
	{
		const float timeToColision(hit.distance / maxDistance);
		memcpy(&result.collisionNormal, &hit.normal, sizeof(result.collisionNormal));
		memcpy(&result.collisionContact, &hit.impact, sizeof(result.collisionContact));
		result.timeToCollision = timeToColision;
		result.distanceToCollision = hit.distance;

		if(hit.shape && hit.shape->getActor().isRigidDynamic())
			result.pDynamicActor = (ae::physic::C_DynamicActor*)hit.shape->getActor().userData;
		else if(hit.shape && hit.shape->getActor().isRigidStatic())
			result.pStaticActor = (ae::physic::C_StaticActor*)hit.shape->getActor().userData;

		return true;
	}

	return false;
}


bool C_Scene::SweepCollision(const PxGeometry& geometry, const PxTransform& sweepOrigin, const S_CollisionParams& params, S_CollisionResult& result) const
{
	const PxU32 maxHitsCount = 32;
	PxSweepHit hits[maxHitsCount];
	bool blockedHit;
	const ::physx::PxSceneQueryFlags outputFlags =	PxSceneQueryFlag::eDISTANCE |
													PxSceneQueryFlag::eIMPACT |
													PxSceneQueryFlag::eNORMAL |
													PxSceneQueryFlag::eINITIAL_OVERLAP | 
													PxSceneQueryFlag::eINITIAL_OVERLAP_KEEP; 

	PxSceneQueryFilterFlags filterFlags =	PxSceneQueryFilterFlag::eSTATIC |
											PxSceneQueryFilterFlag::eDYNAMIC |
											PxSceneQueryFilterFlag::ePREFILTER;

	PxSceneQueryFilterData filterData(filterFlags);
	C_MultiCastIgnoreFilter ignoreFilter(params.pActorToIgnore);

	PxVec3 direction(params.direction.x, params.direction.y, params.direction.z);

	PxI32 nbHits = m_Device.Scene().sweepMultiple(geometry, sweepOrigin, direction, params.distance, outputFlags, hits, maxHitsCount, blockedHit, filterData, &ignoreFilter);
	AE_ASSERT(nbHits != -1 && "Hits buffer overflowed !");

	// ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
	// sweepSingle doesn't return closest contact so we must use sweepMultiple instead
	// however sweepMultiple doesn't sort hits and even doesn't fill distances for other then first hit
	// so at first we have to calculate distances for returned hits and then pick closest one
	// ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !

	if(nbHits)
	{
		ae::u32 closestHit = 0;
		float minDistance = params.distance + 0.1f;
		// calculating new distance from the sweep origin based on sweep direction and keep closest distance hit index
		for(PxI32 i = 0; i < nbHits; i++) {
			hits[i].distance = ae::math::PlaneDistance(params.pos, CreatePlane(*(ae::math::float3*)&hits[i].impact, params.direction)) - params.distance;
			if(hits[i].distance < minDistance) {
				minDistance = hits[i].distance;
				closestHit = i;
			}
		}


		memcpy(&result.collisionNormal, &hits[closestHit].normal, sizeof(result.collisionNormal));
		memcpy(&result.collisionContact, &hits[closestHit].impact, sizeof(result.collisionContact));

		const float timeToColision(hits[closestHit].distance / params.distance);
		result.timeToCollision = timeToColision;
		result.distanceToCollision = hits[closestHit].distance;

		if(hits[closestHit].shape && hits[closestHit].shape->getActor().isRigidDynamic())
			result.pDynamicActor = (ae::physic::C_DynamicActor*)hits[closestHit].shape->getActor().userData;
		else if(hits[closestHit].shape && hits[closestHit].shape->getActor().isRigidStatic())
			result.pStaticActor = (ae::physic::C_StaticActor*)hits[closestHit].shape->getActor().userData;

		return true;
	}

	return false;
}

bool C_Scene::SweepCollision(const S_Cylinder& cylinder, const S_CollisionParams& params, S_CollisionResult& result) const
{
	PxConvexMeshGeometry cylinderGeometry;
	PxTransform sweepOrigin;

	CylinderToGeometry(cylinder, params.pos, cylinderGeometry, sweepOrigin);

	return SweepCollision(cylinderGeometry, sweepOrigin, params, result);
}

bool C_Scene::SweepCollision(const S_Capsule& capsule, const S_CollisionParams& params, S_CollisionResult& result) const
{
	PxCapsuleGeometry capsuleGeometry;
	PxTransform sweepOrigin;

	CapsuleToGeometry(capsule, params.pos, capsuleGeometry, sweepOrigin);

	return SweepCollision(capsuleGeometry, sweepOrigin, params, result);
}

bool C_Scene::SweepCollision(const S_Box& box, const S_CollisionParams& params, S_CollisionResult& result) const
{
	PxBoxGeometry boxGeometry;
	PxTransform sweepOrigin;
	BoxToGeometry(box, params.pos, boxGeometry, sweepOrigin);

	return SweepCollision(boxGeometry, sweepOrigin, params, result);
}

bool C_Scene::SweepCollision(const S_Sphere& sphere, const S_CollisionParams& params, S_CollisionResult& result) const
{
	PxSphereGeometry sphereGeometry;
	PxTransform sweepOrigin;
	SphereToGeometry(sphere, params.pos, sphereGeometry, sweepOrigin);

	return SweepCollision(sphereGeometry, sweepOrigin, params, result);
}

// *****************************************************************************************************

} } } // namespace ae { namespace physic { namespace physx {
