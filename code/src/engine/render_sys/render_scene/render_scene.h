#pragma once

#ifndef AE_RENDER_SYS_RENDER_SCENE_H
#define AE_RENDER_SYS_RENDER_SCENE_H

#include "render_2d_context.h"
#include "render_3d_context.h"
#include "render_lights.h"
#include "render_camera.h"

#include "../../resource_sys/resources/resources_manager.h"

namespace ae { namespace render {

	namespace static_scene_node {
		struct S_StaticSceneNode;
	}

class C_RenderSys;
class C_RenderResources;
class C_RenderDevice;
struct S_EffectResource;
struct S_TextureResource;
struct S_Init;

// *****************************************************************************************************************
// *****************************************************************************************************************

class C_RenderScene
{
	friend class C_SceneRenderNodes;
// *****************************************************************************************************************
// *****************************************************************************************************************
	class C_PackageNotify : public ae::resource::I_PackageNotify
	{
		NOCOPY_CLASS(C_PackageNotify);
	public:
		C_PackageNotify(C_RenderScene& scene) : m_Scene(scene) { }
		C_RenderScene& m_Scene;
		virtual void OnPackageLoaded(const char* /*packageName*/);
		virtual void OnPackageUnloading(const char* /*packageName*/);
	};
	C_PackageNotify m_PackageNotify;
// *****************************************************************************************************************
	struct S_FrameSB { // Hold frame related variables like matrices for 2d/3d
		D3DXMATRIX projViewMatrix3DT;
		D3DXMATRIX projMatrix2DT;
		ae::math::float2 screenZeroPoint;
		ae::math::float2 screenScale;
		ae::math::float4 ambientColor;
		ae::math::float4 globalColor;
		D3DXMATRIX invProjViewMatrix3DT;
		ae::math::float3 viewPos;
		float padding;
	};
// *****************************************************************************************************************
// *****************************************************************************************************************
	C_Render2DContext m_2DContext;
	C_Render3DContext m_3DContext;
	C_RenderLights m_LightsRender;
// *****************************************************************************************************************
	C_StructuredBufferResource	m_SBInstancies;
	C_ConstantBufferResource	m_FrameCB;
	S_FrameSB					m_FrameSB;
// *****************************************************************************************************************
// *****************************************************************************************************************
	C_RenderDevice* m_Device;
	C_RenderResources* m_Resources;
	S_EffectResource* m_pStencilShadows;
	S_EffectResource* m_pShadowMap;
// *****************************************************************************************************************
	ae::u32 m_ViewWidth, m_ViewHeight;
	C_Camera camera;
// *****************************************************************************************************************
	C_RenderScene();
	NOCOPY_CLASS(C_RenderScene);
	C_RenderScene(C_RenderDevice*, C_RenderResources*, S_Init&);
	~C_RenderScene();
	friend class C_RenderSys;
// *****************************************************************************************************************
	void RenderScene(ID3D11DeviceContext* context);
	void RenderSceneShadows(ID3D11DeviceContext* context);
	void RenderShadowMaps(ID3D11DeviceContext* context);
	void UpdateFrameSBCB(ID3D11DeviceContext* context);
#ifdef AE_EDITOR
	bool bUpdateStaticNodesTransformation;
	void UpdateStaticNodesTransformation(ID3D11DeviceContext* context);
#endif
public:
#ifdef AE_EDITOR
	void UpdateStaticNodesTransformation() { bUpdateStaticNodesTransformation = true; }
#endif
	void GetScreenDimensions(ae::u32& viewWidth, ae::u32& viewHeight) { viewWidth = m_ViewWidth; viewHeight = m_ViewHeight; }
	void SetScreenDimensions(ae::u32 viewWidth, ae::u32 viewHeight);
	// *****************************************************************************************************************
	void SetCamera(const C_Camera&);
	const C_Camera& GetCamera() const;
	// if true, start and end vector contains valid mouse ray
	// false means that cursor is out of window or window has no focus
	// todo refactorize, due too much functionality and uses input system
	// and finalize
//	bool GetMouseRay(ae::math::float3& start, ae::math::float3& end) const;
	// *****************************************************************************************************************
	C_Render2DContext& Get2DContext() { return m_2DContext; }
	C_Render3DContext& Get3DContext() { return m_3DContext; }
	C_RenderLights& Lights() { return m_LightsRender; }
	// *****************************************************************************************************************
	void PreRender();
	void Render();
	// *****************************************************************************************************************
	void SetAmbientColor(const ae::math::float4& color);
	void SetSceneColor(const ae::math::float4& color);
	ae::math::float4 GetAmbientColor() const;
	ae::math::float4 GetSceneColor() const;
	// *****************************************************************************************************************
};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RENDER_SCENE_H
