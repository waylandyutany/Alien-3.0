#pragma once

#ifndef AE_RENDER_SYS_RENDER_3D_CONTETX_H
#define AE_RENDER_SYS_RENDER_3D_CONTETX_H

#include "../../math_sys/types.h"
#include "../../base_sys/memory/allocators/incremental_allocator.h"

#ifdef AE_RENDER_DX11
	#include "../render_device/dx11/resources/resource_cb_dx11.h"
	#include "../render_device/dx11/resources/resource_structured_buffer_dx11.h"
	using ae::render::dx11::C_ConstantBufferResource;
	using ae::render::dx11::C_StructuredBufferResource;
#else
	#error No render resources available !
#endif // #ifdef AE_RENDER_DX11

namespace ae { namespace render {

class C_RenderScene;
class C_RenderResources;
struct S_EffectResource;
struct S_Init;

// *****************************************************************************************************************
// *****************************************************************************************************************

struct S_3DRenderResources
{
	S_3DRenderResources( C_RenderResources&, const S_Init&);
	~S_3DRenderResources();

	void ActivateDefaultResources();
	void DeactivateDefaultResources();

	struct S_3DLineVertex
	{
		ae::math::float3 pos;
		ae::math::float2 tex;
	};

	typedef ae::u32 T_ElementType;
	typedef ae::u32 T_2DTexture;
	static const T_ElementType elementLine		= 0;
	static const T_ElementType elementTriangle	= 1;

	static ae::u32 MakeElementFlags(T_ElementType eType, bool solid, T_2DTexture eTexture)
	{
		return (eType & 0xf) | ((eTexture & 0xffff) << 5) | (solid ? 1 << 4 : 0);
	}

	#pragma pack(push)
	#pragma pack(1)
	struct S_3DElement
	{
		ae::math::float3 p0,p1,p2;
		ae::math::float4 color;
		ae::u32 elementFlags;
	};
	#pragma pack(pop)
	
	D3DXMATRIX					m_PerspProjMatrix;
	D3DXMATRIX					m_ViewMatrix;
	C_StructuredBufferResource	m_StructuredBuffer;

	C_RenderResources&			m_Resources;
	S_EffectResource*			m_Simple3DWired_Effect;
	S_EffectResource*			m_Simple3DSolid_Effect;
};

// *****************************************************************************************************************
// *****************************************************************************************************************

class C_Render3DContext
{
	C_Render3DContext();
	NOCOPY_CLASS(C_Render3DContext);

	C_Render3DContext(const S_Init&, C_RenderResources&);
	friend class C_RenderScene;
// *****************************************************************************************************************
	ae::u32 m_NumElements;
// *****************************************************************************************************************
	S_3DRenderResources m_Resources;
	ae::base::C_IncrementalAllocator m_Allocator;
// *****************************************************************************************************************
	void Render(ID3D11DeviceContext* context);
public:
	void DrawLine(const ae::math::float3& p0, const ae::math::float3& p1, const ae::math::float4& color);
	void DrawLine(float x0, float y0, float z0, float x1, float y1, float z1, const ae::math::float4& color);
	void DrawCross(const ae::math::float3& point, float crossSize, const ae::math::float4& color);

	void DrawTriangle(const ae::math::float3& p0, const ae::math::float3& p1, const ae::math::float3& p2, const ae::math::float4& color);
	void DrawQuad(const ae::math::float3& p0, const ae::math::float3& p1, const ae::math::float3& p2, const ae::math::float3& p3, const ae::math::float4& color);
	void DrawBox(const ae::math::float3& center, const ae::math::float3& halfSize, const ae::math::float4& color);
};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RENDER_3D_CONTETX_H
