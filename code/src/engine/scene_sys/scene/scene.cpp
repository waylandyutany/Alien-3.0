#include "StdAfx.h"

#include "scene.h"
#include "lights.h"

#include "../../resource_sys/resource_sys.h"
#include "../../resource_sys/resources/resources_manager.h"

#include "../../render_sys/render_sys.h"
#include "../../render_sys/render_scene/render_scene.h"
#include "../../render_sys/render_device/render_resources.h"

#include "../../physic_sys/physic_sys.h"

namespace ae { namespace scene {

// *********************************************************************************************************
// *********************************************************************************************************
C_DynamicSceneNode::C_DynamicSceneNode(ae::physic::C_DynamicActor* pDynamicActor, ae::render::S_MeshResource* pMesh, ae::render::S_MeshResource::S_Geometry* pGeometry) :
	m_pDynamicActor(pDynamicActor),
	m_pMesh(pMesh),
	m_pGeometry(pGeometry)
{
	ae::render::dx11::T_BufferFlags cbFlags = ae::render::dx11::E_BF_VS | ae::render::dx11::E_BF_GS;
	if(!ae::render::C_RenderSys::Instance().GetResources().GetImpl()->CreateConstantBufferResource(0, sizeof(ae::math::float4x4), cbFlags, ae::render::dx11::E_BSI_PerObject, m_CBuffer))
		DBG_ERR << "Unable to create per dynamic scene node constant buffer !" << std::endl;
}

/*virtual*/ C_DynamicSceneNode::~C_DynamicSceneNode()
{
	ae::render::C_RenderSys::Instance().GetResources().GetImpl()->DestroyConstantBufferResource(m_CBuffer);
}

// *********************************************************************************************************
// *********************************************************************************************************
C_Scene::C_Scene() {

}

C_Scene::~C_Scene() {
}

// *********************************************************************************************************
// *********************************************************************************************************
C_DynamicSceneNode* C_Scene::CreateDynamicNode(const char* meshResource, const ae::math::float3& pos)
{
	ae::resource::S_Resource* pResource(ae::resource::C_ResourceSys::Inst().Resources().FindResource<ae::resource::S_Resource>(meshResource));
	if(!pResource) return 0;
	ae::render::S_MeshResource* pMesh(pResource->GetData<ae::render::S_MeshResource>());
	if(!pMesh) return 0;
	ae::render::S_MeshResource::S_Geometry* pGeometry = ae::resource::C_ResourceSys::Inst().Resources().FindElement<ae::render::S_MeshResource::S_Geometry>(meshResource);
	if(!pGeometry) return 0;

	const float radius = (pGeometry->boundingBox.max.x - pGeometry->boundingBox.min.x) * 0.5f;
	const float height = pGeometry->boundingBox.max.y - pGeometry->boundingBox.min.y;

	ae::physic::S_Cylinder cylinder(height, radius);
	ae::physic::C_DynamicActor* pDynamicActor = ae::physic::C_PhysicSys::Instance().Scene().CreateDynamicActor(cylinder, pos);

	if(!pDynamicActor || !pMesh || !pGeometry) return 0;

	C_DynamicSceneNode* pDynamicNode = new C_DynamicSceneNode(pDynamicActor, pMesh, pGeometry);
	m_DynamicSceneNodes.push_back(pDynamicNode);
	return pDynamicNode;
}

void C_Scene::DestroyDynamicNode(C_DynamicSceneNode* pDynamicNode)
{
	ae::physic::C_PhysicSys::Instance().Scene().DestroyDynamicActor(pDynamicNode->m_pDynamicActor);

	T_DynamicSceneNodes::iterator it(std::find(m_DynamicSceneNodes.begin(), m_DynamicSceneNodes.end(), pDynamicNode));
	AE_ASSERT(it != m_DynamicSceneNodes.end() && "Trying to remove no existing dynamic scene node !");
	m_DynamicSceneNodes.erase(it);
	delete pDynamicNode;
}
// *********************************************************************************************************
// *********************************************************************************************************
void C_Scene::AddSceneNode(S_SceneNode* pSceneNode)
{
	AE_ASSERT(pSceneNode && "Invalid scene node to add !");
	AE_ASSERT(std::find(m_SceneNodes.begin(), m_SceneNodes.end(), pSceneNode) == m_SceneNodes.end() && "Scene node already added !");
	m_SceneNodes.push_back(pSceneNode);

	ae::physic::C_PhysicSys::Instance().Scene().AddSceneNode(pSceneNode);
}

void C_Scene::RemoveSceneNode(S_SceneNode* pSceneNode)
{
	ae::physic::C_PhysicSys::Instance().Scene().RemoveSceneNode(pSceneNode);

	AE_ASSERT(pSceneNode && "Invalid scene node to remove !");
	T_SceneNodes::iterator it(std::find(m_SceneNodes.begin(), m_SceneNodes.end(), pSceneNode));
	AE_ASSERT(it != m_SceneNodes.end() && "Trying to remove no existing scene node !");
	m_SceneNodes.erase(it);
}
// *********************************************************************************************************
// *********************************************************************************************************
void C_Scene::EnumerateNodes(S_SceneNode& sceneNode, S_ScenePrefabBase& sceneInstNode, E_ScenePrefab_Type type, I_NodesEnumerator& enumerator)
{
	if(sceneInstNode.type == type)
		enumerator.OnNode(sceneNode, sceneInstNode);
	S_ScenePrefabBase* pNode = sceneInstNode.child;
	while(pNode)
	{
		EnumerateNodes(sceneNode, *pNode, type, enumerator);
		pNode = pNode->next;
	}
}

void C_Scene::EnumerateNodes(I_NodesEnumerator& enumerator, E_ScenePrefab_Type type)
{
	T_SceneNodes::iterator it(m_SceneNodes.begin());
	for(; it != m_SceneNodes.end();++it)
	{
		S_SceneNode* pSceneNode((*it));
		while(pSceneNode)
		{
			if(pSceneNode->pNode)
				EnumerateNodes(*pSceneNode, *pSceneNode->pNode, type, enumerator);

			pSceneNode = pSceneNode->next;
		}
	}
}

void C_Scene::EnumerateDynamicNodes(I_NodesEnumerator& enumerator)
{
	T_DynamicSceneNodes::iterator it(m_DynamicSceneNodes.begin());
	for(;it != m_DynamicSceneNodes.end();++it)
		enumerator.OnDynamicNode(*(*it));
}

// *********************************************************************************************************
// *********************************************************************************************************

C_DirectionalLight* C_Scene::CreateDirectionalLight() {
	auto pLight(new C_DirectionalLight());
	ae::render::C_RenderSys::Instance().GetScene().Lights().AddDirLight(&pLight->light);
	return pLight;
}

C_PointLight* C_Scene::CreatePointLight() {
	auto pLight(new C_PointLight());
	ae::render::C_RenderSys::Instance().GetScene().Lights().AddPointLight(&pLight->light);
	return pLight;
}

void C_Scene::DestroyDirectionalLight(C_DirectionalLight* pLight) {
	ae::render::C_RenderSys::Instance().GetScene().Lights().RemoveDirLight(&pLight->light);
	delete pLight;
}

void C_Scene::DestroyPointLight(C_PointLight* pLight) {
	ae::render::C_RenderSys::Instance().GetScene().Lights().RemovePointLight(&pLight->light);
	delete pLight;
}

// *********************************************************************************************************
// *********************************************************************************************************

#ifdef AE_EDITOR
void C_Scene::UpdateNodeTransformation(const char* nodeName, const ae::math::float4x4& transformation)
{
	ae::base::T_Hash32 hashID(ae::base::CalculateHashI32(nodeName));

	T_SceneNodes::iterator i = m_SceneNodes.begin();
	for(; i != m_SceneNodes.end(); ++i)
	{
		S_SceneNode* pNode(*i);
		while(pNode)
		{
			if(pNode->hid.idHash == hashID)
			{
				pNode->worldMatrix = transformation;
				pNode->invWorldMatrix = transformation.Inverse();
				ae::render::C_RenderSys::Instance().GetScene().UpdateStaticNodesTransformation();
				return;
			}
			pNode = pNode->next;
		}
	}
}

void C_Scene::UpdateGeometryInstanceTransformations(const char* prefabName, const char* geometryName, const ae::math::float4x4* transformations, ae::u32 numTransformations)
{
	ae::base::T_Hash32 prefabID(ae::base::CalculateHashI32(prefabName));

	T_SceneNodes::iterator i = m_SceneNodes.begin();
	for(; i != m_SceneNodes.end(); ++i)
	{
		S_SceneNode* pNode(*i);
		while(pNode)
		{
			S_ScenePrefab* pPrefabNode(pNode->pNode->type == E_SINT_Instance ? (S_ScenePrefab*)pNode->pNode : 0);
			AE_ASSERT(pPrefabNode && "pNode->pNode must be of type E_SINT_Instance !");

			if(pPrefabNode->hid.idHash == prefabID)
			{
				S_ScenePrefab_MGeometry* pMGeo(FindGeometryNode(pPrefabNode, geometryName, numTransformations));
				if(pMGeo)
				{
					AE_ASSERT(numTransformations == pMGeo->numInstances && "Wrong instancies to update !");
					for(ae::u32 i = 0; i < numTransformations; i++)
					{
						pMGeo->instances[i].worldMatrix = transformations[i];
						pMGeo->instances[i].invWorldMatrix = pMGeo->instances[i].worldMatrix.Inverse();
					}

					ae::render::C_RenderSys::Instance().GetScene().UpdateStaticNodesTransformation();
					return;
				}
			}

			pNode = pNode->next;
		}
	}

}

#endif

// *********************************************************************************************************
// *********************************************************************************************************

} } // namespace ae { namespace scene {
