#include "StdAfx.h"

#include "scene_prefabs_handler.h"
#include "../../../scene_sys/scene_prefabs/scene_prefabs_resource.h"
#include "../../../render_sys/render_device/resources/resource_mesh.h"
#include "../../../render_sys/render_device/render_resources.h"
#include "../../../render_sys/render_sys.h"

#include "../../../debug_sys/output/outputs.h"

namespace ae { namespace resource {

C_ScenePrefabsResourceHandler::C_ScenePrefabsResourceHandler() :
	I_ResourceHandler()
{
}

void C_ScenePrefabsResourceHandler::DeactivateNode(ae::scene::S_ScenePrefabBase* pNode)
{
	while(pNode)
	{
		if(pNode->type == ae::scene::E_SINT_MGeometry)
		{
			ae::scene::S_ScenePrefab_MGeometry* pMGeo((ae::scene::S_ScenePrefab_MGeometry*)pNode);
			ae::render::C_RenderSys::Instance().GetResources().GetImpl()->DestroyStructuredBufferResource(pMGeo->instancesCB);
		}
		DeactivateNode(pNode->child);
		pNode = pNode->next;
	}
}

void C_ScenePrefabsResourceHandler::ActivateNode(ae::scene::S_ScenePrefabBase* pNode)
{
	while(pNode)
	{
		if(pNode->type == ae::scene::E_SINT_Instance)
		{
//			ae::scene::S_SceneInst_Instance* pInstance((ae::scene::S_SceneInst_Instance*)pNode);
		}
		else if(pNode->type == ae::scene::E_SINT_LOD)
		{
//			ae::scene::S_SceneInst_LOD* pLOD((ae::scene::S_SceneInst_LOD*)pNode);
		}
		else if(pNode->type == ae::scene::E_SINT_MGeometry)
		{
			ae::scene::S_ScenePrefab_MGeometry* pMGeo((ae::scene::S_ScenePrefab_MGeometry*)pNode);
			pMGeo->pGeometry = ae::resource::C_ResourceSys::Inst().Resources().FindElement<ae::render::S_MeshResource::S_Geometry>(pMGeo->elementPath);
			ae::resource::S_Resource* pResource(ae::resource::C_ResourceSys::Inst().Resources().FindResource<ae::resource::S_Resource>(pMGeo->elementPath));
			if(pResource) pMGeo->pMesh = pResource->GetData<ae::render::S_MeshResource>();
			if(pMGeo->numInstances)
			{
				ae::render::dx11::T_BufferFlags flags = ae::render::dx11::E_BF_VS;
				ae::render::S_InstanciesBuffer& inst(ae::render::C_RenderSys::Instance().GetResources().InstanciesBuffer());
				for(ae::u32 i = 0; i < pMGeo->numInstances; i++)
					inst.instanciesMatricies[i] = pMGeo->instances[i].worldMatrix.Transpose();
				ae::render::C_RenderSys::Instance().GetResources().GetImpl()->CreateStructuredBufferResource(inst.instanciesMatricies,pMGeo->numInstances, sizeof(ae::math::float4x4),flags, ae::render::dx11::E_BSI_PerObject, pMGeo->instancesCB);
				inst.Clear();
			}
		}
		else if(pNode->type == ae::scene::E_SINT_CGeometry)
		{
			ae::scene::S_ScenePrefab_CGeometry* pCGeo((ae::scene::S_ScenePrefab_CGeometry*)pNode);

			pCGeo->pGeometry = ae::resource::C_ResourceSys::Inst().Resources().FindElement<ae::scene::S_CollisionResource::S_Geometry>(pCGeo->elementPath);
			S_Resource* pResource = ae::resource::C_ResourceSys::Inst().Resources().FindResource<S_Resource>(pCGeo->elementPath);
			if(pResource)
				pCGeo->pCollision = pResource->GetData<ae::scene::S_CollisionResource>();
		}
		
		ActivateNode(pNode->child);
		pNode = pNode->next;
	}
}

/*virtual*/ void C_ScenePrefabsResourceHandler::Activate()
{
	I_ResourceHandler::Activate();
	ResolvePointers();

	ae::scene::S_ScenePrefabsResource* pScenePrefabsResource = static_cast<ae::scene::S_ScenePrefabsResource*>(GetResource());

	ActivateNode(pScenePrefabsResource->instancies);

//	PrintDebug();
}

/*virtual*/ void C_ScenePrefabsResourceHandler::Deactivate()
{
	ae::scene::S_ScenePrefabsResource* pScenePrefabsResource = static_cast<ae::scene::S_ScenePrefabsResource*>(GetResource());
	DeactivateNode(pScenePrefabsResource->instancies);

	I_ResourceHandler::Deactivate();
}

/*virtual*/ void C_ScenePrefabsResourceHandler::EnumerateElements(I_ResourceElementsEnumerator& enumerator)
{
	ResolvePointers();

	ae::scene::S_ScenePrefabsResource* pScenePrefabsResource = static_cast<ae::scene::S_ScenePrefabsResource*>(GetResource());
	ae::scene::S_ScenePrefab* pPrefab = pScenePrefabsResource->instancies;
	while(pPrefab)
	{
		enumerator.OnElement(pPrefab->hid.id, resourceTypePrefab);
		pPrefab = (ae::scene::S_ScenePrefab*)pPrefab->next;
	}
}

/*virtual*/ void* C_ScenePrefabsResourceHandler::FindElement(const char* elementName)
{
	T_ResourceType resourceType = String2ResourceType(ae::base::GetFileExtension(elementName).c_str());
	if(resourceType != resourceTypePrefab) return 0;

	ae::scene::S_ScenePrefabsResource* pScenePrefabsResource = static_cast<ae::scene::S_ScenePrefabsResource*>(GetResource());
	ae::base::T_Hash32 hashElementName = ae::base::CalculateHashI32(ae::base::GetFileName(elementName, false).c_str());
	ae::scene::S_ScenePrefab* pPrefab = pScenePrefabsResource->instancies;
	while(pPrefab)
	{
		if(pPrefab->hid.idHash == hashElementName)
			return (void*)pPrefab;
		pPrefab = (ae::scene::S_ScenePrefab*)pPrefab->next;
	}

	return 0;
}

/*virtual*/ void C_ScenePrefabsResourceHandler::PrintDebug()
{
	I_ResourceHandler::PrintDebug();
	DBG_OUT << "Scene instancies resource" << std::endl;
}

} } // namespace ae { namespace resource {
