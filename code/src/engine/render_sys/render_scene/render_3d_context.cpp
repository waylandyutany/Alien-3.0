#include "StdAfx.h"

#include "render_3d_context.h"
#include "../render_sys_init.h"

#include "render_scene.h"

#include "../render_device/render_resources.h"
#include "../render_device/resources/resource_effect.h"

#include "../../resource_sys/resource_sys.h"

#include "../../debug_sys/output/outputs.h"

namespace ae { namespace render {
// *****************************************************************************************************************
// *****************************************************************************************************************

S_3DRenderResources::S_3DRenderResources(C_RenderResources& resources, const S_Init& sInit) :
	m_Resources(resources),
	m_Simple3DWired_Effect(0),
	m_Simple3DSolid_Effect(0)
{
	const ae::u32 elementStructSize(sizeof(S_3DElement));
	const ae::u32 maxNumElements(sInit.contextMemory3D / elementStructSize);
	m_Resources.GetImpl()->CreateStructuredBufferResource(0,maxNumElements, elementStructSize,dx11::E_BF_GS, dx11::E_BSI_PerFrame, m_StructuredBuffer);
}

S_3DRenderResources::~S_3DRenderResources()
{
	m_Resources.GetImpl()->DestroyStructuredBufferResource(m_StructuredBuffer);
}

void S_3DRenderResources::ActivateDefaultResources()
{
	m_Simple3DWired_Effect = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_EffectResource>("default/effects/simple_3d_wired.effe");
	m_Simple3DSolid_Effect = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_EffectResource>("default/effects/simple_3d_solid.effe");
}

void S_3DRenderResources::DeactivateDefaultResources()
{
	m_Simple3DWired_Effect = m_Simple3DSolid_Effect = 0;
}

// *****************************************************************************************************************
// *****************************************************************************************************************

C_Render3DContext::C_Render3DContext(const S_Init& sInit, C_RenderResources& resources) :
	m_NumElements(0),
	m_Resources(resources, sInit)
{
	m_Allocator.Create(sInit.allocator.Alloc(sInit.contextMemory3D), sInit.contextMemory3D);
	m_Allocator.Reset(true);
}

void C_Render3DContext::Render(ID3D11DeviceContext* context)
{
	m_Resources.m_StructuredBuffer.Update(context, m_Allocator.GetBottomPointer(), 0);
	m_Resources.m_StructuredBuffer.Activate(context);

	// first solid shapes are rendered
	if(m_Resources.m_Simple3DSolid_Effect)
	{
		m_Resources.m_Simple3DSolid_Effect->m_Effect.Activate(context);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->DrawIndexedInstanced(3,m_NumElements, 0,0,0);
	}

	// then wired shapes are rendered
	if(m_Resources.m_Simple3DWired_Effect)
	{
		m_Resources.m_Simple3DWired_Effect->m_Effect.Activate(context);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		context->DrawIndexedInstanced(2,m_NumElements, 0,0,0);
	}

	m_Allocator.Reset(true);
	m_NumElements = 0;
}

void C_Render3DContext::DrawLine(const ae::math::float3& p0, const ae::math::float3& p1, const ae::math::float4& color)
{
	S_3DRenderResources::S_3DElement* pElement = reinterpret_cast<S_3DRenderResources::S_3DElement*>(m_Allocator.Alloc(sizeof S_3DRenderResources::S_3DElement));
	AE_ASSERT(pElement && "C_Render3DContext out of memory !");
	if(!pElement) return;

	pElement->color = color;
	pElement->p0 = p0;
	pElement->p1 = p1;
	pElement->elementFlags = S_3DRenderResources::MakeElementFlags(S_3DRenderResources::elementLine, false, 0);

	m_NumElements++;
}

void C_Render3DContext::DrawLine(float x0,float y0,float z0,float x1,float y1,float z1, const ae::math::float4& color)
{
	DrawLine(ae::math::float3(x0,y0,z0), ae::math::float3(x1,y1,z1), color);
}

void C_Render3DContext::DrawCross(const ae::math::float3& point, float crossSize, const ae::math::float4& color)
{
	DrawLine(point.x - crossSize, point.y, point.z, point.x + crossSize, point.y, point.z, color);
	DrawLine(point.x, point.y - crossSize, point.z, point.x, point.y + crossSize, point.z, color);
	DrawLine(point.x, point.y, point.z - crossSize, point.x, point.y, point.z + crossSize, color);
}

void C_Render3DContext::DrawTriangle(const ae::math::float3& p0, const ae::math::float3& p1, const ae::math::float3& p2, const ae::math::float4& color)
{
	S_3DRenderResources::S_3DElement* pElement = reinterpret_cast<S_3DRenderResources::S_3DElement*>(m_Allocator.Alloc(sizeof S_3DRenderResources::S_3DElement));
	AE_ASSERT(pElement && "C_Render3DContext out of memory !");
	if(!pElement) return;

	pElement->color = color;
	pElement->p0 = p0;
	pElement->p1 = p1;
	pElement->p2 = p2;

	pElement->elementFlags = S_3DRenderResources::MakeElementFlags(S_3DRenderResources::elementTriangle, true, 0);

	m_NumElements++;
}

void C_Render3DContext::DrawQuad(const ae::math::float3& p0, const ae::math::float3& p1, const ae::math::float3& p2, const ae::math::float3& p3, const ae::math::float4& color)
{
	// todo draw it via shader in order to save element's buffer
	DrawTriangle(p2, p1, p0, color);
	DrawTriangle(p3, p2, p0, color);
}

void C_Render3DContext::DrawBox(const ae::math::float3& center, const ae::math::float3& halfSize, const ae::math::float4& color)
{
	// --/+-/++/-+
	// todo draw it via shader in order to save element's buffer
	// front face
	DrawQuad(	center + ae::math::float3(-halfSize.x, halfSize.y, halfSize.z),
				center + ae::math::float3(halfSize.x, halfSize.y, halfSize.z), 
				center + ae::math::float3(halfSize.x, -halfSize.y, halfSize.z), 
				center + ae::math::float3(-halfSize.x, -halfSize.y, halfSize.z), color);
	// back face
	DrawQuad(	center + ae::math::float3(-halfSize.x, -halfSize.y, -halfSize.z), 
				center + ae::math::float3(halfSize.x, -halfSize.y, -halfSize.z), 
				center + ae::math::float3(halfSize.x, halfSize.y, -halfSize.z), 
				center + ae::math::float3(-halfSize.x, halfSize.y, -halfSize.z), color);

	// top face
	DrawQuad(	center + ae::math::float3(-halfSize.x, halfSize.y, -halfSize.z), 
				center + ae::math::float3(halfSize.x, halfSize.y, -halfSize.z), 
				center + ae::math::float3(halfSize.x, halfSize.y, halfSize.z), 
				center + ae::math::float3(-halfSize.x, halfSize.y, halfSize.z), color);
	// bottom face
	DrawQuad(	center + ae::math::float3(-halfSize.x, -halfSize.y, halfSize.z), 
				center + ae::math::float3(halfSize.x, -halfSize.y, halfSize.z), 
				center + ae::math::float3(halfSize.x, -halfSize.y, -halfSize.z), 
				center + ae::math::float3(-halfSize.x, -halfSize.y, -halfSize.z), color);

	// right face
	DrawQuad(	center + ae::math::float3(halfSize.x, -halfSize.y, halfSize.z), 
				center + ae::math::float3(halfSize.x, halfSize.y, halfSize.z), 
				center + ae::math::float3(halfSize.x, halfSize.y, -halfSize.z), 
				center + ae::math::float3(halfSize.x, -halfSize.y, -halfSize.z),
				color);
	// left face
	DrawQuad(	center + ae::math::float3(-halfSize.x, -halfSize.y, -halfSize.z), 
				center + ae::math::float3(-halfSize.x, halfSize.y, -halfSize.z), 
				center + ae::math::float3(-halfSize.x, halfSize.y, halfSize.z), 
				center + ae::math::float3(-halfSize.x, -halfSize.y, halfSize.z), color);
}

} } // namespace ae { namespace render {
