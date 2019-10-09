#pragma once

#ifndef AE_RENDER_SYS_RENDER_LIGHTS_H
#define AE_RENDER_SYS_RENDER_LIGHTS_H

#ifdef AE_RENDER_DX11
	#include "../render_device/dx11/resources/resource_cb_dx11.h"
	#include "../render_device/dx11/resources/resource_structured_buffer_dx11.h"
	using ae::render::dx11::C_StructuredBufferResource;
	using ae::render::dx11::C_ConstantBufferResource;
#else
	#error No render resources available !
#endif // #ifdef AE_RENDER_DX11

#include "../../math_sys/types.h"
#include "../../base_sys/memory/allocators/incremental_allocator.h"

namespace ae { namespace render {

class C_RenderScene;
class C_Camera;
class C_RenderResources;
class C_RenderDevice;
struct S_Init;
struct S_EffectResource;
struct S_TextureResource;

// *****************************************************************************************************************
// *****************************************************************************************************************
struct S_DirLight
{
	ae::math::float4 color; // color.xyz is color rgb, color.w is color intensity
	ae::math::float4 dir; // dir.xyz is normalized light direction
	D3DXMATRIX worldToLightProj;
	D3DXMATRIX worldToLightClip;
};

struct S_PointLight
{
	ae::math::float4 color; // color.xyz is color rgb, color.w is color intensity
	ae::math::float4 posAndRadius; // dir.x is light radius
};

typedef std::deque<S_DirLight*> T_DirLights;
typedef std::deque<S_PointLight*> T_PointLights;

// *****************************************************************************************************************
// *****************************************************************************************************************
class C_RenderLights
{
	C_RenderLights();
	NOCOPY_CLASS(C_RenderLights);

	C_RenderLights(const S_Init&, C_RenderResources&, C_RenderDevice&);
	~C_RenderLights();
	friend class C_RenderScene;
	C_RenderResources& m_Resources;
	C_RenderDevice& m_Device;

// *****************************************************************************************************************
	const ae::u32 m_iMaxDirLights, m_iMaxPointLights;
// *****************************************************************************************************************
	ae::render::T_PointLights m_PointLights;
	ae::render::T_DirLights m_DirectionalLights;
// *****************************************************************************************************************
	S_EffectResource*			m_pDfrLighting;
	C_ConstantBufferResource	m_LightsInfoCB;
	C_StructuredBufferResource	m_DirLightsSB;
	C_StructuredBufferResource	m_PointLightsSB;
	struct S_LightsInfo {
		ae::u32 numDirLights;
		ae::u32 numPointLights;
		ae::u32 padding0, padding1; 
	};
	S_LightsInfo m_LightsInfo;
	S_DirLight* m_DirLightsBuffer;
	S_PointLight* m_PointLightsBuffer;
// *****************************************************************************************************************
	void CalculateDirLightMatricies(S_DirLight&, const C_Camera&);
public:
	void ActivateDefaultResources();
	void DeactivateDefaultResources();
	void PreRender(ID3D11DeviceContext* context, const C_Camera&);
	void Render(ID3D11DeviceContext* context);
// *****************************************************************************************************************
	void AddDirLight(S_DirLight*);
	void AddPointLight(S_PointLight*);
	void RemoveDirLight(S_DirLight*);
	void RemovePointLight(S_PointLight*);
};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RENDER_LIGHTS_H
