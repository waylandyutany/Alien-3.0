#include "StdAfx.h"

#include "render_lights.h"
#include "../render_sys_init.h"

#include "render_scene.h"
#include "render_camera.h"

#include "../render_device/render_device.h"
#include "../render_device/render_resources.h"
#include "../render_device/resources/resource_effect.h"
#include "../render_device/resources/resource_texture.h"

#include "../../math_sys/utils.h"
#include "../../resource_sys/resource_sys.h"
#include "../../debug_sys/output/outputs.h"
#include "../../_sys/templates.h"

namespace ae { namespace render {

C_RenderLights::C_RenderLights(const S_Init& sInit, C_RenderResources& resources, C_RenderDevice& device) : 
	m_Resources(resources),
	m_Device(device),
	m_iMaxDirLights(sInit.maxDirLights), m_iMaxPointLights(sInit.maxPointLights)
{
	m_Resources.GetImpl()->CreateConstantBufferResource(0, sizeof(S_LightsInfo), dx11::E_BF_PS | dx11::E_BF_GS | dx11::E_BF_VS, 3, m_LightsInfoCB);
	m_Resources.GetImpl()->CreateStructuredBufferResource(0, sInit.maxDirLights, sizeof(S_DirLight), dx11::E_BF_PS | dx11::E_BF_GS | dx11::E_BF_VS, 4, m_DirLightsSB);
	m_Resources.GetImpl()->CreateStructuredBufferResource(0, sInit.maxPointLights, sizeof(S_PointLight), dx11::E_BF_PS | dx11::E_BF_GS | dx11::E_BF_VS, 5, m_PointLightsSB);
	m_DirLightsBuffer = (S_DirLight*)sInit.allocator.Alloc(sizeof(S_DirLight) * m_iMaxDirLights);
	m_PointLightsBuffer = (S_PointLight*)sInit.allocator.Alloc(sizeof(S_PointLight) * m_iMaxPointLights);
	// *****************************************************************************************************************
}

C_RenderLights::~C_RenderLights()
{
	// *****************************************************************************************************************
	m_Resources.GetImpl()->DestroyStructuredBufferResource(m_DirLightsSB);
	m_Resources.GetImpl()->DestroyStructuredBufferResource(m_PointLightsSB);
	m_Resources.GetImpl()->DestroyConstantBufferResource(m_LightsInfoCB);
	// *****************************************************************************************************************
	AE_ASSERT(m_PointLights.empty() && "Some point light was not destroyed !");
	AE_ASSERT(m_DirectionalLights.empty() && "Some directional light was not destroyed !");
}

void C_RenderLights::ActivateDefaultResources()
{
	m_pDfrLighting = C_ResourceSys::Inst().Resources().FindResource<ae::render::S_EffectResource>("default/effects/dfr_lighting.effe");
}

void C_RenderLights::DeactivateDefaultResources()
{
	m_pDfrLighting = 0;
}

// *****************************************************************************************************************
// *****************************************************************************************************************

void C_RenderLights::CalculateDirLightMatricies(S_DirLight& dirLight, const C_Camera& camera)
{
	// *****************************************************************************************************************
	ae::math::float3 pts[9],lightViewX, lightViewY, lightViewZ, origin;
	ae::math::float4x4 viewMat, projMat, viewProjMat, viewProjMatI;

	float radius, depthRange = 128.0f, zFar = 128.0f;
	camera.CalculateFrustumPoints(pts, 0.1f, zFar);
	radius = CalcMinBoundingSphere(pts, 8, origin);
	radius = ceilf(radius);

	lightViewZ = dirLight.dir;
	lightViewZ = lightViewZ.normalize();
	CalcOrthonormalBasis(lightViewZ,lightViewX,lightViewY);

	viewMat.Identity();
	viewMat.SetColumn(0, lightViewX, 0);
	viewMat.SetColumn(1, lightViewY, 0);
	viewMat.SetColumn(2, lightViewZ, 0);
	viewMat.SetColumn(3, origin, 1);
	viewMat = viewMat.Inverse().Transpose();

	CalcOrthoProjection(projMat, -depthRange,depthRange,-depthRange,depthRange,-depthRange,radius);
// *****************************************************************************************************************
#if 1
//	shadow edge shimmering prevention
	viewProjMat = viewMat * projMat;
	viewProjMatI = viewProjMat.Inverse();

	const float texelSize(2.0f / 4096.0f); // @todo need to pass valid shadow map resolution
	ae::math::float4 projOrigin(origin.x, origin.y, origin.z,0);
	projOrigin = viewProjMat * projOrigin;
	projOrigin.x += (texelSize - fmodf(projOrigin.x, texelSize));
	projOrigin.y += (texelSize - fmodf(projOrigin.y, texelSize));
	ae::math::float3 snapOrigin(viewProjMatI * projOrigin);

	viewMat.Identity();
	viewMat.SetColumn(0, lightViewX, 0);
	viewMat.SetColumn(1, lightViewY, 0);
	viewMat.SetColumn(2, lightViewZ, 0);
	viewMat.SetColumn(3, snapOrigin, 1);
	viewMat = viewMat.Inverse().Transpose();
#endif
// *****************************************************************************************************************
	D3DXMATRIX viewMatrix, projMatrix;
	memcpy(viewMatrix, viewMat.m, sizeof(viewMatrix));
	memcpy(projMatrix, projMat.m, sizeof(projMatrix));
// *****************************************************************************************************************

	D3DXMatrixMultiply(&dirLight.worldToLightProj, &viewMatrix, &projMatrix);

	D3DXMATRIX Clip2Tex = D3DXMATRIX(
		0.5,    0,    0,   0,
		0,	   -0.5,  0,   0,
		0,     0,     1,   0,
		0.5,   0.5,   0,   1 );
	D3DXMatrixMultiply(&dirLight.worldToLightClip, &dirLight.worldToLightProj, &Clip2Tex);
	D3DXMatrixTranspose(&dirLight.worldToLightProj, &dirLight.worldToLightProj);
	D3DXMatrixTranspose(&dirLight.worldToLightClip, &dirLight.worldToLightClip);
}

void C_RenderLights::PreRender(ID3D11DeviceContext* context, const C_Camera& camera)
{
	// Here is the place where I will allow sort out lights for rendering

	m_LightsInfo.numDirLights = (ae::u32)m_DirectionalLights.size();
	m_LightsInfo.numPointLights = (ae::u32)m_PointLights.size();
	m_LightsInfoCB.Update(context,&m_LightsInfo, sizeof(m_LightsInfo));
	m_LightsInfoCB.Activate(context);

	for(ae::u32 i(0); i < m_LightsInfo.numDirLights; i++)
	{
		memcpy(&m_DirLightsBuffer[i], m_DirectionalLights[i], sizeof(S_DirLight));
		CalculateDirLightMatricies(m_DirLightsBuffer[i], camera);
	}
	for(ae::u32 i(0); i < m_LightsInfo.numPointLights; i++)
		memcpy(&m_PointLightsBuffer[i], m_PointLights[i], sizeof(S_PointLight));

	m_DirLightsSB.Update(context, m_DirLightsBuffer, sizeof(S_DirLight) * m_LightsInfo.numDirLights);
	m_PointLightsSB.Update(context, m_PointLightsBuffer, sizeof(S_PointLight) * m_LightsInfo.numPointLights);
	m_DirLightsSB.Activate(context);
	m_PointLightsSB.Activate(context);
}


void C_RenderLights::Render(ID3D11DeviceContext* context)
{
//	PreRender(context); // its called before stencil_shadow pass

	if(m_pDfrLighting)	{
		m_pDfrLighting->m_Effect.Activate(context);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->DrawIndexed(3, 0, 0);
	}
}

// *****************************************************************************************************************
void C_RenderLights::AddDirLight(S_DirLight* pLight) {
	m_DirectionalLights.push_back(pLight);
}

void C_RenderLights::AddPointLight(S_PointLight* pLight) {
	m_PointLights.push_back(pLight);
}

void C_RenderLights::RemoveDirLight(S_DirLight* pLight) {
	ae::RemoveElement(m_DirectionalLights, pLight);
}

void C_RenderLights::RemovePointLight(S_PointLight* pLight) {
	ae::RemoveElement(m_PointLights, pLight);
}
// *****************************************************************************************************************

} } // namespace ae { namespace render {
