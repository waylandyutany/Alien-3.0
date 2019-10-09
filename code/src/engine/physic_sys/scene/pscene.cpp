#include "StdAfx.h"

#include "scene.h"
#include "static_actor.h"
#include "dynamic_actor.h"

#include "../device/device.h"

#include "../../debug_sys/output/outputs.h"
#include "../../scene_sys/scene_sys.h"

namespace ae { namespace physic {

C_Scene::C_Scene(C_Resources& resources, C_Device& device) : m_Resources(resources), m_Device(device), m_Impl(device.Impl())
{
	m_Resources.CreateMaterial(m_DefaultMaterial);
}

C_Scene::~C_Scene()
{
	AE_ASSERT(m_StaticActors.empty() && "There must be no static actor on scene !");
	m_Resources.DestroyMaterial(m_DefaultMaterial);
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_Scene::AddSceneNode(ae::scene::S_SceneNode* pSceneNode)
{
	// for now we create only ground and building collision
	while(pSceneNode && pSceneNode->pNode && pSceneNode->pNode->child)
	{
		ae::scene::S_ScenePrefabBase* pInst(pSceneNode->pNode->child);
		while(pInst)
		{
			// we found collision node instance
			if(pInst->type == ae::scene::E_SINT_CGeometry)
			{
				ae::scene::S_ScenePrefab_CGeometry* pCInst((ae::scene::S_ScenePrefab_CGeometry*)pInst);
				ae::scene::S_CollisionResource::S_Geometry* pGeometry(pCInst->pGeometry);

				if(pCInst->pGeometry)
				{
					C_StaticActor* pStaticActor = new C_StaticActor(pSceneNode);
					m_StaticActors.push_back(pStaticActor);

					if(m_Impl.CreateStaticActor(pSceneNode->worldMatrix, pStaticActor->Impl(), pStaticActor))
					{
						ae::scene::S_CollisionResource::S_Geometry::S_Mesh* pMesh(pGeometry->meshes);
						while(pMesh)
						{
							m_Impl.AddStaticActorGeometry(pStaticActor->Impl(), pMesh->m_Mesh.Impl(), m_DefaultMaterial.Impl());
							pMesh = pMesh->next;
						}
					}
				}
			}
			pInst = pInst->next;
		}

		pSceneNode = pSceneNode->next;
	}
}

void C_Scene::RemoveSceneNode(ae::scene::S_SceneNode* pSceneNode)
{
	while(pSceneNode)
	{
		T_StaticActors::iterator i(m_StaticActors.begin());
		for(; i != m_StaticActors.end(); ++i)
			if((*i)->SceneNode() == pSceneNode)
			{
				m_Impl.DeleteStaticActor((*i)->Impl());
				delete (*i);
				i = m_StaticActors.erase(i);
				if(i == m_StaticActors.end())
					break;
			}
		pSceneNode = pSceneNode->next;
	}
}

// *****************************************************************************************************
// *****************************************************************************************************

C_DynamicActor* C_Scene::CreateDynamicActor(const S_Cylinder& shape, const ae::math::float3& pos)
{
	C_DynamicActor* pDynamicActor = new C_DynamicActor();
	m_Impl.CreateDynamicActor(shape, pos, m_DefaultMaterial.Impl(), pDynamicActor->Impl(), pDynamicActor);
	return pDynamicActor;
}

C_DynamicActor* C_Scene::CreateDynamicActor(const S_Capsule& shape, const ae::math::float3& pos)
{
	C_DynamicActor* pDynamicActor = new C_DynamicActor();
	m_Impl.CreateDynamicActor(shape, pos, m_DefaultMaterial.Impl(), pDynamicActor->Impl(), pDynamicActor);
	return pDynamicActor;
}

C_DynamicActor* C_Scene::CreateDynamicActor(const S_Box& shape, const ae::math::float3& pos)
{
	C_DynamicActor* pDynamicActor = new C_DynamicActor();
	m_Impl.CreateDynamicActor(shape, pos, m_DefaultMaterial.Impl(), pDynamicActor->Impl(), pDynamicActor);
	return pDynamicActor;
}

C_DynamicActor* C_Scene::CreateDynamicActor(const S_Sphere& shape, const ae::math::float3& pos)
{
	C_DynamicActor* pDynamicActor = new C_DynamicActor();
	m_Impl.CreateDynamicActor(shape, pos, m_DefaultMaterial.Impl(), pDynamicActor->Impl(), pDynamicActor);
	return pDynamicActor;
}

void C_Scene::DestroyDynamicActor(C_DynamicActor* pDynamicActor)
{
	m_Impl.DeleteDynamicActor(pDynamicActor->Impl());
	delete pDynamicActor;
}
// *****************************************************************************************************
// *****************************************************************************************************

} } // namespace ae { namespace physic {
