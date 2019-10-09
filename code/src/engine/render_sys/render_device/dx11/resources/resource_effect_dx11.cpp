#include "StdAfx.h"

#include "resource_effect_dx11.h"

namespace ae { namespace render { namespace dx11 {

C_EffectResource::C_EffectResource() :
	m_pVertexShader(0),
	m_pPixelShader(0),
	m_pGeometryShader(0),
	m_pComputeShader(0),
	m_pHullShader(0),
	m_pDomainShader(0)
{
}

void C_EffectResource::Activate(ID3D11DeviceContext* context)
{
	if(m_pVertexShader)		context->VSSetShader(m_pVertexShader,	NULL, 0);
	if(m_pPixelShader)		context->PSSetShader(m_pPixelShader,	NULL, 0);
	if(m_pGeometryShader)	context->GSSetShader(m_pGeometryShader, NULL, 0);
	if(m_pComputeShader)	context->CSSetShader(m_pComputeShader,	NULL, 0);
	if(m_pHullShader)		context->HSSetShader(m_pHullShader,		NULL, 0);
	if(m_pDomainShader)		context->DSSetShader(m_pDomainShader,	NULL, 0);
}

} } } // namespace ae { namespace render { namespace dx11 {
