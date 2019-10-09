#include "StdAfx.h"

#include "scene_handler.h"

#include "../../../scene_sys/scene/scene_resource.h"
#include "../../../scene_sys/scene_sys.h"

#include "../../../debug_sys/output/outputs.h"

#include "../../../render_sys/render_sys.h"
#include "../../../render_sys/render_device/render_resources.h"

namespace ae { namespace resource {

C_SceneResourceHandler::C_SceneResourceHandler() :
	I_ResourceHandler()
{
}

/*virtual*/ void C_SceneResourceHandler::Activate()
{
	I_ResourceHandler::Activate();
	ResolvePointers();

	ae::scene::S_SceneResource* pSceneResource = static_cast<ae::scene::S_SceneResource*>(GetResource());
	ae::scene::S_SceneNode* pNode = pSceneResource->pNode;
	ae::render::dx11::T_BufferFlags cbFlags = ae::render::dx11::E_BF_VS | ae::render::dx11::E_BF_GS;
	while(pNode)
	{
		pNode->pNode = ae::resource::C_ResourceSys::Inst().Resources().FindElement<ae::scene::S_ScenePrefabBase>(pNode->nodePath);

		ae::math::float4x4 renderWorldMatrix(pNode->worldMatrix.Transpose());
		if(!ae::render::C_RenderSys::Instance().GetResources().GetImpl()->CreateConstantBufferResource(reinterpret_cast<void*>(&renderWorldMatrix), sizeof(pNode->worldMatrix), cbFlags, ae::render::dx11::E_BSI_PerObject, pNode->m_CBuffer))
			DBG_ERR << "Unable to create per scene node constant buffer !" << std::endl;

		pNode = pNode->next;
	}

	if(pSceneResource->pNode)
		ae::scene::C_SceneSys::Inst().Scene().AddSceneNode(pSceneResource->pNode);
//	PrintDebug();
}

/*virtual*/ void C_SceneResourceHandler::Deactivate()
{
	ae::scene::S_SceneResource* pSceneResource = static_cast<ae::scene::S_SceneResource*>(GetResource());
	ae::scene::S_SceneNode* pNode = pSceneResource->pNode;
	if(pSceneResource->pNode)
		ae::scene::C_SceneSys::Inst().Scene().RemoveSceneNode(pNode);

	while(pNode)
	{
		ae::render::C_RenderSys::Instance().GetResources().GetImpl()->DestroyConstantBufferResource(pNode->m_CBuffer);
		pNode = pNode->next;
	}

	I_ResourceHandler::Deactivate();
}

/*virtual*/ void C_SceneResourceHandler::EnumerateElements(I_ResourceElementsEnumerator& enumerator)
{
	ResolvePointers();

	ae::scene::S_SceneResource* pSceneResource = static_cast<ae::scene::S_SceneResource*>(GetResource());
	ae::scene::S_SceneNode* pNode = pSceneResource->pNode;
	while(pNode)
	{
		enumerator.OnElement(pNode->hid.id, resourceTypeSceneNode);
		pNode = pNode->next;
	}
}

/*virtual*/ void C_SceneResourceHandler::PrintDebug()
{
	I_ResourceHandler::PrintDebug();
//	S_TextureResource* pTXTR = static_cast<S_TextureResource*>(GetResource());
	DBG_OUT << "Scene resource" << std::endl;
}

} } // namespace ae { namespace resource {
