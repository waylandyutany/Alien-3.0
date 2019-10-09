#pragma once

#ifndef AE_RENDER_SYS_RENDER_2D_CONTETX_H
#define AE_RENDER_SYS_RENDER_2D_CONTETX_H

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
class C_RenderResources;
struct S_Init;
struct S_EffectResource;
struct S_TextureResource;

// *****************************************************************************************************************
// *****************************************************************************************************************
struct S_2DRenderResources
{
	S_2DRenderResources(C_RenderResources&, ae::u32 elementsBufferSize);
	~S_2DRenderResources();

	void ActivateDefaultResources();
	void DeactivateDefaultResources();
	void SetScreenDimensions(ae::u32 viewWidth, ae::u32 viewHeight, float screenNear, float screenDepth);

	typedef ae::u32 T_ElementType;
	typedef ae::u32 T_2DTexture;

	static const T_ElementType elementBox		= 0;
	static const T_ElementType elementRectangle	= 1;
	static const T_ElementType elementText		= 2;
	static const ae::u32 maxElementCharacters	= 16;

	static ae::u32 MakeElementFlags(T_ElementType eType, T_2DTexture eTexture)
	{
		return (eType & 0xf) | ((eTexture & 0xffff) << 4);
	}

	#pragma pack(push)
	#pragma pack(1)
	struct S_2DBoxElement
	{
		ae::math::float2 pxy,psxy;
		ae::math::float2 txy,tsxy;
		ae::math::float4 color;
		ae::u32 elementFlags;
		ae::math::float4 reserved;
	};
	struct S_2DRectangleElement
	{
		ae::math::float2 pxy,psxy;
		ae::math::float2 thickness, padding1;
		ae::math::float4 color;
		ae::u32 elementFlags;
		ae::math::float4 reserved;
	};
	struct S_2DTextElement
	{
		ae::math::float2 pxy,psxy;
		ae::u32 char0003, char0407;
		ae::u32 char0811, char1215;
		ae::math::float4 color;
		ae::u32 elementFlags;
		ae::math::float4 reserved;
	};
	#pragma pack(pop)

	S_EffectResource*	m_Simple2D_Effect;
	S_EffectResource*	m_DfrComposeEffect;

	S_TextureResource*	m_FontTextures[4];
	S_TextureResource*	m_BckgTextures[4];

	C_StructuredBufferResource	m_StructuredBuffer;

	C_RenderResources&			m_Resources;
};

// *****************************************************************************************************************
// *****************************************************************************************************************
typedef S_2DRenderResources::T_2DTexture T_2DTexture;
static const T_2DTexture E_2DT_None  = 0;
static const T_2DTexture E_2DT_Font0 = 1;
static const T_2DTexture E_2DT_Font1 = 2;
static const T_2DTexture E_2DT_Font2 = 4;
static const T_2DTexture E_2DT_Font3 = 8;
static const T_2DTexture E_2DT_Text0 = 16;
static const T_2DTexture E_2DT_Text1 = 32;
static const T_2DTexture E_2DT_Text2 = 64;
static const T_2DTexture E_2DT_Text3 = 128;
// *****************************************************************************************************************
class C_Render2DContext
{
	C_Render2DContext();
	NOCOPY_CLASS(C_Render2DContext);

	C_Render2DContext(const S_Init&, C_RenderResources&);
	friend class C_RenderScene;
// *****************************************************************************************************************
	ae::u32 m_NumElements;
// *****************************************************************************************************************
	S_2DRenderResources m_Resources;
	ae::base::C_IncrementalAllocator m_Allocator;
// *****************************************************************************************************************
	void RenderDfrCompose(ID3D11DeviceContext* context);
	void Render2D(ID3D11DeviceContext* context);
public:
	void DrawBox(T_2DTexture t, ae::s32 x, ae::s32 y, ae::s32 sx, ae::s32 sy, float tx, float ty, float tsx, float tsy, const ae::math::float4& color);
	void DrawBox(ae::s32 x, ae::s32 y, ae::s32 sx, ae::s32 sy, const ae::math::float4& color);
	void DrawRectangle(ae::s32 x, ae::s32 y, ae::s32 sx, ae::s32 sy, ae::u32 thickness, const ae::math::float4& color);
	void DrawText(T_2DTexture t, ae::s32 x, ae::s32 y, ae::s32 fsx, ae::s32 fsy, ae::s32 shrinkx,const char* text, const ae::math::float4& color);
};

} } // namespace ae { namespace render {

#endif // #ifdef AE_RENDER_SYS_RENDER_2D_CONTETX_H
