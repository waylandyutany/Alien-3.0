#include "StdAfx.h"

#include "render_2d_context.h"
#include "../render_sys_init.h"

#include "render_scene.h"

#include "../render_device/render_resources.h"
#include "../render_device/resources/resource_effect.h"
#include "../render_device/resources/resource_texture.h"

#include "../../resource_sys/resource_sys.h"

#include "../../debug_sys/output/outputs.h"

namespace ae { namespace render {

// *****************************************************************************************************************
// *****************************************************************************************************************

S_2DRenderResources::S_2DRenderResources(C_RenderResources& resources, ae::u32 elementsBufferSize) : 
	m_Simple2D_Effect(0),
	m_DfrComposeEffect(0),
	m_Resources(resources)
{
	memset(	m_FontTextures, 0, sizeof(m_FontTextures) );
	memset(	m_BckgTextures, 0, sizeof(m_BckgTextures) );

	AE_ASSERT(sizeof(S_2DBoxElement) == sizeof(S_2DRectangleElement) && sizeof(S_2DBoxElement) == sizeof(S_2DTextElement) && "All 2D elements must have same size !!!");

	const ae::u32 elementStructSize(sizeof(S_2DBoxElement));
	const ae::u32 maxNumElements(elementsBufferSize / elementStructSize);
	m_Resources.GetImpl()->CreateStructuredBufferResource(0,maxNumElements, elementStructSize, dx11::E_BF_GS, dx11::E_BSI_PerFrame, m_StructuredBuffer);
}

S_2DRenderResources::~S_2DRenderResources()
{
	m_Resources.GetImpl()->DestroyStructuredBufferResource(m_StructuredBuffer);
}

void S_2DRenderResources::ActivateDefaultResources()
{
	m_Simple2D_Effect = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_EffectResource>("default/effects/simple_2d.effe");
	m_DfrComposeEffect = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_EffectResource>("default/effects/dfr_compose.effe");

	m_FontTextures[0]  = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_TextureResource>("default/fonts/font0.txtr");
	m_FontTextures[1]  = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_TextureResource>("default/fonts/font1.txtr");
	m_BckgTextures[0]  = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_TextureResource>("default/materials/ripstop.txtr");
}

void S_2DRenderResources::DeactivateDefaultResources()
{
	m_Simple2D_Effect = m_DfrComposeEffect = 0;
	memset(	m_FontTextures, 0, sizeof(m_FontTextures) );
	memset(	m_BckgTextures, 0, sizeof(m_BckgTextures) );
}

// *****************************************************************************************************************
// *****************************************************************************************************************

C_Render2DContext::C_Render2DContext(const S_Init& sInit, C_RenderResources& resources) :
	m_NumElements(0),
	m_Resources(resources, sInit.contextMemory2D)
{
	m_Allocator.Create(sInit.allocator.Alloc(sInit.contextMemory2D), sInit.contextMemory2D);
	m_Allocator.Reset(true);
}

void C_Render2DContext::DrawBox(ae::s32 x, ae::s32 y, ae::s32 sx, ae::s32 sy, const ae::math::float4& color)
{
	S_2DRenderResources::S_2DBoxElement* pElement = reinterpret_cast<S_2DRenderResources::S_2DBoxElement*>(m_Allocator.Alloc(sizeof S_2DRenderResources::S_2DBoxElement));
	AE_ASSERT(pElement && "C_Render2DContext out of memory !");
	if(!pElement) return;

	pElement->pxy = ae::math::vector2<ae::s32>(x,y);
	pElement->psxy = ae::math::vector2<ae::s32>(sx,sy);

	pElement->color = color;
	pElement->elementFlags = S_2DRenderResources::MakeElementFlags(S_2DRenderResources::elementBox, 0);

	m_NumElements++;
}

void C_Render2DContext::DrawBox(T_2DTexture t, ae::s32 x, ae::s32 y, ae::s32 sx, ae::s32 sy, float tx, float ty, float tsx, float tsy, const ae::math::float4& color)
{
	S_2DRenderResources::S_2DBoxElement* pElement = reinterpret_cast<S_2DRenderResources::S_2DBoxElement*>(m_Allocator.Alloc(sizeof S_2DRenderResources::S_2DBoxElement));
	AE_ASSERT(pElement && "C_Render2DContext out of memory !");
	if(!pElement) return;

	pElement->pxy = ae::math::vector2<ae::s32>(x,y);
	pElement->psxy = ae::math::vector2<ae::s32>(sx,sy);
	pElement->txy = ae::math::float2(tx,ty);
	pElement->tsxy = ae::math::float2(tsx,tsy);

	pElement->color = color;
	pElement->elementFlags = S_2DRenderResources::MakeElementFlags(S_2DRenderResources::elementBox, t);

	m_NumElements++;
}

void C_Render2DContext::DrawRectangle(ae::s32 x, ae::s32 y, ae::s32 sx, ae::s32 sy, ae::u32 thickness, const ae::math::float4& color)
{
	S_2DRenderResources::S_2DRectangleElement* pElement = reinterpret_cast<S_2DRenderResources::S_2DRectangleElement*>(m_Allocator.Alloc(sizeof S_2DRenderResources::S_2DRectangleElement));
	AE_ASSERT(pElement && "C_Render2DContext out of memory !");
	if(!pElement) return;

	pElement->pxy = ae::math::vector2<ae::s32>(x,y);
	pElement->psxy = ae::math::vector2<ae::s32>(sx,sy);
	pElement->thickness = ae::math::vector2<ae::u32>(thickness,thickness);
	pElement->color = color;
	pElement->elementFlags = S_2DRenderResources::MakeElementFlags(S_2DRenderResources::elementRectangle, 0);

	m_NumElements++;
}

void C_Render2DContext::DrawText(T_2DTexture t, ae::s32 x, ae::s32 y, ae::s32 fsx, ae::s32 fsy, ae::s32 shrinkx, const char* text, const ae::math::float4& color)
{
	const size_t strLen(strlen(text));
	for(ae::u32 i = 0; i < strLen; i += S_2DRenderResources::maxElementCharacters)
	{
		S_2DRenderResources::S_2DTextElement* pElement = reinterpret_cast<S_2DRenderResources::S_2DTextElement*>(m_Allocator.Alloc(sizeof S_2DRenderResources::S_2DTextElement));
		AE_ASSERT(pElement && "C_Render2DContext out of memory !");
		if(!pElement) return;

		pElement->pxy = ae::math::vector2<ae::s32>(x + (i * fsx) + (i * shrinkx), y);
		pElement->psxy = ae::math::vector2<ae::s32>(fsx,fsy);
		pElement->color = color;
		pElement->elementFlags = S_2DRenderResources::MakeElementFlags(S_2DRenderResources::elementText, t);
		pElement->reserved.x = static_cast<float>(shrinkx);

		char* pText = reinterpret_cast<char*>(&pElement->char0003);
		memcpy(pText, text + i, ae::math::Min<size_t>(S_2DRenderResources::maxElementCharacters, strLen - i));

		m_NumElements++;
	}
}

void C_Render2DContext::RenderDfrCompose(ID3D11DeviceContext* context)
{
	if(m_Resources.m_DfrComposeEffect)
	{
		m_Resources.m_DfrComposeEffect->m_Effect.Activate(context);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->DrawIndexedInstanced(3 , 1, 0, 0, 0);
	}
}

void C_Render2DContext::Render2D(ID3D11DeviceContext* context)
{
	if(m_Resources.m_Simple2D_Effect)
	{
		m_Resources.m_StructuredBuffer.Update(context, m_Allocator.GetBottomPointer(), 0);
		m_Resources.m_Simple2D_Effect->m_Effect.Activate(context);
		m_Resources.m_StructuredBuffer.Activate(context);

		for(int i = 0; i < 4; i++)
			if(m_Resources.m_FontTextures[i]) m_Resources.m_FontTextures[i]->m_TextureResource.Activate(context, i);
		for(int i = 0; i < 4; i++)
			if(m_Resources.m_BckgTextures[i]) m_Resources.m_BckgTextures[i]->m_TextureResource.Activate(context, 4+i);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->DrawIndexedInstanced(3,m_NumElements, 0,0,0);
	}
	m_NumElements = 0; // Fucking BUG, I forgoted to clear m_NumElements after render
	m_Allocator.Reset(true);
}

} } // namespace ae { namespace render {
