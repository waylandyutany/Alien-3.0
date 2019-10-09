#include "StdAfx.h"

#include "render_scene.h"

#include "../../scene_sys/scene_sys.h"

#include "../render_device/resources/resource_texture.h"
#include "../render_device/render_resources.h"
#include "../render_device/render_device.h"

#include "../render_device/resources/resource_vb.h"
#include "../render_device/resources/resource_ib.h"
#include "../render_device/resources/resource_effect.h"
#include "../render_device/resources/resource_texture.h"
#include "../render_device/resources/resource_mesh.h"
#include "../../render_sys/render_sys.h"

#include "../../debug_sys/output/outputs.h"
#include "../../debug_sys/profiler/profiler.h"

#include "../../../engine/physic_sys/scene/dynamic_actor.h"
#include "../../../engine/input_sys/input_sys.h"

namespace ae { namespace render {

// *****************************************************************************************************************
// *****************************************************************************************************************
/*virtual*/ void C_RenderScene::C_PackageNotify::OnPackageLoaded(const char* packageName)
{
	if(!_stricmp(packageName, "default"))
	{
		m_Scene.m_2DContext.m_Resources.ActivateDefaultResources();
		m_Scene.m_3DContext.m_Resources.ActivateDefaultResources();
		m_Scene.m_LightsRender.ActivateDefaultResources();
		m_Scene.m_pStencilShadows = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_EffectResource>("default/effects/stencil_shadows.effe");
		m_Scene.m_pShadowMap = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_EffectResource>("default/effects/shadow_map.effe");
	}
}

/*virtual*/ void C_RenderScene::C_PackageNotify::OnPackageUnloading(const char* packageName)
{
	if(!_stricmp(packageName, "default"))
	{
		m_Scene.m_2DContext.m_Resources.DeactivateDefaultResources();
		m_Scene.m_3DContext.m_Resources.DeactivateDefaultResources();
		m_Scene.m_LightsRender.DeactivateDefaultResources();
		m_Scene.m_pStencilShadows = 0;
		m_Scene.m_pShadowMap = 0;
	}
}
// *****************************************************************************************************************
// *****************************************************************************************************************

C_RenderScene::C_RenderScene(C_RenderDevice* device, C_RenderResources* resources, S_Init& sInit) :
	m_PackageNotify(*this),
	m_2DContext(sInit, *resources),
	m_3DContext(sInit, *resources),
	m_LightsRender(sInit, *resources, *device), 
	m_Device(device),
	m_Resources(resources),
	m_pStencilShadows(0),
	m_pShadowMap(0)
{
	camera.SetNear(.1f);
	camera.SetFar(1000.0f);

	dx11::T_BufferFlags flags = dx11::E_BF_VS;
	resources->GetImpl()->CreateStructuredBufferResource(resources->InstanciesBuffer().instanciesMatricies, resources->InstanciesBuffer().maxInstancies, sizeof(ae::math::float4x4),flags, dx11::E_BSI_PerObject, m_SBInstancies);

	m_Resources->GetImpl()->CreateConstantBufferResource(&m_FrameSB, sizeof(m_FrameSB), dx11::E_BF_GS | dx11::E_BF_VS | dx11::E_BF_PS, dx11::E_BSI_PerFrame, m_FrameCB);

	ae::resource::C_ResourceSys::Inst().Resources().RegisterPackageNotifier(&m_PackageNotify);
}

C_RenderScene::~C_RenderScene()
{
	ae::resource::C_ResourceSys::Inst().Resources().UnregisterPackageNotifier(&m_PackageNotify);

	m_Resources->GetImpl()->DestroyConstantBufferResource(m_FrameCB);
	m_Resources->GetImpl()->DestroyStructuredBufferResource(m_SBInstancies);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

void C_RenderScene::UpdateFrameSBCB(ID3D11DeviceContext* context)
{
	// m_FrameSB.projViewMatrix3DT calculation
	D3DXMATRIX perspProjMatrix, viewMatrix;

	camera.GetViewMatrix(viewMatrix);
	camera.GetProjMatrix(perspProjMatrix);

	D3DXMatrixMultiply(&m_FrameSB.projViewMatrix3DT, &viewMatrix, &perspProjMatrix);
	D3DXMatrixInverse(&m_FrameSB.invProjViewMatrix3DT, 0, &m_FrameSB.projViewMatrix3DT);

	D3DXMatrixTranspose(&m_FrameSB.invProjViewMatrix3DT, &m_FrameSB.invProjViewMatrix3DT);
	D3DXMatrixTranspose(&m_FrameSB.projViewMatrix3DT, &m_FrameSB.projViewMatrix3DT);

	// m_FrameSB.projMatrix2DT calculation
	D3DXMatrixOrthoLH(&m_FrameSB.projMatrix2DT, (float)m_ViewWidth, (float)m_ViewHeight, camera.GetNear(), camera.GetFar());
	D3DXMatrixTranspose(&m_FrameSB.projMatrix2DT, &m_FrameSB.projMatrix2DT);

	// 2D screen stuff
	m_FrameSB.screenZeroPoint = ae::math::float2(-(float)m_ViewWidth / 2.0f, m_ViewHeight / 2.0f);
	m_FrameSB.screenScale = ae::math::float2(1, 1);

	// Updating and activation of frame constant buffer to render context
	m_FrameCB.Update(context, &m_FrameSB, sizeof(m_FrameSB));
	m_FrameCB.Activate(context);
}

void C_RenderScene::SetScreenDimensions(ae::u32 viewWidth, ae::u32 viewHeight)
{
	m_ViewWidth = viewWidth;
	m_ViewHeight = viewHeight;

	camera.SetAspect((float)m_ViewWidth / (float)m_ViewHeight);

	m_Device->ResizeBuffers(viewWidth, viewHeight);
}

void C_RenderScene::SetCamera(const C_Camera& cam)
{
	// well we must separate camera to scene and render camera
	// for now we store and restore projection near,far and aspect value whose are render related
	float fNear(camera.GetNear()), fFar(camera.GetFar()), fAspect(camera.GetAspect());

	camera = cam;
	
	// restoring render related proj values
	camera.SetNear(fNear);
	camera.SetFar(fFar);
	camera.SetAspect(fAspect);

	m_FrameSB.viewPos = cam.GetPos();
}

const C_Camera& C_RenderScene::GetCamera() const {
	return camera;
}
/*
bool C_RenderScene::GetMouseRay(ae::math::float3& start, ae::math::float3& end) const
{
	if(ae::input::C_InputSys::Inst().IsFrozen())
		return false;

	ae::math::float2 mouseCursorPos(ae::input::C_InputSys::Inst().MouseCursorPos());
	ae::math::float2 screenDimensions((float)m_ViewWidth, (float)m_ViewHeight);
	if( mouseCursorPos.x < 0.0f || screenDimensions.x < mouseCursorPos.x ||
		mouseCursorPos.y < 0.0f || screenDimensions.y < mouseCursorPos.y)
		return false;

	ae::math::float3 mouseProjectSpace(mouseCursorPos.x / screenDimensions.x, mouseCursorPos.y / screenDimensions.y, 0);
	mouseProjectSpace.x = (mouseProjectSpace.x * 2.0f) - 1.0f;
	mouseProjectSpace.y = ((1.0f - mouseProjectSpace.y) * 2.0f) - 1.0f;

	start = end = mouseProjectSpace;

	// todo need to finish, but mouseProjectSpace is valid
	return true;
}
*/
// *****************************************************************************************************************
// *****************************************************************************************************************

void C_RenderScene::SetAmbientColor(const ae::math::float4& color) {
	m_FrameSB.ambientColor = color;
}

void C_RenderScene::SetSceneColor(const ae::math::float4& color) {
	m_FrameSB.globalColor = color;
}

ae::math::float4 C_RenderScene::GetAmbientColor() const {
	return m_FrameSB.ambientColor;
}

ae::math::float4 C_RenderScene::GetSceneColor() const {
	return m_FrameSB.globalColor;
}

// *********************************************************************************************************
// *********************************************************************************************************

#ifdef AE_EDITOR
class C_UpdateSceneNodes : public ae::scene::C_Scene::I_NodesEnumerator
{
	NOCOPY_CLASS(C_UpdateSceneNodes);
	ID3D11DeviceContext* pContext;
public:
	C_UpdateSceneNodes(ID3D11DeviceContext* context) : pContext(context) {}
	virtual void OnNode(ae::scene::S_SceneNode& sceneNode, ae::scene::S_ScenePrefabBase& sceneInstNode)
	{
		ae::math::float4x4 tMat = sceneNode.worldMatrix.Transpose();
		sceneNode.m_CBuffer.Update(pContext, &tMat, sizeof(tMat));

		ae::scene::S_ScenePrefab_MGeometry& mgeometry((ae::scene::S_ScenePrefab_MGeometry&)sceneInstNode);
		if(!mgeometry.pGeometry) return;
		if(mgeometry.numInstances)
		{
			ae::render::S_InstanciesBuffer& inst(ae::render::C_RenderSys::Instance().GetResources().InstanciesBuffer());
			for(ae::u32 i = 0; i < mgeometry.numInstances; i++)
				inst.instanciesMatricies[i] = mgeometry.instances[i].worldMatrix.Transpose();
			mgeometry.instancesCB.Update(pContext, inst.instanciesMatricies, sizeof(ae::math::float4x4) * mgeometry.numInstances);
			inst.Clear();
		}
	}
};

void C_RenderScene::UpdateStaticNodesTransformation(ID3D11DeviceContext* context)
{
	C_UpdateSceneNodes updater(context);
	ae::scene::C_SceneSys::Inst().Scene().EnumerateNodes(updater, ae::scene::E_SINT_MGeometry);
}
#endif // #ifdef AE_EDITOR

void C_RenderScene::PreRender()
{
	ID3D11DeviceContext* context = m_Device->GetImpl()->GetContext();
	UpdateFrameSBCB(context);

#ifdef AE_EDITOR
	if(bUpdateStaticNodesTransformation) {
		UpdateStaticNodesTransformation(context);
		bUpdateStaticNodesTransformation = false;
	}
#endif
}

//#define USE_STENCIL_SHADOWS

void C_RenderScene::Render()
{
	ID3D11DeviceContext* context = m_Device->GetImpl()->GetContext();
// *********************************************************************************************************
	m_Device->BeginRender();
// *********************************************************************************************************
	m_Device->BeginSceneRender();
	RenderScene(context);
// *********************************************************************************************************
	m_LightsRender.PreRender(context, camera);
#ifdef USE_STENCIL_SHADOWS
	m_Device->BeginStencilShadowsRender();
	RenderSceneShadows(context);
#endif // #ifdef USE_STENCIL_SHADOWS
// *********************************************************************************************************
	m_Device->BeginShadowMapRender();
#ifndef USE_STENCIL_SHADOWS
	RenderShadowMaps(context);
#endif // #ifndef USE_STENCIL_SHADOWS
// *********************************************************************************************************
	m_Device->BeginLightRender();
	m_LightsRender.Render(context);
// *********************************************************************************************************
	m_Device->BeginDfrComposeRender();
	m_2DContext.RenderDfrCompose(context);
// *********************************************************************************************************
	m_3DContext.Render(context);
	m_Device->Begin2DRender();
	m_2DContext.Render2D(context);
// *********************************************************************************************************
	m_Device->EndRender();
}

// *********************************************************************************************************
// *********************************************************************************************************
class C_SceneRenderNodes : public ae::scene::C_Scene::I_NodesEnumerator
{
	NOCOPY_CLASS(C_SceneRenderNodes);
	ID3D11DeviceContext* m_pContext;
	C_RenderScene& m_RenderScene;
	bool m_bRenderShadowCasters;
public:
	C_SceneRenderNodes(C_RenderScene& renderScene, ID3D11DeviceContext* pContext, bool bShadows) : 
		m_RenderScene(renderScene), m_pContext(pContext), m_bRenderShadowCasters(bShadows) {}

	virtual void OnNode(ae::scene::S_SceneNode& sceneNode, ae::scene::S_ScenePrefabBase& sceneInstNode)
	{
		ae::scene::S_ScenePrefab_MGeometry& mgeometry((ae::scene::S_ScenePrefab_MGeometry&)sceneInstNode);
		if(!mgeometry.pGeometry) return;
		if(m_bRenderShadowCasters && !mgeometry.bCastShadows) return;
		if(mgeometry.numInstances)
			mgeometry.instancesCB.Activate(m_pContext);
		else
			m_RenderScene.m_SBInstancies.Activate(m_pContext);

		mgeometry.pMesh->m_pVBResource->m_Buffer.Activate(m_pContext);
		mgeometry.pMesh->m_pIBResource->m_Buffer.Activate(m_pContext);
		mgeometry.pMesh->m_InputLayoutResource.Activate(m_pContext);

		if(mgeometry.pMesh->m_pEffect && !m_bRenderShadowCasters)
			mgeometry.pMesh->m_pEffect->m_Effect.Activate(m_pContext);

		sceneNode.m_CBuffer.Activate(m_pContext);

		S_MeshResource::S_Geometry::S_Triangles* pTriangles = mgeometry.pGeometry->triangles;
		while(pTriangles)
		{
			if(!m_bRenderShadowCasters) {
				pTriangles->material->m_CBuffer.Activate(m_pContext);
				if(pTriangles->material->pDiffuseTexture)
					pTriangles->material->pDiffuseTexture->m_TextureResource.Activate(m_pContext, 0);
			}

			if(mgeometry.numInstances)
				m_pContext->DrawIndexedInstanced(pTriangles->indexCount, mgeometry.numInstances, pTriangles->indexOffset, pTriangles->vertexOffset,0);
			else
				m_pContext->DrawIndexed(pTriangles->indexCount, pTriangles->indexOffset, pTriangles->vertexOffset);

			pTriangles = pTriangles->next;
		}
	}

	virtual void OnDynamicNode(ae::scene::C_DynamicSceneNode& dynamicSceneNode)
	{
		m_RenderScene.m_SBInstancies.Activate(m_pContext);

		dynamicSceneNode.m_pMesh->m_pVBResource->m_Buffer.Activate(m_pContext);
		dynamicSceneNode.m_pMesh->m_pIBResource->m_Buffer.Activate(m_pContext);
		dynamicSceneNode.m_pMesh->m_InputLayoutResource.Activate(m_pContext);

		if(dynamicSceneNode.m_pMesh->m_pEffect && !m_bRenderShadowCasters)
			dynamicSceneNode.m_pMesh->m_pEffect->m_Effect.Activate(m_pContext);

		ae::math::float4x4 transformation;
		dynamicSceneNode.m_pDynamicActor->GetTransformation(transformation);
		transformation = transformation.Transpose();
		dynamicSceneNode.m_CBuffer.Update(m_pContext, (void*)&transformation, sizeof(transformation));
		dynamicSceneNode.m_CBuffer.Activate(m_pContext);

		S_MeshResource::S_Geometry::S_Triangles* pTriangles = dynamicSceneNode.m_pGeometry->triangles;
		while(pTriangles)
		{
			if(!m_bRenderShadowCasters) {
				pTriangles->material->m_CBuffer.Activate(m_pContext);
				if(pTriangles->material->pDiffuseTexture)
					pTriangles->material->pDiffuseTexture->m_TextureResource.Activate(m_pContext, 0);
			}

			m_pContext->DrawIndexed(pTriangles->indexCount, pTriangles->indexOffset, pTriangles->vertexOffset);

			pTriangles = pTriangles->next;
		}

	};
};

void C_RenderScene::RenderSceneShadows(ID3D11DeviceContext* context)
{
	if(m_pStencilShadows)
	{
		m_pStencilShadows->m_Effect.Activate(context);
		C_SceneRenderNodes renderNodes(*this, context, true);
		ae::scene::C_SceneSys::Inst().Scene().EnumerateNodes(renderNodes, ae::scene::E_SINT_MGeometry);
		ae::scene::C_SceneSys::Inst().Scene().EnumerateDynamicNodes(renderNodes);
	}
}

void C_RenderScene::RenderShadowMaps(ID3D11DeviceContext* context)
{
	if(m_pShadowMap)
	{
		m_pShadowMap->m_Effect.Activate(context);
		C_SceneRenderNodes renderNodes(*this, context, true);
		ae::scene::C_SceneSys::Inst().Scene().EnumerateNodes(renderNodes, ae::scene::E_SINT_MGeometry);
		ae::scene::C_SceneSys::Inst().Scene().EnumerateDynamicNodes(renderNodes);
	}
}

void C_RenderScene::RenderScene(ID3D11DeviceContext* context)
{
	C_SceneRenderNodes renderNodes(*this, context, false);
	ae::scene::C_SceneSys::Inst().Scene().EnumerateNodes(renderNodes, ae::scene::E_SINT_MGeometry);
	ae::scene::C_SceneSys::Inst().Scene().EnumerateDynamicNodes(renderNodes);
}

} } // namespace ae { namespace render {
