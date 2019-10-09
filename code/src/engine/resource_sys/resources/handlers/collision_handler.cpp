#include "StdAfx.h"

#include "collision_handler.h"
#include "../resources_manager.h"

#include "../../../debug_sys/output/outputs.h"
#include "../../../math_sys/types_dbg.h"
#include "../../../base_sys/file_system/file_system.h"

#include "../../../scene_sys/scene/scene.h"

#include "../../../physic_sys/physic_sys.h"
#include "../../../physic_sys/resources/resource_collision.h"

using namespace ae::scene;

namespace ae { namespace resource {

C_CollisionResourceHandler::C_CollisionResourceHandler() : I_ResourceHandler()
{
}

void C_CollisionResourceHandler::Activate()
{
	I_ResourceHandler::Activate();
	ResolvePointers();

	ae::scene::S_CollisionResource* pResource(GetResource()->GetData<ae::scene::S_CollisionResource>());
	ae::scene::S_CollisionResource::S_Geometry* pGeometry(pResource->geometries);
	while(pGeometry)
	{
		ae::scene::S_CollisionResource::S_Geometry::S_Mesh* pMesh(pGeometry->meshes);
		while(pMesh)
		{
			if(pResource->bHasConvexMeshes) {
				if(!ae::physic::C_PhysicSys::Instance().Resources().CreateConvexMesh(pMesh->pBakedMeshBuffer, pMesh->sizeBakedMeshBuffer, pMesh->m_Mesh))
					DBG_ERR << "Unable to create convex collision mesh !" << std::endl;
			} else {
				if(!ae::physic::C_PhysicSys::Instance().Resources().CreateTriangleMesh(pMesh->pBakedMeshBuffer, pMesh->sizeBakedMeshBuffer, pMesh->m_Mesh))
					DBG_ERR << "Unable to create triangle collision mesh !" << std::endl;
			}

			pMesh = pMesh->next;
		}
		pGeometry = pGeometry->next;
	}
}

void C_CollisionResourceHandler::Deactivate()
{
	ae::scene::S_CollisionResource* pResource(GetResource()->GetData<ae::scene::S_CollisionResource>());
	ae::scene::S_CollisionResource::S_Geometry* pGeometry(pResource->geometries);
	while(pGeometry)
	{
		ae::scene::S_CollisionResource::S_Geometry::S_Mesh* pMesh(pGeometry->meshes);
		while(pMesh)
		{
			if(pResource->bHasConvexMeshes)
				ae::physic::C_PhysicSys::Instance().Resources().DestroyTriangleMesh(pMesh->m_Mesh);
			else
				ae::physic::C_PhysicSys::Instance().Resources().DestroyConvexMesh(pMesh->m_Mesh);
	
			pMesh = pMesh->next;
		}
		pGeometry = pGeometry->next;
	}

	I_ResourceHandler::Deactivate();
}

/*virtual*/ void C_CollisionResourceHandler::EnumerateElements(I_ResourceElementsEnumerator& enumerator)
{
	REFERENCE(enumerator);

	ResolvePointers();

	ae::scene::S_CollisionResource* pResource(GetResource()->GetData<ae::scene::S_CollisionResource>());
	ae::scene::S_CollisionResource::S_Geometry* pGeometry(pResource->geometries);
	while(pGeometry)
	{
		enumerator.OnElement(pGeometry->hid.id, elementTypeCGeo);
		pGeometry = pGeometry->next;
	}
/*
	S_CollisionResource* pNode = reinterpret_cast<S_CollisionResource*>(GetResource()->GetData<S_Static_Collision_Node>());
	S_CollisionResource::S_Geometry* pGeometry = pNode->geometries;
	while(pGeometry)
	{
		enumerator.OnElement(pGeometry->hid.id, elementTypeCGeo);
		pGeometry = pGeometry->next;
	}
*/
}

/*virtual*/ void* C_CollisionResourceHandler::FindElement(const char* elementName)
{
	REFERENCE(elementName);

	T_ResourceType resourceType = String2ResourceType(ae::base::GetFileExtension(elementName).c_str());
	if(resourceType != elementTypeCGeo) return 0;
	ae::base::T_Hash32 hashElementName = ae::base::CalculateHashI32(ae::base::GetFileName(elementName, false).c_str());
	ae::scene::S_CollisionResource* pResource(GetResource()->GetData<ae::scene::S_CollisionResource>());
	ae::scene::S_CollisionResource::S_Geometry* pGeometry(pResource->geometries);
	while(pGeometry)
	{
		if(pGeometry->hid.idHash == hashElementName)
			return (void*)pGeometry;

		pGeometry = pGeometry->next;
	}

	return 0;
/*
	T_ResourceType resourceType = String2ResourceType(ae::base::GetFileExtension(elementName).c_str());
	if(resourceType != elementTypeCGeo) return 0;

	ae::base::T_Hash32 hashElementName = ae::base::CalculateHashI32(ae::base::GetFileName(elementName, false).c_str());

	S_CollisionResource* pNode = reinterpret_cast<S_CollisionResource*>(GetResource()->GetData<S_Static_Collision_Node>());
	S_CollisionResource::S_Geometry* pGeometry = pNode->geometries;
	while(pGeometry)
	{
		if(pGeometry->hid.idHash == hashElementName)
			return (void*)pGeometry;
		pGeometry = pGeometry->next;
	}

	return 0;
*/
}

void C_CollisionResourceHandler::PrintDebug()
{
	I_ResourceHandler::PrintDebug();
//	S_CollisionResource2* pNode = GetResource()->GetData<S_CollisionResource2>();
}


} } // namespace ae { namespace resource {
